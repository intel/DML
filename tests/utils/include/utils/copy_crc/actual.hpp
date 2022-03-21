/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_COPY_CRC_ACTUAL_HPP
#define DML_TESTING_COPY_CRC_ACTUAL_HPP

#include <utils/test.hpp>
#include <utils/copy_crc/result_builder.hpp>
#include <utils/copy_crc/workload.hpp>

namespace dml::testing
{
    inline auto ActualImplementation(Workload<CopyCrcOperation>& workload) noexcept
    {
#if defined(C_API)
        auto job = Job();

        auto crc_seed = workload.get_crc_seed();

        job->operation             = DML_OP_COPY_CRC;
        job->source_first_ptr      = workload.get_src().data();
        job->destination_first_ptr = workload.get_dst().data();
        job->source_length         = workload.get_src().size();
        job->destination_length    = workload.get_dst().size();
        job->crc_checksum_ptr      = &crc_seed;

        job->flags |= DML_FLAG_CRC_READ_SEED;

        if (workload.get_bypass_reflection())
        {
            job->flags |= DML_FLAG_CRC_BYPASS_REFLECTION;
        }

        if (workload.get_bypass_data_reflection())
        {
            job->flags |= DML_FLAG_CRC_BYPASS_DATA_REFLECTION;
        }

        auto status = Status(dml_execute_job(job));

        auto crc_value = *job->crc_checksum_ptr;
#elif defined(CPP_API)
        auto op = dml::copy_crc;

        if (workload.get_bypass_reflection())
        {
            op = op.bypass_reflection();
        }

        if (workload.get_bypass_data_reflection())
        {
            op = op.bypass_data_reflection();
        }

        auto result = dml::execute<execution_path>(op,
                                                   dml::make_view(workload.get_src()),
                                                   dml::make_view(workload.get_dst()),
                                                   workload.get_crc_seed());

        auto status    = Status(result.status);
        auto crc_value = result.crc_value;
#endif

        return ResultBuilder<CopyCrcOperation>()
            .set_status(status)
            .set_crc_value(crc_value)
            .build();
    }
}  // namespace dml::testing

#endif  //DML_TESTING_COPY_CRC_ACTUAL_HPP
