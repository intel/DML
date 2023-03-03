/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_BENCHMARKS_OPS_DISPATCHER_HPP
#define DML_BENCHMARKS_OPS_DISPATCHER_HPP

#include <types.hpp>
#include "c_api/mem_move.hpp"
#include "cpp_api/mem_move.hpp"
#include "glibc/mem_move.hpp"

namespace bench::ops
{
template <api_e api, path_e path, operation_e operation, typename ...ArgsT>
struct api_dispatcher_t
{
};

template <path_e path, typename ...ArgsT>
struct api_dispatcher_t<api_e::c, path, operation_e::move, ArgsT...>
{
    using impl_t = c_api::move_t<path, ArgsT...>;
};
template <path_e path, typename ...ArgsT>
struct api_dispatcher_t<api_e::cpp, path, operation_e::move, ArgsT...>
{
    using impl_t = cpp_api::move_t<path, ArgsT...>;
};
template <path_e path, typename ...ArgsT>
struct api_dispatcher_t<api_e::glibc, path, operation_e::move, ArgsT...>
{
    using impl_t = glibc::move_t<path, ArgsT...>;
};
}

#endif //DML_BENCHMARKS_OPS_DISPATCHER_HPP
