/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_IMPL_HPP
#define DML_IMPL_HPP

#include <dml/detail/ml/execution_path.hpp>
#include <dml/detail/ml/view.hpp>

#include "job_view.hpp"
#include "make_task.hpp"
#include "range_check.hpp"
#include "write_result.hpp"

namespace dml
{
    [[nodiscard]] static inline dml_status_t wait(job_view job, bool umwait) noexcept
    {
        auto task_view = detail::ml::make_view(job.state().task);

        switch (job.state().path)
        {
            case DML_PATH_SW:
                detail::ml::wait<detail::ml::execution_path::software>(task_view, umwait);
                break;
            case DML_PATH_HW:
                detail::ml::wait<detail::ml::execution_path::hardware>(task_view, umwait);
                break;
            case DML_PATH_AUTO:
                detail::ml::wait<detail::ml::execution_path::automatic>(task_view, umwait);
                break;
        }

        // Extract result
        return write_result(job);
    }

    [[nodiscard]] static inline dml_status_t check(job_view job) noexcept
    {
        bool finished = false;

        auto task_view = detail::ml::make_view(job.state().task);

        switch (job.state().path)
        {
            case DML_PATH_SW:
                finished = detail::ml::finished<detail::ml::execution_path::software>(task_view);
                break;
            case DML_PATH_HW:
                finished = detail::ml::finished<detail::ml::execution_path::hardware>(task_view);
                break;
            case DML_PATH_AUTO:
                finished = detail::ml::finished<detail::ml::execution_path::automatic>(task_view);
                break;
        }

        if (finished)
        {
            // Extract result
            return write_result(job);
        }
        else
        {
            return DML_STATUS_BEING_PROCESSED;
        }
    }

    [[nodiscard]] static inline dml_status_t submit(job_view job) noexcept
    {
        if (auto status = range_check(job); status != DML_STATUS_OK)
        {
            return status;
        }
        if(job.operation() == DML_OP_MEM_MOVE && job.flags() & DML_FLAG_COPY_ONLY){
            job.set_flags( job.flags() & ~DML_FLAG_COPY_ONLY);
        }

        job.state().task = make_task(job);

        auto validation_status = detail::validation_status::error;
        auto submission_status = detail::submission_status::failure;

        switch (job.state().path)
        {
            case DML_PATH_SW:
                std::tie(validation_status, submission_status) =
                    detail::ml::submit<detail::ml::execution_path::software>(make_view(job.state().task), job.numa_id());
                break;
            case DML_PATH_HW:
                std::tie(validation_status, submission_status) =
                    detail::ml::submit<detail::ml::execution_path::hardware>(make_view(job.state().task), job.numa_id());
                break;
            case DML_PATH_AUTO:
                std::tie(validation_status, submission_status) =
                    detail::ml::submit<detail::ml::execution_path::automatic>(make_view(job.state().task), job.numa_id());
                break;
        }

        if (validation_status != detail::validation_status::success)
        {
            return to_own_status(validation_status);
        }

        if (submission_status != detail::submission_status::success)
        {
            return to_own_status(submission_status);
        }

        return DML_STATUS_OK;
    }

    [[nodiscard]] static inline dml_status_t execute(job_view job, bool umwait) noexcept
    {
        if (auto status = submit(job); status != DML_STATUS_OK)
        {
            return status;
        }

        // Busy polling
        return wait(job, umwait);
    }

}  // namespace dml

#endif  //DML_IMPL_HPP
