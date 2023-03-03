/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_MEM_MOVE_ACTUAL_HPP
#define DML_TESTING_MEM_MOVE_ACTUAL_HPP

#include <cstring>
#include <utils/test.hpp>
#include <utils/compare_pattern/result_builder.hpp>
#include <utils/compare_pattern/workload.hpp>

namespace dml::testing
{
    inline auto ActualImplementation(Workload<ComparePatternOperation>& workload) noexcept
    {
#if defined(C_API)
        auto job = Job();

        job->operation        = DML_OP_COMPARE_PATTERN;
        job->source_first_ptr = workload.get_src().data();
        job->source_length    = workload.get_src().size();
        job->expected_result  = workload.get_expected_result();

        for (auto i = 0u; i < sizeof(std::uint64_t); ++i)
        {
            job->pattern[i] = reinterpret_cast<std::uint8_t*>(&workload.get_pattern())[i];
        }

        if (workload.check_result_enabled())
        {
            job->flags |= DML_FLAG_CHECK_RESULT;
        }

        auto status   = Status(dml_execute_job(job, DML_WAIT_MODE_BUSY_POLL));
        auto result   = job->result;
        auto mismatch = job->offset;
#elif defined(CPP_API)
        auto op = dml::compare_pattern;

        if (workload.check_result_enabled() && workload.get_expected_result() == 0)
        {
            op = op.expect_equal();
        }

        if (workload.check_result_enabled() && workload.get_expected_result() == 1)
        {
            op = op.expect_not_equal();
        }

        auto op_result =
            dml::execute<execution_path>(op,
                                         workload.get_pattern(),
                                         dml::make_view(workload.get_src()));

        auto status   = Status(op_result.status);
        auto result   = static_cast<uint8_t>(op_result.result);
        auto mismatch = op_result.mismatch;
#endif

        return ResultBuilder<ComparePatternOperation>()
            .set_status(status)
            .set_result(result)
            .set_mismatch(mismatch)
            .build();
    }
}  // namespace dml::testing

#endif  //DML_TESTING_MEM_MOVE_ACTUAL_HPP
