/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_BENCHMARKS_OPS_BASE_HPP
#define DML_BENCHMARKS_OPS_BASE_HPP

#include <types.hpp>
#include <ops/results.hpp>

#include <cstdint>
#include <stdexcept>

namespace bench::ops
{
template <typename Type>
struct traits {};

template <typename DerivedT>
class operation_base_t
{
public:
    operation_base_t() noexcept {}
    ~operation_base_t() noexcept {}

    // Init library and set data for the measurement loop
    // Init has 3 parts:
    // - Initialization of operation buffers (not included in full_time)
    // - Initialization of operation context (included in full_time)
    // - Initialization of operation arguments (included in full_time)
    template <typename ParamsT>
    void init(const case_params_t &common_params, const ParamsT &params)
    {
        common_params_ = common_params;
        cache_control_ = get_mem_cc(common_params.out_mem_);
        common_params_.out_mem_ = get_mem_no_cc(common_params.out_mem_);
        if(common_params_.batch_size_ < 0)
            throw std::runtime_error("bench::ops::operation_base_t: invalid batch size");

        derived().deinit_lib_impl();         // Check initialization to be cleared
        derived().init_buffers_impl(params); // Init operation specific buffers
        if(common_params_.full_time_)
            return;

        derived().init_lib_impl();        // Init library specific context
        derived().init_lib_params_impl(); // Init operation specific arguments
    }

    // Synchronous operation execution
    void sync_execute()
    {
        if(common_params_.full_time_)
        {
            derived().init_lib_impl();
            derived().init_lib_params_impl();
        }

        // Fetch input to required memory location
        //derived().mem_control_impl(common_params_.in_mem_, mem_loc_mask_e::src);

        retired_ = true;
        derived().sync_execute_impl();

        // Fetch output to required memory location
        derived().mem_control_impl(common_params_.out_mem_, mem_loc_mask_e::dst);

        if(common_params_.full_time_)
            derived().deinit_lib_impl();
    }

    // Asynchronous operation execution
    void async_submit()
    {
        if(common_params_.full_time_)
        {
            derived().init_lib_impl();
            derived().init_lib_params_impl();
        }

        // Fetch input to required memory location
        //derived().mem_control_impl(common_params_.in_mem_, mem_loc_mask_e::src);

        retired_ = false;
        derived().async_submit_impl();
    }

    // Blocking wait for asynchronous operation
    task_status_e async_wait()
    {
        if(retired_)
            return task_status_e::retired;

        auto result = derived().async_wait_impl();
        if(result == task_status_e::completed)
        {
            // Fetch output to required memory location
            derived().mem_control_impl(common_params_.out_mem_, mem_loc_mask_e::dst);

            if(common_params_.full_time_)
                derived().deinit_lib_impl();
            retired_ = true;
        }
        return result;
    }

    // Non-blocking wait for asynchronous operation
    [[nodiscard]] task_status_e async_poll()
    {
        if(retired_)
            return task_status_e::retired;

        auto result = derived().async_poll_impl();
        if(result == task_status_e::completed)
        {
            // Fetch output to required memory location
            derived().mem_control_impl(common_params_.out_mem_, mem_loc_mask_e::dst);

            if(common_params_.full_time_)
                derived().deinit_lib_impl();
            retired_ = true;
        }
        return result;
    }

    // Lightweight reset of the operation
    void light_reset() noexcept
    {
        if(common_params_.full_time_)
            return;

        derived().light_reset_impl();
    }

    // Get result vectors
    auto get_result() noexcept -> typename traits<operation_base_t>::result_t&
    {
        return derived().get_result_impl();
    }

    void mem_control(mem_loc_e op, mem_loc_mask_e mask) const noexcept
    {
        derived().mem_control_impl(op, mask);
    }

    // Getters for amount of processed bytes for throughput stats
    size_t get_bytes_read()    { return bytes_read_; }
    size_t get_bytes_written() { return bytes_written_; }

protected:
    void copy_base(const operation_base_t &rhl)
    {
        common_params_  = rhl.common_params_;
        cache_control_  = rhl.cache_control_;
        retired_        = rhl.retired_;
        bytes_read_     = rhl.bytes_read_;
        bytes_written_  = rhl.bytes_written_;
    }

    const DerivedT& derived() const { return *static_cast<const DerivedT*>(this); }
    DerivedT&       derived()       { return *static_cast<DerivedT*>(this); }

    case_params_t common_params_{};
    bool          cache_control_{true};
    bool          retired_{true};
    std::size_t   bytes_read_{0};
    std::size_t   bytes_written_{0};
};
}

#endif //DML_BENCHMARKS_OPS_BASE_HPP
