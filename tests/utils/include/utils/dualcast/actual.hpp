/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_DUALCAST_ACTUAL_HPP
#define DML_TESTING_DUALCAST_ACTUAL_HPP

#include <utils/test.hpp>
#include <utils/dualcast/result_builder.hpp>
#include <utils/dualcast/workload.hpp>

namespace dml::testing
{
    inline auto ActualImplementation(Workload<DualcastOperation>& workload) noexcept
    {
#if defined(C_API)
        auto job = Job();

        job->operation              = DML_OP_DUALCAST;
        job->source_first_ptr       = workload.get_src().data();
        job->destination_first_ptr  = workload.get_dst1().data();
        job->destination_second_ptr = workload.get_dst2().data();
        job->source_length          = workload.get_src().size();
        job->destination_length     = workload.get_dst1().size();

        auto status = Status(dml_execute_job(job, DML_WAIT_MODE_BUSY_POLL));
#elif defined(CPP_API)
        auto result = dml::execute<execution_path>(dml::dualcast,
                                                   dml::make_view(workload.get_src()),
                                                   dml::make_view(workload.get_dst1()),
                                                   dml::make_view(workload.get_dst2()));

        auto status = Status(result.status);
#endif

        return ResultBuilder<DualcastOperation>().set_status(status).build();
    }
}  // namespace dml::testing

#endif  //DML_TESTING_DUALCAST_ACTUAL_HPP
