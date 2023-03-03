/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_BENCHMARKS_OPS_C_API_BASE_HPP
#define DML_BENCHMARKS_OPS_C_API_BASE_HPP

#include <types.hpp>
#include <ops/base.hpp>
#include <ops/params.hpp>
#include <ops/results.hpp>

#include <dml/dml.h>

#include <memory.h>
#include <stdexcept>

namespace bench::ops::c_api
{
template<path_e path>
static inline dml_path_t to_dml_path()
{
    if constexpr(path == path_e::cpu)
        return DML_PATH_SW;
    else if(path == path_e::dsa)
        return DML_PATH_HW;
    else if(path == path_e::auto_)
        return DML_PATH_AUTO;
    else
        throw std::runtime_error("bench::ops::c_api::to_dml_path: invalid path conversion");
}

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
        std::uint32_t size;
        auto status = dml_get_job_size(to_dml_path<DerivedT::path_v>(), &size);
        if (DML_STATUS_OK != status)
            throw std::runtime_error(format("bench::ops::c_api::operation_base_t::init_lib_impl: dml_get_job_size() failed with status %d", status));

        job_ = (dml_job_t *)malloc(size);
        if(!job_)
            throw std::runtime_error("bench::ops::c_api::operation_base_t::init_lib_impl: malloc() failed");

        status = dml_init_job(to_dml_path<DerivedT::path_v>(), job_);
        if (DML_STATUS_OK != status)
        {
            free(job_);
            job_ = nullptr;
            throw std::runtime_error(format("bench::ops::c_api::operation_base_t::init_lib_impl: dml_init_job() failed with status %d", status));
        }
        job_->numa_id = common_params_.numa_id_;

        if(common_params_.batch_size_)
        {
            status = dml_get_batch_size(job_, common_params_.batch_size_, &batch_buffer_size_);
            if (DML_STATUS_OK != status)
                throw std::runtime_error(format("bench::ops::c_api::operation_base_t::init_lib_impl: dml_get_batch_size() failed with status %d", status));

            batch_buffer_ = (std::uint8_t *)malloc(batch_buffer_size_);
            if(!batch_buffer_)
                throw std::runtime_error("bench::ops::c_api::operation_base_t::init_lib_impl: malloc() failed");

            job_->operation             = DML_OP_BATCH;
            job_->destination_first_ptr = batch_buffer_;
            job_->destination_length    = batch_buffer_size_;
        }
    }

    void deinit_lib_impl()
    {
        if (job_)
        {
            auto status = dml_finalize_job(job_);
            free(job_);
            job_ = nullptr;
            if (DML_STATUS_OK != status)
                throw std::runtime_error(format("bench::ops::c_api::operation_base_t::deinit_lib_impl: dml_finalize_job() failed with status %d", status));
        }
        if(batch_buffer_)
        {
            free(batch_buffer_);
            batch_buffer_      = nullptr;
            batch_buffer_size_ = 0;
        }
    }

protected:
    friend class ops::operation_base_t<DerivedT>;

    void copy_base_api(const operation_base_t &rhl)
    {
        copy_base(rhl);
    }

    dml_job_t     *job_{nullptr};
    std::uint8_t  *batch_buffer_{nullptr};
    std::uint32_t  batch_buffer_size_{0};
};
}

#endif //DML_BENCHMARKS_OPS_C_API_BASE_HPP
