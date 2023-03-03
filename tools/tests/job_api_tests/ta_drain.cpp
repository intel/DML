/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains algorithmic tests for DML_OP_DRAIN operation
 * @date 4/17/2020
 *
 */
#include "t_common.hpp"
#include "t_random_generator.hpp"
#include "t_random_parameters.hpp"


namespace dml
{

    /**
     * @brief Tests the operation with default parameters
     */
    DML_JOB_API_TEST_GENERATOR(dml_drain, ta_default_parameters)
    {
        lib_job->operation = DML_OP_DRAIN;

        ref_job.status = DML_STATUS_OK;
    }


    // Test registers
    DML_JOB_API_TEST_REGISTER(dml_drain, ta_default_parameters);

}
