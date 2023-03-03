/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_BENCHMARKS_OPS_CPP_API_BASE_HPP
#define DML_BENCHMARKS_OPS_CPP_API_BASE_HPP

#include <types.hpp>
#include <ops/base.hpp>
#include <ops/params.hpp>
#include <ops/results.hpp>

#include <dml/dml.hpp>

namespace bench::ops::cpp_api
{
template<path_e path>
struct to_dml_path;
template <>
struct to_dml_path<path_e::cpu>
{
    using path = dml::software;
};
template <>
struct to_dml_path<path_e::dsa>
{
    using path = dml::hardware;
};
template <>
struct to_dml_path<path_e::auto_>
{
    using path = dml::automatic;
};
template<path_e path>
using to_dml_path_t = typename to_dml_path<path>::path;


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
    operation_base_t() noexcept {}
    ~operation_base_t() noexcept {}

    void init_lib_impl()
    {
        if(!cache_control_)
            throw std::runtime_error("bench::ops::cpp_api::operation_base_t: cache control disabling is not supported");
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

#endif //DML_BENCHMARKS_OPS_CPP_API_BASE_HPP
