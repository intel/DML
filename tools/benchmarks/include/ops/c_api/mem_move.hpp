/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_BENCHMARKS_OPS_C_API_MEM_MOVE_HPP
#define DML_BENCHMARKS_OPS_C_API_MEM_MOVE_HPP

#include <ops/c_api/base.hpp>

namespace bench::ops::c_api
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
    using base_api_t::job_;
    using base_t::bytes_read_;
    using base_t::bytes_written_;
    using base_t::common_params_;
    using base_t::cache_control_;

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
        std::uint32_t flags{0};
        if(params_.copy_mode_)
            flags |= DML_FLAG_COPY_ONLY;
        if(cache_control_)
            flags |= DML_FLAG_PREFETCH_CACHE;

        if(common_params_.batch_size_)
        {
            for(size_t i = 0; i < dst_.size(); ++i)
            {
                auto status = dml_batch_set_mem_move_by_index(job_,
                                                              i,
                                                              const_cast<std::uint8_t*>(src_[i].data()),
                                                              dst_[i].data(),
                                                              static_cast<std::uint32_t>(dst_[i].size()),
                                                              flags);
                if(DML_STATUS_OK != status)
                    throw std::runtime_error(format("bench::ops::c_api::operation_base_t::init_lib_params_impl: dml_batch_set_mem_move_by_index() failed with status %d", status));
            }
        }
        else
        {
            job_->source_first_ptr      = const_cast<std::uint8_t*>(src_[0].data());
            job_->source_length         = static_cast<std::uint32_t>(src_[0].size());
            job_->destination_first_ptr = dst_[0].data();
            job_->destination_length    = static_cast<std::uint32_t>(dst_[0].size());
            job_->operation             = DML_OP_MEM_MOVE;
            job_->flags                |= flags;
        }
    }

    void sync_execute_impl()
    {
        auto status = dml_execute_job(job_);
        if(DML_STATUS_OK == status)
        {
            bytes_read_    = src_.size()*src_[0].size();
            bytes_written_ = dst_.size()*dst_[0].size();
        }
        else
            throw std::runtime_error(format("bench::ops::c_api::operation_base_t::sync_execute_impl: dml_execute_job() failed with status %d", status));
    }

    void async_submit_impl()
    {
        auto status = dml_submit_job(job_);
        if(DML_STATUS_OK != status)
            throw std::runtime_error(format("bench::ops::c_api::operation_base_t::async_submit_impl: dml_submit_job() failed with status %d", status));
    }

    task_status_e async_wait_impl()
    {
        auto status = dml_wait_job(job_, DML_WAIT_MODE_BUSY_POLL);
        if(DML_STATUS_OK == status)
        {
            bytes_read_    = src_.size()*src_[0].size();
            bytes_written_ = dst_.size()*dst_[0].size();
            return task_status_e::completed;
        }
        else
            throw std::runtime_error(format("bench::ops::c_api::operation_base_t::async_wait_impl: dml_wait_job() failed with status %d", status));
    }

    [[nodiscard]] task_status_e async_poll_impl()
    {
        auto status = dml_check_job(job_);
        if(DML_STATUS_BEING_PROCESSED == status)
            return task_status_e::in_progress;
        else
        {
            if(DML_STATUS_OK == status)
            {
                bytes_read_    = src_.size()*src_[0].size();
                bytes_written_ = dst_.size()*dst_[0].size();
                return task_status_e::completed;
            }
            else
                throw std::runtime_error(format("bench::ops::c_api::operation_base_t::async_poll_impl: dml_check_job() failed with status %d", status));
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

    params_t    params_;
    result_t    result_;
    src_type_t  src_;
    dst_type_t  dst_;
};
}

namespace bench::ops
{
template <path_e path, typename SrcAllocatorT, typename DstAllocatorT>
struct traits<operation_base_t<c_api::move_t<path, SrcAllocatorT, DstAllocatorT>>>
{
    using result_t = move_results_t<DstAllocatorT>;
};
}

#endif //DML_BENCHMARKS_OPS_C_API_MEM_MOVE_HPP
