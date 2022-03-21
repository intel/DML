/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains bad argument tests for DML_OP_FILL operation
 * @date 4/17/2020
 *
 */
#include "t_common.hpp"


namespace dml
{

    /**
     * @brief Tests that destination first null vector leads to bad status
     */
    DML_JOB_API_TEST_GENERATOR(dml_fill, tb_destination_first_null)
    {
        // Variables
        constexpr auto length = 1u;

        
        // Library job initialization
        lib_job->destination_length = length;
        lib_job->operation          = DML_OP_FILL;


        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }


    // Tests registration
    DML_JOB_API_TEST_REGISTER(dml_fill, tb_destination_first_null);

}
