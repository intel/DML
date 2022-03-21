/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains bad argument tests for the @ref dml_submit_job
 * @date 5/18/2020
 *
 */
#include "t_common.hpp"


namespace dml
{

    /**
     * @brief Tests the function with incorrect operation
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_execute_job, tb_source_first_null)
    {
        auto job_size = 0u;

        const auto get_job_size_status  = dml_get_job_size(dml::test::variables_t::path, &job_size);
        EXPECT_EQ(DML_STATUS_OK, get_job_size_status);

        auto dml_job_ptr = reinterpret_cast<dml_job_t *>(dml::test::global_allocator::allocate_ptr(job_size));
        EXPECT_NE(nullptr, dml_job_ptr);

        const auto init_job_status = dml_init_job(dml::test::variables_t::path, dml_job_ptr);
        EXPECT_EQ(DML_STATUS_OK, init_job_status);

        dml_job_ptr->operation = static_cast<dml_operation_t>(~0ll);

        const auto execute_job_status = dml_execute_job(dml_job_ptr);
        EXPECT_EQ(DML_STATUS_JOB_OPERATION_ERROR, execute_job_status);
    }


    // Test registers
    DML_UNIT_TEST_REGISTER(unit_dml_execute_job, tb_source_first_null);

}
