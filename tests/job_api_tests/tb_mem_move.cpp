/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains bad argument tests for DML_OP_MEM_MOVE operation
 * @date 4/17/2020
 *
 */
#include "t_common.hpp"


namespace dml
{

    /**
     * @brief Tests that source first null vector leads to bad status
     */
    DML_JOB_API_TEST_GENERATOR(dml_mem_move, tb_source_first_null)
    {
        // Variables
        constexpr auto length = 1u;


        // Library job initialization
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length    = length;
        lib_job->operation             = DML_OP_MEM_MOVE;


        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }


    /**
     * @brief Tests that destination first null vector leads to bad status
     */
    DML_JOB_API_TEST_GENERATOR(dml_mem_move, tb_destination_first_null)
    {
        // Variables
        constexpr auto length = 1u;


        // Library job initialization
        lib_job->source_first_ptr    = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length       = length;
        lib_job->destination_length  = length;
        lib_job->operation           = DML_OP_MEM_MOVE;


        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }


    /**
     * @brief Tests that when destination overlap source that leads to bad status
     */
    DML_JOB_API_TEST_GENERATOR(dml_mem_move, tb_copy_only_destination_overlap_source)
    {
        // Variables
        constexpr auto total_length = 100u;
        constexpr auto length       = (total_length / 2u);


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(total_length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = lib_job->source_first_ptr + (length / 2u);
        lib_job->destination_length    = length;   
        lib_job->flags                 = DML_FLAG_COPY_ONLY;
        lib_job->operation             = DML_OP_MEM_MOVE;


        // Reference job initialization
        ref_job.status = DML_STATUS_OVERLAPPING_BUFFER_ERROR;
    }


    // Tests registration
    DML_JOB_API_TEST_REGISTER(dml_mem_move, tb_source_first_null);
    DML_JOB_API_TEST_REGISTER(dml_mem_move, tb_destination_first_null);
    DML_JOB_API_TEST_REGISTER(dml_mem_move, tb_copy_only_destination_overlap_source);

}
