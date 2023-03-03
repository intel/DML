/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_BENCHMARKS_MEASURE_HPP
#define DML_BENCHMARKS_MEASURE_HPP

#include <utility.hpp>
#include <details/measure_sync.hpp>
#include <details/measure_async.hpp>

#include <benchmark/benchmark.h>

namespace bench
{
template <execution_e exec, path_e path, typename OperationT, typename ParamsT>
static inline statistics_t measure(benchmark::State &state, const case_params_t &common_params, OperationT &&operations, ParamsT &&params)
{
    if constexpr (exec == execution_e::async)
        return details::measure_async<path>(state, common_params, std::forward<OperationT>(operations), params);
    else
        return details::measure_sync<path>(state, common_params, std::forward<OperationT>(operations), params);
}
}

#endif //DML_BENCHMARKS_MEASURE_HPP
