/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_BENCHMARKS_OPS_RESULTS_HPP
#define DML_BENCHMARKS_OPS_RESULTS_HPP

#include <types.hpp>
#include <utility.hpp>

#include <vector>

namespace bench::ops
{
template <typename AllocatorT>
struct move_results_t
{
    using data_type_t  = std::vector<std::uint8_t, AllocatorT>;
    using batch_type_t = std::vector<data_type_t>;

    move_results_t(batch_type_t &data) :
        data_(data) {}
    move_results_t(const move_results_t &) = default;
    move_results_t& operator=(const move_results_t &) = default;

    batch_type_t &data_;
};
}

#endif //DML_BENCHMARKS_OPS_RESULTS_HPP
