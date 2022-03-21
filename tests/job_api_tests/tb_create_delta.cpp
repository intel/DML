/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains bad argument tests for DML_OP_DELTA_CREATE operation
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
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, tb_source_first_null)
    {
        // Variables
        constexpr auto length   = DELTA_DATA_FIELD_SIZE;
        constexpr auto max_size = DELTA_NOTE_BYTE_SIZE;


        // Library job initialization
        lib_job->source_second_ptr     = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;


        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }


    /**
     * @brief Tests the operation with null source second pointer
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, tb_source_second_null)
    {
        // Variables
        constexpr auto length   = DELTA_DATA_FIELD_SIZE;
        constexpr auto max_size = DELTA_NOTE_BYTE_SIZE;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;


        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }


    /**
     * @brief Tests the operation with null destination first pointer
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, tb_destination_first_null)
    {
        // Variables
        constexpr auto length   = DELTA_DATA_FIELD_SIZE;
        constexpr auto max_size = DELTA_NOTE_BYTE_SIZE;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_second_ptr     = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;


        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }


    /**
     * @brief Tests the operation with zero length destination first
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, tb_destination_zero_length)
    {
        // Variables
        constexpr auto regions_count     = 10u;
        constexpr auto length            = (regions_count * DELTA_DATA_FIELD_SIZE);
        constexpr auto max_size          = 0u;
        constexpr auto mismatch_region   = 5u;
        constexpr auto mismatch_position = (mismatch_region * DELTA_DATA_FIELD_SIZE + 2u);


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;


        // Vectors initialization
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr,
                                        lib_job->source_first_ptr + lib_job->source_length);
        lib_job->source_second_ptr[mismatch_position] =
            ~lib_job->source_second_ptr[mismatch_position];


        // Reference job initialization
        ref_job.status = DML_STATUS_JOB_LENGTH_ERROR;
    }


    /**
     * @brief Tests the operation with not aligned source first vector
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, tb_source_first_not_aligned)
    {
        // Variables
        const auto regions_count = 100u;
        const auto length        = regions_count * DELTA_DATA_FIELD_SIZE;
        const auto max_size      = regions_count * DELTA_NOTE_BYTE_SIZE;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length + 1u) + 1u;
        lib_job->source_second_ptr     = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;


        // Reference job initialization
        ref_job.status = DML_STATUS_DELTA_ALIGN_ERROR;
    }


    /**
     * @brief Tests the operation with not aligned source second vector
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, tb_source_second_not_aligned)
    {
        // Variables
        const auto regions_count = 100u;
        const auto length        = regions_count * DELTA_DATA_FIELD_SIZE;
        const auto max_size      = regions_count * DELTA_NOTE_BYTE_SIZE;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_second_ptr     = dml::test::global_allocator::allocate_ptr(length + 1u) + 1u;
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;


        // Reference job initialization
        ref_job.status = DML_STATUS_DELTA_ALIGN_ERROR;
    }


    /**
     * @brief Tests the operation with not aligned source length
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, tb_source_length_not_aligned)
    {
        // Variables
        const auto regions_count = 100u;
        const auto length        = regions_count * DELTA_DATA_FIELD_SIZE + 1u;
        const auto max_size      = regions_count * DELTA_NOTE_BYTE_SIZE;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_second_ptr     = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;


        // Reference job initialization
        ref_job.status = DML_STATUS_DELTA_INPUT_SIZE_ERROR;
    }


    /**
     * @brief Tests the operation with source length overflow
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, tb_source_length_overflow)
    {
        // Variables
        const auto regions_count = 0x80000u + 8u;
        const auto length        = regions_count * DELTA_DATA_FIELD_SIZE;
        const auto max_size      = regions_count * DELTA_NOTE_BYTE_SIZE;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_second_ptr     = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;


        // Reference job initialization
        ref_job.status = DML_STATUS_DELTA_INPUT_SIZE_ERROR;
    }


    /**
     * @brief Tests the operation with bad destination size
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, tb_bad_destination_size)
    {
        // Variables
        const auto regions_count = 100u;
        const auto length        = regions_count * DELTA_DATA_FIELD_SIZE;
        const auto max_size      = regions_count * DELTA_NOTE_BYTE_SIZE + 1u;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_second_ptr     = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;


        // Reference job initialization
        ref_job.status = DML_STATUS_DELTA_RECORD_SIZE_ERROR;
    }



    // Tests registration
    DML_JOB_API_TEST_REGISTER(dml_delta_create, tb_source_first_null);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, tb_source_second_null);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, tb_destination_first_null);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, tb_destination_zero_length);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, tb_source_first_not_aligned);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, tb_source_second_not_aligned);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, tb_source_length_not_aligned);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, tb_source_length_overflow);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, tb_bad_destination_size);

}
