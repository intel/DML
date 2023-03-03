/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains bad argument tests for DML_OP_DIF_UPDATE operation
 * @date 5/20/2020
 *
 */
#include "t_common.hpp"


namespace dml
{

    /**
     * @brief Tests the operation with null source vector
     */
    DML_JOB_API_TEST_GENERATOR(dml_dif_update, tb_source_first_null)
    {
        // Variables
        constexpr auto length = 100u;


        // Library job initialization
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length    = length;
        lib_job->operation             = DML_OP_DIF_UPDATE;

        lib_job->dif_config.block_size = DML_DIF_BLOCK_SIZE_512;


        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }


    /**
     * @brief Tests the operation with null destination vector
     */
    DML_JOB_API_TEST_GENERATOR(dml_dif_update, tb_destination_first_null)
    {
        // Variables
        constexpr auto length = 100u;


        // Library job initialization
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length    = length;
        lib_job->operation             = DML_OP_DIF_UPDATE;

        lib_job->dif_config.block_size = DML_DIF_BLOCK_SIZE_512;


        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }


    /**
     * @brief Tests the operation with bad vectors length
     */
    DML_JOB_API_TEST_GENERATOR(dml_dif_update, tb_bad_length)
    {
        // Variables
        constexpr auto block_count  = 100u;
        constexpr auto length       = ((512u + 8u) * block_count) + 1u;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length    = length;
        lib_job->operation             = DML_OP_DIF_UPDATE;

        lib_job->dif_config.block_size = DML_DIF_BLOCK_SIZE_512;


        // Reference job initialization
        ref_job.status = DML_STATUS_JOB_LENGTH_ERROR;
    }


    /**
     * @brief Tests the operation with source overlap destination
     */
    DML_JOB_API_TEST_GENERATOR(dml_dif_update, tb_source_overlap_destination)
    {
        // Variables
        constexpr auto block_count  = 100u;
        constexpr auto length       = ((512u + 8u) * block_count);
        const auto memory_buffer    = dml::test::global_allocator::allocate_ptr(length + length);


        // Library job initialization
        lib_job->destination_first_ptr = memory_buffer;
        lib_job->destination_length    = length;
        lib_job->source_first_ptr      = (memory_buffer + length / 2u);
        lib_job->source_length         = length;
        lib_job->operation             = DML_OP_DIF_UPDATE;

        lib_job->dif_config.block_size = DML_DIF_BLOCK_SIZE_512;


        // Reference job initialization
        ref_job.status = DML_STATUS_OVERLAPPING_BUFFER_ERROR;
    }


    /**
     * @brief Tests the operation with destination overlap source
     */
    DML_JOB_API_TEST_GENERATOR(dml_dif_update, tb_destination_overlap_source)
    {
        // Variables
        constexpr auto block_count = 100u;
        constexpr auto length      = ((512u + 8u) * block_count);
        const auto memory_buffer   = dml::test::global_allocator::allocate_ptr(length + length);


        // Library job initialization
        lib_job->source_first_ptr      = memory_buffer;
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = (memory_buffer + length / 2u);
        lib_job->destination_length    = length;
        lib_job->operation             = DML_OP_DIF_UPDATE;

        lib_job->dif_config.block_size = DML_DIF_BLOCK_SIZE_512;


        // Reference job initialization
        ref_job.status = DML_STATUS_OVERLAPPING_BUFFER_ERROR;
    }


    // Tests registration
    DML_JOB_API_TEST_REGISTER(dml_dif_update, tb_source_first_null);
    DML_JOB_API_TEST_REGISTER(dml_dif_update, tb_destination_first_null);
    DML_JOB_API_TEST_REGISTER(dml_dif_update, tb_bad_length);
    DML_JOB_API_TEST_REGISTER(dml_dif_update, tb_source_overlap_destination);
    DML_JOB_API_TEST_REGISTER(dml_dif_update, tb_destination_overlap_source);

}
