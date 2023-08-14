/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_BENCHMARKS_DETAILS_MEASURE_SYNC_HPP
#define DML_BENCHMARKS_DETAILS_MEASURE_SYNC_HPP

#include <utility.hpp>
#include <ops/ops.hpp>

#include <benchmark/benchmark.h>

namespace bench::details
{
template <path_e path, typename OperationT, typename ParamsT>
static statistics_t measure_sync(benchmark::State &state, const case_params_t &common_params, OperationT &&operations, ParamsT &&params)
{
    statistics_t res{};
    res.queue_size = common_params.queue_size_;
    res.operations = res.queue_size;

    res.operations_per_thread = res.operations;
    if (state.threads() > 1)
        throw std::runtime_error("bench::details::measure_sync: synchronous measurements do not support threading");

    // Initialize operations
    operations.resize(res.queue_size);
    for (auto &operation: operations)
        operation.init(common_params, params);

    // Small warmup
    for (auto &operation : operations)
    {
        operation.async_submit();
        operation.async_wait();
        operation.light_reset();
    }

    // Start measurement
    for (auto _ : state)
    {
        // Fetch input to required memory location
        // For sync mode we do this every loop
        state.PauseTiming();
        for (auto &operation : operations)
            operation.mem_control(common_params.in_mem_, mem_loc_mask_e::src);
        state.ResumeTiming();

        // Submit task
        for (auto &operation : operations)
            operation.async_submit();

        for (auto &operation : operations)
        {
            // Wait for task to complete
            operation.async_wait();
            operation.light_reset();

            // Increment counters
            res.completed_operations++;
            res.data_read    += operation.get_bytes_read();
            res.data_written += operation.get_bytes_written();
        }
    }

    if(state.iterations() != 0){
        // Normalize metrics on iteration
        res.completed_operations /= state.iterations();
        res.data_read            /= state.iterations();
        res.data_written         /= state.iterations();
    }

    return res;
}
}

#endif //DML_BENCHMARKS_DETAILS_MEASURE_SYNC_HPP
