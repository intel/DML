/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains bad argument tests for DML_OP_DIF_INSERT operation
 * @date 5/18/2020
 *
 */
#include "t_common.hpp"


namespace dml
{

    /**
     * @brief Tests the operation with null source first pointer
     */
    DML_JOB_API_TEST_GENERATOR(dml_dif_insert, tb_source_first_null)
    {
        // Variables
        constexpr auto length = 100u;


        // Library job initialization
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length    = length;
        lib_job->operation             = DML_OP_DIF_INSERT;

        lib_job->dif_config.block_size = DML_DIF_BLOCK_SIZE_512;


        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }


    /**
     * @brief Tests the operation with null destination first pointer
     */
    DML_JOB_API_TEST_GENERATOR(dml_dif_insert, tb_destination_first_null)
    {
        // Variables
        constexpr auto length = 100u;


        // Library job initialization
        lib_job->source_first_ptr   = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length      = length;
        lib_job->destination_length = length;
        lib_job->operation          = DML_OP_DIF_INSERT;

        lib_job->dif_config.block_size = DML_DIF_BLOCK_SIZE_512;


        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }


    /**
     * @brief Tests the operation with bad source first vector size
     */
    DML_JOB_API_TEST_GENERATOR(dml_dif_insert, tb_bad_source_first_size)
    {
        // Variables
        constexpr auto block_count        = 100u;
        constexpr auto source_length      = (512u * block_count) + 1u;
        constexpr auto destination_length = ((512u + 8u) * block_count);


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(source_length);
        lib_job->source_length         = source_length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(destination_length);
        lib_job->destination_length    = destination_length;
        lib_job->operation             = DML_OP_DIF_INSERT;

        lib_job->dif_config.block_size = DML_DIF_BLOCK_SIZE_512;


        // Reference job initialization
        ref_job.status = DML_STATUS_JOB_LENGTH_ERROR;
    }


    /**
     * @brief Tests the operation with source overlap destination
     */
    DML_JOB_API_TEST_GENERATOR(dml_dif_insert, tb_source_overlap_destination)
    {
        // Variables
        constexpr auto block_count        = 100u;
        constexpr auto source_length      = (512u * block_count);
        constexpr auto destination_length = ((512u + 8u) * block_count);
        const auto memory_buffer          = dml::test::global_allocator::allocate_ptr(source_length + destination_length);


        // Library job initialization
        lib_job->destination_first_ptr = memory_buffer;
        lib_job->destination_length    = destination_length;
        lib_job->source_first_ptr      = (memory_buffer + destination_length / 2u);
        lib_job->source_length         = source_length;
        lib_job->operation             = DML_OP_DIF_INSERT;

        lib_job->dif_config.block_size = DML_DIF_BLOCK_SIZE_512;


        // Reference job initialization
        ref_job.status = DML_STATUS_OVERLAPPING_BUFFER_ERROR;
    }


    /**
     * @brief Tests the operation with destination overlap source
     */
    DML_JOB_API_TEST_GENERATOR(dml_dif_insert, tb_destination_overlap_source)
    {
        // Variables
        constexpr auto block_count        = 100u;
        constexpr auto source_length      = (512u * block_count);
        constexpr auto destination_length = ((512u + 8u) * block_count);
        const auto memory_buffer          = dml::test::global_allocator::allocate_ptr(source_length + destination_length);


        // Library job initialization
        lib_job->source_first_ptr      = memory_buffer;
        lib_job->source_length         = source_length;
        lib_job->destination_first_ptr = (memory_buffer + source_length / 2u);
        lib_job->destination_length    = destination_length;
        lib_job->operation             = DML_OP_DIF_INSERT;

        lib_job->dif_config.block_size = DML_DIF_BLOCK_SIZE_512;


        // Reference job initialization
        ref_job.status = DML_STATUS_OVERLAPPING_BUFFER_ERROR;
    }


    // Tests registration
    DML_JOB_API_TEST_REGISTER(dml_dif_insert, tb_source_first_null);
    DML_JOB_API_TEST_REGISTER(dml_dif_insert, tb_destination_first_null);
    DML_JOB_API_TEST_REGISTER(dml_dif_insert, tb_bad_source_first_size);
    DML_JOB_API_TEST_REGISTER(dml_dif_insert, tb_source_overlap_destination);
    DML_JOB_API_TEST_REGISTER(dml_dif_insert, tb_destination_overlap_source);

}