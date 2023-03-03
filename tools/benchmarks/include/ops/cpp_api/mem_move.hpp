/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_BENCHMARKS_OPS_CPP_API_MEM_MOVE_HPP
#define DML_BENCHMARKS_OPS_CPP_API_MEM_MOVE_HPP

#include <ops/cpp_api/base.hpp>

namespace bench::ops::cpp_api
{
template <path_e path, typename SrcAllocatorT, typename DstAllocatorT>
class move_t: public operation_base_t<move_t<path, SrcAllocatorT, DstAllocatorT>>
{
public:
    using params_t    = move_params_t;
    using result_t    = move_results_t<DstAllocatorT>;
    using base_t      = ops::operation_base_t<move_t<path, SrcAllocatorT, DstAllocatorT>>;
    using base_api_t  = operation_base_t<move_t<path, SrcAllocatorT, DstAllocatorT>>;

    using src_type_t  = typename params_t::batch_type_t<SrcAllocatorT>;
    using dst_type_t  = typename result_t::batch_type_t;

    static constexpr auto path_v = path;

private:
    using base_api_t::copy_base_api;
    using base_api_t::deinit_lib_impl;
    using base_t::bytes_read_;
    using base_t::bytes_written_;
    using base_t::common_params_;
    using base_t::cache_control_;

    using allocator_t     = std::allocator<std::uint8_t>;
    using copy_handler_t  = dml::handler<dml::mem_copy_operation, allocator_t>;
    using move_handler_t  = dml::handler<dml::mem_move_operation, allocator_t>;
    using batch_handler_t = dml::handler<dml::batch_operation, allocator_t>;
    using dml_path        = to_dml_path_t<path>;

public:
    move_t() noexcept :
        result_{dst_} {}
    ~move_t() noexcept
    {
        deinit_lib_impl();
    }

    move_t(const move_t&rhl) :
        result_{dst_}
    {
        copy_base_api(rhl);

        copy_operation_       = rhl.copy_operation_;
        move_operation_       = rhl.move_operation_;
        batch_operation_      = rhl.batch_operation_;

        params_ = rhl.params_;
        src_    = rhl.src_;
        dst_    = rhl.dst_;
    }

protected:
    void init_buffers_impl(const params_t &params)
    {
        params_ = params;
        setup_buffers(common_params_, src_, *params_.p_source_data_);
        setup_buffers(common_params_, dst_,  params_.p_source_data_->size());
    }

    void init_lib_params_impl()
    {
        if(common_params_.batch_size_)
        {
            batch_sequence_ = dml::sequence<allocator_t>{(dml::size_t)common_params_.batch_size_};
            if(params_.copy_mode_)
            {
                for(size_t i = 0; i < dst_.size(); ++i)
                    batch_sequence_.add(copy_operation_, dml::make_view(src_[i].data(), src_[i].size()), dml::make_view(dst_[i].data(), dst_[i].size()));
            }
            else
            {
                for(size_t i = 0; i < dst_.size(); ++i)
                    batch_sequence_.add(move_operation_, dml::make_view(src_[i].data(), src_[i].size()), dml::make_view(dst_[i].data(), dst_[i].size()));
            }
        }
    }

    void sync_execute_impl()
    {
        dml::status_code status{};
        if(common_params_.batch_size_)
        {
            auto result = dml::execute<dml_path>(batch_operation_,
                                                 batch_sequence_,
                                                 common_params_.numa_id_);
            status = result.status;
        }
        else
        {
            if(params_.copy_mode_)
            {
                auto result = dml::execute<dml_path>(copy_operation_,
                                                     dml::make_view(src_[0].data(), src_[0].size()),
                                                     dml::make_view(dst_[0].data(), dst_[0].size()),
                                                     common_params_.numa_id_);
                status = result.status;
            }
            else
            {
                auto result = dml::execute<dml_path>(move_operation_,
                                                     dml::make_view(src_[0].data(), src_[0].size()),
                                                     dml::make_view(dst_[0].data(), dst_[0].size()),
                                                     common_params_.numa_id_);
                status = result.status;
            }
        }
        if(status == dml::status_code::ok)
        {
            bytes_read_    = src_.size()*src_[0].size();
            bytes_written_ = dst_.size()*dst_[0].size();
        }
        else
            throw std::runtime_error(format("bench::ops::cpp_api::move_t::sync_execute_impl: dml::execute() failed with status %d", status));
    }

