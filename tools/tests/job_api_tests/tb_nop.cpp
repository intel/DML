/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains bad argument tests for DML_OP_NOP operation
 * @date 7/18/2023
 *
 */
#include "t_common.hpp"


namespace dml
{

    /**
     * @brief Tests the operation using the reserved block on fault flag
     */
    DML_JOB_API_TEST_GENERATOR(dml_nop, tb_bof)
    {
        if(test::variables_t::path == DML_PATH_SW){
            GTEST_SKIP();
        }

        // Library job initialization
        lib_job->operation = DML_OP_NOP;
        lib_job->flags = DML_FLAG_BLOCK_ON_FAULT;

        // Reference job initialization
        ref_job.status = DML_STATUS_JOB_FLAGS_ERROR;
    }


    // Test registers
    DML_JOB_API_TEST_REGISTER(dml_nop, tb_bof)

}
