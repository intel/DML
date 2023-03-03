/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_BENCHMARKS_OPS_GLIBC_BASE_HPP
#define DML_BENCHMARKS_OPS_GLIBC_BASE_HPP

#include <types.hpp>
#include <ops/base.hpp>
#include <ops/params.hpp>
#include <ops/results.hpp>

#include <memory.h>

namespace bench::ops::glibc
{
template <typename DerivedT>
class operation_base_t: public ops::operation_base_t<DerivedT>
{
public:
    using base_t = ops::operation_base_t<DerivedT>;

protected:
    using base_t::copy_base;
    using base_t::common_params_;
    using base_t::cache_control_;

public:
    operation_base_t() noexcept
    {
        static_assert(DerivedT::path_v == path_e::cpu, "only CPU path is supported for glibc");
    }
    ~operation_base_t() noexcept {}

    void init_lib_impl()
    {
        if(cache_control_)
            throw std::runtime_error("bench::ops::glibc::operation_base_t: cache control option is not supported");
    }
    void deinit_lib_impl() {}

protected:
    friend class ops::operation_base_t<DerivedT>;

    void copy_base_api(const operation_base_t &rhl)
    {
        copy_base(rhl);
    }
};
}

#endif //DML_BENCHMARKS_OPS_GLIBC_BASE_HPP
