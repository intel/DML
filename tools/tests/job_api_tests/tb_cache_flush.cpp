/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains bad argument tests for DML_OP_CACHE_FLUSH operation
 * @date 4/17/2020
 *
 */
#include "t_common.hpp"


namespace dml
{

    /**
     * @brief Tests the operation with null destination_first_ptr
     */
    DML_JOB_API_TEST_GENERATOR(dml_cache_flush, tb_destiation_first_null)
    {
        // Constants
        const auto length = 8u * 1000u;

        // Library job initialization
        lib_job->destination_length = length;
        lib_job->operation          = DML_OP_CACHE_FLUSH;

        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }


    // Test registers
    DML_JOB_API_TEST_REGISTER(dml_cache_flush, tb_destiation_first_null);

}
