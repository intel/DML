/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_BENCHMARKS_OPS_PARAMS_HPP
#define DML_BENCHMARKS_OPS_PARAMS_HPP

#include <types.hpp>
#include <utility.hpp>

namespace bench::ops
{
struct move_params_t
{
    template <typename AllocatorT = default_allocator_t>
    using data_type_t  = std::vector<std::uint8_t, AllocatorT>;
    template <typename AllocatorT = default_allocator_t>
    using batch_type_t = std::vector<data_type_t<AllocatorT>>;

    explicit move_params_t() = default;
    move_params_t(const data_type_t<> &data, bool copy_mode = false) :
        copy_mode_(copy_mode),
        p_source_data_(&data)
    {
    }

    bool                 copy_mode_{false};
    const data_type_t<> *p_source_data_{nullptr};
};
}

#endif //DML_BENCHMARKS_OPS_PARAMS_HPP
