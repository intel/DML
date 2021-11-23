/*
* Copyright 2021 Intel Corporation.
*
* This software and the related documents are Intel copyrighted materials,
* and your use of them is governed by the express license under which they
* were provided to you ("License"). Unless the License provides otherwise,
* you may not use, modify, copy, publish, distribute, disclose or transmit
* this software or the related documents without Intel's prior written
* permission.
*
* This software and the related documents are provided as is, with no
* express or implied warranties, other than those that are expressly
* stated in the License.
*
*/

#ifndef DML_IMPL_HPP
#define DML_IMPL_HPP

#include <dml/cpp/middle_layer/device.hpp>
#include <dml/cpp/middle_layer/validation.hpp>

#include "job_view.hpp"
#include "range_check.hpp"
#include "write_descriptor.hpp"
#include "write_result.hpp"

namespace dml
{
    inline dml_status_t wait(job_view job) noexcept
    {
        ml::wait(job.state().record);

        // Extract result
        return write_result(job);
    }

    inline dml_status_t check(job_view job) noexcept
    {
        if (ml::is_finished(job.state().record))
        {
            // Extract result
            return write_result(job);
        }
        else
        {
            return DML_STATUS_BEING_PROCESSED;
        }
    }

    inline dml_status_t submit(job_view job) noexcept
    {
        // Job API specific checks
        if (auto status = range_check(job); status != DML_STATUS_OK)
        {
            return status;
        }

        write_descriptor(job);

        // Middle Layer checks
        if (auto status = to_own_status(ml::validate(job.state().dsc)); status != DML_STATUS_OK)
        {
            return status;
        }

        if (job.state().path == DML_PATH_HW)
        {
            return to_own_status(ml::hardware().submit(job.state().dsc, job.state().record));
        }
        else
        {
            return to_own_status(ml::software().submit(job.state().dsc, job.state().record));
        }
    }

    inline dml_status_t execute(job_view job) noexcept
    {
        if (auto status = submit(job); status != DML_STATUS_OK)
        {
            return status;
        }

        return wait(job);
    }

}  // namespace dml

#endif  //DML_IMPL_HPP
