/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_BENCHMARKS_OPS_GLIBC_MEM_MOVE_HPP
#define DML_BENCHMARKS_OPS_GLIBC_MEM_MOVE_HPP

#include <ops/glibc/base.hpp>

namespace bench::ops::glibc
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

    move_t& operator=(const move_t &rhl){
        deinit_lib_impl();

        copy_base_api(rhl);

        params_ = rhl.params_;
        src_    = rhl.src_;
        dst_    = rhl.dst_;

        result_ = {dst_};

        return *this;
    }

protected:
    void init_buffers_impl(const params_t &params)
    {
        params_ =  params;
        setup_buffers(common_params_, src_, *params_.p_source_data_);
        setup_buffers(common_params_, dst_,  params_.p_source_data_->size());
    }

    void init_lib_params_impl() {}

    void sync_execute_impl()
    {

        if(params_.copy_mode_)
        {
            for(size_t i = 0; i < dst_.size(); ++i)
                memcpy(dst_[i].data(), src_[i].data(), dst_[i].size());
        }
        else
        {
            for(size_t i = 0; i < dst_.size(); ++i)
                memmove(dst_[i].data(), src_[i].data(), dst_[i].size());
        }
        bytes_read_    = src_.size()*src_[0].size();
        bytes_written_ = dst_.size()*dst_[0].size();
    }

    void async_submit_impl()
    {
        sync_execute_impl();
    }

    task_status_e               async_wait_impl() { return task_status_e::completed; }
    [[nodiscard]] task_status_e async_poll_impl() { return task_status_e::completed; }
    void                        light_reset_impl() noexcept {}

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
struct traits<operation_base_t<glibc::move_t<path, SrcAllocatorT, DstAllocatorT>>>
{
    using result_t = move_results_t<DstAllocatorT>;
};
}

#endif //DML_BENCHMARKS_OPS_GLIBC_MEM_MOVE_HPP
