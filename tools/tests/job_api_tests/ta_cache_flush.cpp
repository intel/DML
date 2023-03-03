/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains algorithmic tests for DML_OP_CACHE_FLUSH operation
 * @date 4/17/2020
 *
 */
#include "t_common.hpp"


namespace dml
{

    /**
     * @brief Tests the operation with DML_FLAG_DONT_INVALIDATE_CACHE flag
     */
    DML_JOB_API_TEST_GENERATOR(dml_cache_flush, ta_do_not_invalidate)
    {
        // Constants
        const auto length = 8u * 1000u;

        // Library job initialization
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length    = length;
        lib_job->operation             = DML_OP_CACHE_FLUSH;
        lib_job->flags                 = DML_FLAG_DONT_INVALIDATE_CACHE;

        // Reference job initialization
        ref_job.status            = DML_STATUS_OK;
        ref_job.destination_first = dml::test::global_allocator::allocate(length);
    }


    /**
     * @brief Tests the operation without DML_FLAG_DONT_INVALIDATE_CACHE flag
     */
    DML_JOB_API_TEST_GENERATOR(dml_cache_flush, ta_invalidate)
    {
        // Constants
        const auto length = 8u * 1000u;

        // Library job initialization
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length    = length;
        lib_job->operation             = DML_OP_CACHE_FLUSH;

        // Reference job initialization
        ref_job.status            = DML_STATUS_OK;
        ref_job.destination_first = dml::test::global_allocator::allocate(length);
    }


    // Test registers
    DML_JOB_API_TEST_REGISTER(dml_cache_flush, ta_do_not_invalidate);
    DML_JOB_API_TEST_REGISTER(dml_cache_flush, ta_invalidate);

}
