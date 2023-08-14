/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_BENCHMARKS_DETAILS_MEASURE_ASYNC_HPP
#define DML_BENCHMARKS_DETAILS_MEASURE_ASYNC_HPP

#include <utility.hpp>
#include <ops/ops.hpp>

#include <benchmark/benchmark.h>

//#define PER_THREAD_STAT
#ifdef PER_THREAD_STAT
#include <chrono>
#include <mutex>
#endif

namespace bench::details
{
template <path_e path, typename OperationT, typename ParamsT>
static statistics_t measure_async(benchmark::State &state, const case_params_t &common_params, OperationT &&operations, ParamsT &&params)
{
    statistics_t res{};
    auto threads   = state.threads();
    auto devices   = get_current_numa_accels();
    res.queue_size = common_params.queue_size_;
    res.operations = res.queue_size*((devices)?devices:1);

    // Operations are divided equally across threads
    res.operations_per_thread = res.operations/threads;
    if (res.operations_per_thread < 1)
        throw std::runtime_error("bench::details::measure_async: operation pool is too small for given threads");

    // Setup affinity. Try to distribute CPUs equality between accelerators
    set_affinity_map(state);

    // Initialize operations
    operations.resize(res.operations_per_thread);
    for (auto &operation: operations)
        operation.init(common_params, params);

    size_t completion_limit = res.operations_per_thread; // Do at least qdepth tasks for each iteration
    bool   first_iteration  = true;
#ifdef PER_THREAD_STAT
    std::chrono::high_resolution_clock::time_point timer_start;
    std::chrono::high_resolution_clock::time_point timer;
    timer_start = std::chrono::high_resolution_clock::now();
    std::uint64_t polls = 0;
#endif

    // Fetch input to required memory location
    // For cpu mode we do this every loop
    auto set_mem_loc_cpu = [&]()
    {
        if constexpr(path == path_e::cpu)
        {
            state.PauseTiming();
            for (auto &operation : operations)
                operation.mem_control(common_params.in_mem_, mem_loc_mask_e::src);
            state.ResumeTiming();
        }
    };

    // In case of accelerator set memory once before the loop. Accelerator does no prefetching itself.
    if(path != path_e::cpu)
    {
        for (auto &operation : operations)
            operation.mem_control(common_params.in_mem_, mem_loc_mask_e::src);
    }

    // Start measurement
    for (auto _ : state)
    {
        // Submit full queue on first iteration
        if(first_iteration)
        {
            set_mem_loc_cpu();
            for (auto &operation : operations)
                operation.async_submit();
            first_iteration = false;
        }

        // Complete at least queue size number of task for each iteration
        size_t completed = 0;
        while(completed < completion_limit)
        {
            // Loop over the queue
            for (size_t idx = 0; idx < operations.size(); ++idx)
            {
                // Check task completion
                auto task_status_e = operations[idx].async_poll();

#ifdef PER_THREAD_STAT
                if(task_status_e != task_status_e::retired)
                    polls++;
#endif

                // If task completed increment counters and resubmit the task
                if(task_status_e == task_status_e::completed)
                {
                    completed++;
                    res.completed_operations++;
                    res.data_read    += operations[idx].get_bytes_read();
                    res.data_written += operations[idx].get_bytes_written();

                    operations[idx].light_reset();

                    set_mem_loc_cpu(); // Reset memory location for CPU
                    operations[idx].async_submit();
                }
            }
        }
    }
#ifdef PER_THREAD_STAT
    auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - timer_start).count()*1000000000;
#endif

    // Wait for remaining tasks to complete
    for (auto &operation : operations)
        operation.async_wait();

    if(state.iterations() != 0){
        // Normalize metrics on iteration
        res.completed_operations /= state.iterations();
        res.data_read            /= state.iterations();
        res.data_written         /= state.iterations();
    }

#ifdef PER_THREAD_STAT
    static std::mutex guard;
    guard.lock();
    auto per_op = elapsed_seconds/(state.iterations()*res.completed_operations);
    printf("Thread: %3d; iters: %6lu; ops: %3u; completed/iter: %3u; polls/op: %6u; time/op: %5.0f ns\n", state.thread_index(), state.iterations(), res.operations_per_thread, (std::uint32_t)res.completed_operations, (std::uint32_t)(polls/(state.iterations()*res.completed_operations)), per_op);
    guard.unlock();
#endif

    return res;
}
}

#endif //BENCHMARK_DETAILS_MEASURE_ASYNC_HPP
