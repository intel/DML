/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_JOB_HPP
#define DML_TESTING_JOB_HPP

#ifndef C_API
#error "This file is indended for C API only"
#endif

#include <dml/dml.h>

#include <cstdint>
#include <cstdlib>
#include <stdexcept>

namespace dml::testing
{
    class Job
    {
    public:
        explicit Job(): ptr_(nullptr)
        {
            std::uint32_t size = 0u;

#ifdef SW_PATH
            const auto path = DML_PATH_SW;
#endif

#ifdef HW_PATH
            const auto path = DML_PATH_HW;
#endif

#ifdef AUTO_PATH
            const auto path = DML_PATH_AUTO;
#endif

            dml_status_t status = dml_get_job_size(path, &size);

            if (DML_STATUS_OK != status)
            {
                throw std::runtime_error("Failed to get job size");
            }

            ptr_ = static_cast<dml_job_t*>(malloc(size));

            if (!ptr_)
            {
                throw std::bad_alloc();
            }

            status = dml_init_job(path, ptr_);

            if (DML_STATUS_OK != status)
            {
                free(ptr_);
                throw std::runtime_error("Failed to initialize job");
            }
        }

        ~Job() noexcept
        {
            if (ptr_)
            {
                dml_finalize_job(ptr_);
                std::free(ptr_);
            }
        }

        Job(const Job& other) = delete;

        Job(Job&& other) noexcept = delete;

        Job& operator=(const Job& other) = delete;

        Job& operator=(Job&& other) noexcept = delete;

        auto operator->() noexcept
        {
            return ptr_;
        }

        operator dml_job_t*() noexcept
        {
            return ptr_;
        }

    private:
        dml_job_t* ptr_;
    };
}  // namespace dml::testing

#endif  //DML_TESTING_JOB_HPP
