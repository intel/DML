/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains bad argument tests for DML_OP_DIF_CHECK operation
 * @date 5/21/2020
 *
 */
#include "t_common.hpp"


namespace dml
{

    /**
     * @brief Tests the operation with null source vector
     */
    DML_JOB_API_TEST_GENERATOR(dml_dif_check, tb_source_first_null)
    {
        // Variables
        constexpr auto length = 100u;


        // Library job initialization
        lib_job->source_length = length;
        lib_job->operation     = DML_OP_DIF_CHECK;

        lib_job->dif_config.block_size = DML_DIF_BLOCK_SIZE_512;


        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }


    /**
     * @brief Tests the operation with bad source first vector size
     */
    DML_JOB_API_TEST_GENERATOR(dml_dif_check, tb_bad_source_first_size)
    {
        // Variables
        constexpr auto block_count   = 100u;
        constexpr auto source_length = ((512u + 8u) * block_count) + 1u;


        // Library job initialization
        lib_job->source_first_ptr = dml::test::global_allocator::allocate_ptr(source_length);
        lib_job->source_length    = source_length;
        lib_job->operation        = DML_OP_DIF_CHECK;

        lib_job->dif_config.block_size = DML_DIF_BLOCK_SIZE_512;


        // Reference job initialization
        ref_job.status = DML_STATUS_JOB_LENGTH_ERROR;
    }


    // Tests registration
    DML_JOB_API_TEST_REGISTER(dml_dif_check, tb_source_first_null);
    DML_JOB_API_TEST_REGISTER(dml_dif_check, tb_bad_source_first_size);

}