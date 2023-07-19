/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains bad argument tests for DML_OP_BATCH operation
 * @date 7/18/2023
 *
 */
#include "t_common.hpp"

namespace dml
{
    /**
     * @brief Tests the operation using the reserved block on fault flag
     */
    DML_JOB_API_TEST_GENERATOR(dml_batch, tb_bof)
    {
        if(test::variables_t::path == DML_PATH_SW){
            GTEST_SKIP();
        }

        constexpr auto operation_count = 4u;
        auto           batch_size      = 0u;

        auto status = dml_get_batch_size(&*lib_job, operation_count, &batch_size);
        EXPECT_EQ(DML_STATUS_OK, status);

        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(batch_size);
        lib_job->destination_length    = batch_size;
        lib_job->operation             = DML_OP_BATCH;
        lib_job->flags                 = DML_FLAG_BLOCK_ON_FAULT;

        for(int i = 0; i < operation_count; i++){
            status = dml_batch_set_nop_by_index(&*lib_job, i, 0u);
            EXPECT_EQ(DML_STATUS_OK, status);
        }

        // Reference job initialization
        ref_job.status = DML_STATUS_JOB_FLAGS_ERROR;

    }

    // Test registers
    DML_JOB_API_TEST_REGISTER(dml_batch, tb_bof);

}  // namespace dml
