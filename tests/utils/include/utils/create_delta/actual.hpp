/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_CREATE_DELTA_ACTUAL_HPP
#define DML_TESTING_CREATE_DELTA_ACTUAL_HPP

#include <utils/test.hpp>
#include <utils/create_delta/result_builder.hpp>
#include <utils/create_delta/workload.hpp>

namespace dml::testing
{
    inline auto ActualImplementation(Workload<CreateDeltaOperation>& workload) noexcept
    {
#if defined(C_API)
        auto job = Job();

        job->operation             = DML_OP_DELTA_CREATE;
        job->source_first_ptr      = workload.get_src1().data();
        job->source_second_ptr     = workload.get_src2().data();
        job->destination_first_ptr = workload.get_delta().data();
        job->source_length         = workload.get_src1().size();
        job->destination_length    = workload.get_delta().size();
        job->expected_result       = workload.get_expected_result();

        if (workload.check_result_enabled())
        {
            job->flags |= DML_FLAG_CHECK_RESULT;
        }

        auto status       = Status(dml_execute_job(job));
        auto result       = job->result;
        auto written_size = job->destination_length;
#elif defined(CPP_API)
        auto op = dml::create_delta;

        if (workload.check_result_enabled())
        {
            if (workload.get_expected_result() == 1)
            {
                op = op.expect_equal();
            }
            else if (workload.get_expected_result() == 2)
            {
                op = op.expect_not_equal();
            }
        }
        auto op_result =
            dml::execute<execution_path>(op,
                                         dml::make_view(workload.get_src1()),
                                         dml::make_view(workload.get_src2()),
                                         dml::make_view(workload.get_delta()));

        auto status       = Status(op_result.status);
        auto result       = static_cast<uint8_t>(op_result.result);
        auto written_size = op_result.delta_record_size;
#endif

        return ResultBuilder<CreateDeltaOperation>()
            .set_status(status)
            .set_result(result)
            .set_written_size(written_size)
            .build();
    }
}  // namespace dml::testing

#endif  //DML_TESTING_CREATE_DELTA_ACTUAL_HPP
