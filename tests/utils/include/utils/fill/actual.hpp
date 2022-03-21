/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_FILL_ACTUAL_HPP
#define DML_TESTING_FILL_ACTUAL_HPP

#include <cstring>
#include <utils/test.hpp>
#include <utils/fill/result_builder.hpp>
#include <utils/fill/workload.hpp>

namespace dml::testing
{
    inline auto ActualImplementation(Workload<FillOperation>& workload) noexcept
    {
#if defined(C_API)
        auto job = Job();

        job->operation             = DML_OP_FILL;
        job->destination_first_ptr = workload.get_dst().data();
        job->destination_length    = workload.get_dst().size();

        for (auto i = 0u; i < sizeof(std::uint64_t); ++i)
        {
            job->pattern[i] = reinterpret_cast<std::uint8_t*>(&workload.get_pattern())[i];
        }

        auto status = Status(dml_execute_job(job));
#elif defined(CPP_API)
        auto op_result =
            dml::execute<execution_path>(dml::fill,
                                         workload.get_pattern(),
                                         dml::make_view(workload.get_dst()));

        auto status = Status(op_result.status);
#endif

        return ResultBuilder<FillOperation>().set_status(status).build();
    }
}  // namespace dml::testing

#endif  //DML_TESTING_FILL_ACTUAL_HPP
