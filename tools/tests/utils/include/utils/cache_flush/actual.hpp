/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_CACHE_FLUSH_ACTUAL_HPP
#define DML_TESTING_CACHE_FLUSH_ACTUAL_HPP

#include <utils/test.hpp>
#include <utils/cache_flush/result_builder.hpp>
#include <utils/cache_flush/workload.hpp>

namespace dml::testing
{
    inline auto ActualImplementation(Workload<CacheFlushOperation>& workload) noexcept
    {
#if defined(C_API)
        auto job = Job();

        job->operation             = DML_OP_CACHE_FLUSH;
        job->destination_first_ptr = workload.get_dst().data();
        job->destination_length    = workload.get_dst().size();
        job->flags |= (workload.block_on_fault_enabled()?DML_FLAG_BLOCK_ON_FAULT:0x00);

        if (workload.cache_control_enabled())
        {
            job->flags |= DML_FLAG_DONT_INVALIDATE_CACHE;
        }

        auto status = Status(dml_execute_job(job, DML_WAIT_MODE_BUSY_POLL));
#elif defined(CPP_API)
        auto op = dml::cache_flush;

        if (workload.cache_control_enabled())
        {
            op = op.dont_invalidate_cache();
        }

        if (workload.block_on_fault_enabled())
        {
            op = op.block_on_fault();
        }

        auto result = dml::execute<execution_path>(op,
                                                   dml::make_view(workload.get_dst()));

        auto status = Status(result.status);
#endif

        return ResultBuilder<CacheFlushOperation>().set_status(status).build();
    }
}  // namespace dml::testing

#endif  //DML_TESTING_CACHE_FLUSH_ACTUAL_HPP
