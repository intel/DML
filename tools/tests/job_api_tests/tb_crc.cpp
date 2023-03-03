/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains bad argument tests for DML_OP_CRC operation
 * @date 5/7/2020
 *
 */
#include "t_common.hpp"


namespace dml
{

    /**
     * @brief Tests the operation with null source first pointer
     */
    DML_JOB_API_TEST_GENERATOR(dml_crc, tb_source_first_null)
    {
        // Variables
        constexpr auto length = 1u;


        // Library job initialization
        lib_job->source_length    = length;
        lib_job->crc_checksum_ptr =
            reinterpret_cast<uint32_t *>(dml::test::global_allocator::allocate_ptr(sizeof(uint32_t)));
        lib_job->operation        = DML_OP_CRC;


        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }


    /**
     * @brief Tests the operation with null crc checksum pointer
     */
    DML_JOB_API_TEST_GENERATOR(dml_crc, tb_crc_checksum_null)
    {
        // Variables
        constexpr auto length = 1u;


        // Library job initialization
        lib_job->source_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length    = length;
        lib_job->operation        = DML_OP_CRC;


        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }

    // Test registers
    DML_JOB_API_TEST_REGISTER(dml_crc, tb_source_first_null);
    DML_JOB_API_TEST_REGISTER(dml_crc, tb_crc_checksum_null);

}
