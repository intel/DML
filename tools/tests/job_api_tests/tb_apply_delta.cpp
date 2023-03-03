/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains bad argument tests for DML_OP_DELTA_APPLY operation
 * @date 4/17/2020
 *
 */
#include "t_common.hpp"
#include "t_delta_record_feature_defines.hpp"


namespace dml
{

    /**
     * @brief Tests the operation with null source first pointer
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_apply, tb_source_first_null)
    {
        // Variables
        constexpr auto length   = DELTA_DATA_FIELD_SIZE;
        constexpr auto max_size = DELTA_NOTE_BYTE_SIZE;


        // Library job initialization
        lib_job->source_length         = max_size;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = length;
        lib_job->operation             = DML_OP_DELTA_APPLY;


        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }


    /**
     * @brief Tests the operation with null destination first pointer
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_apply, tb_destination_first_null)
    {
        // Variables
        constexpr auto length   = DELTA_DATA_FIELD_SIZE;
        constexpr auto max_size = DELTA_NOTE_BYTE_SIZE;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = max_size;
        lib_job->destination_length    = length;
        lib_job->operation             = DML_OP_DELTA_APPLY;


        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }


    /**
     * @brief Tests the operation with source first overlap destination first
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_apply, tb_source_overlap_destination)
    {
        // Variables
        constexpr auto length      = 10u * DELTA_DATA_FIELD_SIZE;
        constexpr auto max_size    = 10u * DELTA_NOTE_BYTE_SIZE;
        constexpr auto buffer_size = 2u * (length + max_size);
        auto memory_buffer         = dml::test::global_allocator::allocate_ptr(buffer_size);


        // Library job initialization
        lib_job->destination_first_ptr = &memory_buffer[0];
        lib_job->destination_length    = length;
        lib_job->source_first_ptr      = &memory_buffer[0] + max_size / 2u;
        lib_job->source_length         = max_size;
        lib_job->operation             = DML_OP_DELTA_APPLY;


        // Reference job initialization
        ref_job.status = DML_STATUS_OVERLAPPING_BUFFER_ERROR;
    }


    /**
     * @brief Tests the operation with destination first overlap source first
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_apply, tb_destination_overlap_source)
    {
        // Variables
        constexpr auto length      = 10u * DELTA_DATA_FIELD_SIZE;
        constexpr auto max_size    = 10u * DELTA_NOTE_BYTE_SIZE;
        constexpr auto buffer_size = 2u * (length + max_size);
        auto memory_buffer         = dml::test::global_allocator::allocate_ptr(buffer_size);


        // Library job initialization
        lib_job->source_first_ptr      = &memory_buffer[0];
        lib_job->source_length         = max_size;
        lib_job->destination_first_ptr = &memory_buffer[0] + length / 2u;
        lib_job->destination_length    = length;
        lib_job->operation             = DML_OP_DELTA_APPLY;


        // Reference job initialization
        ref_job.status = DML_STATUS_OVERLAPPING_BUFFER_ERROR;
    }


    /**
     * @brief Tests the operation with not aligned destination length
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_apply, tb_destination_length_not_aligned)
    {
        // Variables
        constexpr auto length   = DELTA_DATA_FIELD_SIZE + 1u;
        constexpr auto max_size = DELTA_NOTE_BYTE_SIZE;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->source_length         = max_size;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length    = length;
        lib_job->operation             = DML_OP_DELTA_APPLY;


        // Reference job initialization
        ref_job.status = DML_STATUS_DELTA_INPUT_SIZE_ERROR;
    }


    /**
     * @brief Tests the operation with not aligned destination ptr
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_apply, tb_destination_ptr_not_aligned)
    {
        // Variables
        constexpr auto length   = DELTA_DATA_FIELD_SIZE;
        constexpr auto max_size = DELTA_NOTE_BYTE_SIZE;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->source_length         = max_size;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(length + 1u) + 1u;
        lib_job->destination_length    = length;
        lib_job->operation             = DML_OP_DELTA_APPLY;


        // Reference job initialization
        ref_job.status = DML_STATUS_DELTA_ALIGN_ERROR;
    }


    /**
     * @brief Tests the operation with source length overflow
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_apply, tb_source_length_overflow)
    {
        // Variables
        constexpr auto length   = 0x80000 + 8u;
        constexpr auto max_size = DELTA_NOTE_BYTE_SIZE;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->source_length         = max_size;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length    = length;
        lib_job->operation             = DML_OP_DELTA_APPLY;


        // Reference job initialization
        ref_job.status = DML_STATUS_DELTA_INPUT_SIZE_ERROR;
    }



    // Tests register
    DML_JOB_API_TEST_REGISTER(dml_delta_apply, tb_source_first_null);
    DML_JOB_API_TEST_REGISTER(dml_delta_apply, tb_destination_first_null);
    DML_JOB_API_TEST_REGISTER(dml_delta_apply, tb_source_overlap_destination);
    DML_JOB_API_TEST_REGISTER(dml_delta_apply, tb_destination_overlap_source);
    DML_JOB_API_TEST_REGISTER(dml_delta_apply, tb_destination_length_not_aligned);
    DML_JOB_API_TEST_REGISTER(dml_delta_apply, tb_destination_ptr_not_aligned);
    DML_JOB_API_TEST_REGISTER(dml_delta_apply, tb_source_length_overflow);

}
