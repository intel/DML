/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_APPLY_DELTA_ACTUAL_HPP
#define DML_TESTING_APPLY_DELTA_ACTUAL_HPP

#include <utils/test.hpp>
#include <utils/apply_delta/result_builder.hpp>
#include <utils/apply_delta/workload.hpp>

namespace dml::testing
{
    inline auto ActualImplementation(Workload<ApplyDeltaOperation>& workload) noexcept
    {
#if defined(C_API)
        auto job = Job();

        job->operation             = DML_OP_DELTA_APPLY;
        job->source_first_ptr      = workload.get_delta().data();
        job->destination_first_ptr = workload.get_dst().data();
        job->source_length         = workload.get_delta().size();
        job->destination_length    = workload.get_dst().size();

        auto status = Status(dml_execute_job(job, DML_WAIT_MODE_BUSY_POLL));
#elif defined(CPP_API)
        auto create_result              = dml::create_delta_result{};
        create_result.delta_record_size = workload.get_delta().size();
        create_result.result            = comparison_result::not_equal;

        auto result = dml::execute<execution_path>(dml::apply_delta,
                                                   dml::make_view(workload.get_delta()),
                                                   dml::make_view(workload.get_dst()),
                                                   create_result);

        auto status = Status(result.status);
#endif

        return ResultBuilder<ApplyDeltaOperation>().set_status(status).build();
    }
}  // namespace dml::testing

#endif  //DML_TESTING_APPLY_DELTA_ACTUAL_HPP
