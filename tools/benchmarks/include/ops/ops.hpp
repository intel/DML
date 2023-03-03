/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_BENCHMARKS_OPS_OPS_HPP
#define DML_BENCHMARKS_OPS_OPS_HPP

#include <ops/dispatcher.hpp>

namespace bench::ops
{
template <api_e api, path_e path, typename SrcAllocatorT = default_allocator_t, typename DstAllocatorT = default_allocator_t>
using move_t   = typename api_dispatcher_t<api, path, operation_e::move, SrcAllocatorT, DstAllocatorT>::impl_t;
}

#endif //DML_BENCHMARKS_OPS_OPS_HPP
