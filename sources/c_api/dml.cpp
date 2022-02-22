/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml/dml.h>

#include <memory>
#include <new>
#include <limits>

#include "impl.hpp"
#include "job_view.hpp"
#include "macros.hpp"
#include "state.hpp"
#include "utils.hpp"

extern "C" dml_status_t dml_get_job_size(const dml_path_t dml_path, uint32_t *const job_size_ptr)
{
    CHECK_NULL(job_size_ptr);
    CHECK_PATH(dml_path);

    *job_size_ptr = static_cast<uint32_t>(dml::get_job_size());

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_init_job(const dml_path_t path, dml_job_t *const dml_job_ptr)
{
    CHECK_NULL(dml_job_ptr);
    CHECK_PATH(path);

    const auto job_ptr  = reinterpret_cast<uint8_t *>(dml_job_ptr);
    const auto job_size = sizeof(dml_job_t);

    // Zeroing-out everything
    std::fill_n(job_ptr, dml::get_job_size(), 0);

    // Special value for "any" numa id
    dml_job_ptr->numa_id = std::numeric_limits<decltype(dml_job_ptr->numa_id)>::max();

    // Take a pointer to the place right after the last struct member and align to default boundary.
    dml_job_ptr->internal_data_ptr = dml::align(job_ptr + job_size);

    new (dml_job_ptr->internal_data_ptr) dml::state();

    dml::job_view(dml_job_ptr).state().path = path;

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_execute_job(dml_job_t *const dml_job_ptr)
{
    CHECK_NULL(dml_job_ptr);

    return dml::execute(dml::job_view(dml_job_ptr));
}

extern "C" dml_status_t dml_submit_job(dml_job_t *const dml_job_ptr)
{
    CHECK_NULL(dml_job_ptr);

    return dml::submit(dml::job_view(dml_job_ptr));
}

extern "C" dml_status_t dml_check_job(dml_job_t *const dml_job_ptr)
{
    CHECK_NULL(dml_job_ptr);

    return dml::check(dml::job_view(dml_job_ptr));
}

extern "C" dml_status_t dml_wait_job(dml_job_t *const dml_job_ptr)
{
    CHECK_NULL(dml_job_ptr);

    return dml::wait(dml::job_view(dml_job_ptr));
}

extern "C" dml_status_t dml_finalize_job(dml_job_t *const dml_job_ptr)
{
    CHECK_NULL(dml_job_ptr);

    auto state_ptr = std::launder(reinterpret_cast<dml::state *>(dml_job_ptr->internal_data_ptr));
    std::destroy_at(state_ptr);

    return DML_STATUS_OK;
}