    void async_submit_impl()
    {
        dml::status_code status{};
        if(common_params_.batch_size_)
        {
            batch_handler_ = dml::submit<dml_path>(batch_operation_,
                                                   batch_sequence_,
                                                   dml::default_execution_interface<dml_path>{},
                                                   common_params_.numa_id_);
            if(!batch_handler_.valid())
                status = batch_handler_.get().status;
        }
        else
        {
            if(params_.copy_mode_)
            {
                copy_handler_ = dml::submit<dml_path>(copy_operation_,
                                                      dml::make_view(src_[0].data(), src_[0].size()),
                                                      dml::make_view(dst_[0].data(), dst_[0].size()),
                                                      dml::default_execution_interface<dml_path>{},
                                                      common_params_.numa_id_);
                if(!copy_handler_.valid())
                    status = copy_handler_.get().status;
            }
            else
            {
                move_handler_ = dml::submit<dml_path>(move_operation_,
                                                      dml::make_view(src_[0].data(), src_[0].size()),
                                                      dml::make_view(dst_[0].data(), dst_[0].size()),
                                                      dml::default_execution_interface<dml_path>{},
                                                      common_params_.numa_id_);
                if(!move_handler_.valid())
                    status = move_handler_.get().status;
            }
        }
        if(status != dml::status_code::ok)
            throw std::runtime_error(format("bench::ops::cpp_api::move_t::async_submit_impl: dml::submit() failed with status %d", status));
    }

    task_status_e async_wait_impl()
    {
        dml::status_code status = (common_params_.batch_size_) ? batch_handler_.get().status : ((params_.copy_mode_)? copy_handler_.get().status : move_handler_.get().status);
        if(status == dml::status_code::ok)
        {
            bytes_read_    = src_.size()*src_[0].size();
            bytes_written_ = dst_.size()*dst_[0].size();
            return task_status_e::completed;
        }
        else
            throw std::runtime_error(format("bench::ops::cpp_api::move_t::async_wait_impl: dml::handler::get() failed with status %d", status));
    }

    [[nodiscard]] task_status_e async_poll_impl()
    {
        bool is_finished = (common_params_.batch_size_) ? batch_handler_.is_finished() : ((params_.copy_mode_) ? copy_handler_.is_finished() : move_handler_.is_finished());
        if(!is_finished)
            return task_status_e::in_progress;
        else
        {
            dml::status_code status = (common_params_.batch_size_) ? batch_handler_.get().status : ((params_.copy_mode_)? copy_handler_.get().status : move_handler_.get().status);
            if(status == dml::status_code::ok)
            {
                bytes_read_    = src_.size()*src_[0].size();
                bytes_written_ = dst_.size()*dst_[0].size();
                return task_status_e::completed;
            }
            else
                throw std::runtime_error(format("bench::ops::cpp_api::move_t::async_poll_impl: dml::handler::get() failed with status %d", status));
        }
    }

    void light_reset_impl() noexcept {}

    void mem_control_impl(mem_loc_e op, mem_loc_mask_e mask) const noexcept
    {
        if(mask&mem_loc_mask_e::src1)
        {
            for(auto &src : src_)
                details::mem_control(src.begin(), src.end(), op, true);
        }
        if(mask&mem_loc_mask_e::dst1)
        {
            for(auto &dst : dst_)
                details::mem_control(dst.begin(), dst.end(), op, false);
        }
    }

    result_t& get_result_impl() noexcept
    {
        return result_;
    }

private:
    friend class ops::operation_base_t<move_t>;
    friend class operation_base_t<move_t>;

    dml::mem_copy_operation    copy_operation_{};
    dml::mem_move_operation    move_operation_{};
    dml::batch_operation       batch_operation_{};
    dml::sequence<allocator_t> batch_sequence_{0};
    copy_handler_t             copy_handler_{};
    move_handler_t             move_handler_{};
    batch_handler_t            batch_handler_{};

    params_t    params_;
    result_t    result_;
    src_type_t  src_;
    dst_type_t  dst_;
};
}

namespace bench::ops
{
template <path_e path, typename SrcAllocatorT, typename DstAllocatorT>
struct traits<operation_base_t<cpp_api::move_t<path, SrcAllocatorT, DstAllocatorT>>>
{
    using result_t = move_results_t<DstAllocatorT>;
};
}

#endif //DML_BENCHMARKS_OPS_CPP_API_MEM_MOVE_HPP
