/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_MEM_MOVE_ACTUAL_HPP
#define DML_TESTING_MEM_MOVE_ACTUAL_HPP

#include <utils/test.hpp>
#include <utils/mem_move/result_builder.hpp>
#include <utils/mem_move/workload.hpp>

namespace dml::testing
{
    inline auto ActualImplementation(Workload<MemMoveOperation>& workload) noexcept
    {
#if defined(C_API)
        auto job = Job();

        job->operation             = DML_OP_MEM_MOVE;
        job->source_first_ptr      = workload.get_src().data();
        job->destination_first_ptr = workload.get_dst().data();
        job->source_length         = workload.get_src().size();
        job->destination_length    = workload.get_dst().size();

        auto status = Status(dml_execute_job(job));
#elif defined(CPP_API)
        auto result = dml::execute<execution_path>(dml::mem_move,
                                                   dml::make_view(workload.get_src()),
                                                   dml::make_view(workload.get_dst()));

        auto status = Status(result.status);
#endif

        return ResultBuilder<MemMoveOperation>().set_status(status).build();
    }
}  // namespace dml::testing

#endif  //DML_TESTING_MEM_MOVE_ACTUAL_HPP
