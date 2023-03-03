/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains algorithmic tests for DML_OP_NOP operation
 * @date 5/20/2020
 *
 */
#include "t_common.hpp"


namespace dml
{

    /**
     * @brief Simple @ref DML_OP_NOP launcher
     */
    DML_JOB_API_TEST_GENERATOR(dml_nop, ta_test)
    {
        // Library job initialization
        lib_job->operation = DML_OP_NOP;

        // Reference job initialization
        ref_job.status = DML_STATUS_OK;
    }


    // Test registers
    DML_JOB_API_TEST_REGISTER(dml_nop, ta_test)

}
