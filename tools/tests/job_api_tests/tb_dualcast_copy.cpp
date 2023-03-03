/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains bad argument tests for DML_OP_DUALCAST operation
 * @date 4/17/2020
 *
 */
#include "t_common.hpp"


namespace dml
{

    /** Specific alignment for the operation **/
    constexpr auto DUALCAST_ALIGNMENT = 0x1000u;
    

    /**
     * @brief Tests that source first null vector leads to bad status
     */
    DML_JOB_API_TEST_GENERATOR(dml_dualcast_copy, tb_source_first_null)
    {
        // Variables
        constexpr auto length = 1u;


        // Library job initialization
        lib_job->source_length          = length;
        lib_job->destination_first_ptr  = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_second_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length     = length;
        lib_job->operation              = DML_OP_DUALCAST;

        
        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }


    /**
     * @brief Tests that destination first null vector leads to bad status
     */
    DML_JOB_API_TEST_GENERATOR(dml_dualcast_copy, tb_destination_first_null)
    {
        // Variables
        constexpr auto length = 1u;


        // Library job initialization
        lib_job->source_first_ptr       = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length          = length;
        lib_job->destination_second_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length     = length;
        lib_job->operation              = DML_OP_DUALCAST;

        
        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }


    /**
     * @brief Tests that destination second null vector leads to bad status
     */
    DML_JOB_API_TEST_GENERATOR(dml_dualcast_copy, tb_destination_second_null)
    {
        // Variables
        constexpr auto length = 1u;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length    = length;
        lib_job->operation             = DML_OP_DUALCAST;

        
        // Reference job initialization
        ref_job.status = DML_STATUS_NULL_POINTER_ERROR;
    }


    /**
     * @brief Tests that if destination first is not aligned it leads to bad status
     */
    DML_JOB_API_TEST_GENERATOR(dml_dualcast_copy, tb_not_aligned_first_destination)
    {
        // Variables
        constexpr auto length        = 100u;
        constexpr auto need_to_align = DUALCAST_ALIGNMENT + (length / DUALCAST_ALIGNMENT) * DUALCAST_ALIGNMENT;
        auto destination_buffer      = dml::test::global_allocator::allocate_ptr(length * 2u + need_to_align);


        // Library job initialization
        lib_job->source_first_ptr       = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length          = length;
        lib_job->destination_first_ptr  = destination_buffer + 1u;
        lib_job->destination_second_ptr = destination_buffer + need_to_align;
        lib_job->destination_length     = length;
        lib_job->operation              = DML_OP_DUALCAST;


        // Reference job initialization
        ref_job.status = DML_STATUS_DUALCAST_ALIGN_ERROR;
    }


    /**
     * @brief Tests that if destination second is not aligned it leads to bad status
     */
    DML_JOB_API_TEST_GENERATOR(dml_dualcast_copy, tb_not_aligned_second_destination)
    {
        // Variables
        constexpr auto length        = 100u;
        constexpr auto need_to_align = DUALCAST_ALIGNMENT + (length / DUALCAST_ALIGNMENT) * DUALCAST_ALIGNMENT;
        auto destination_buffer      = dml::test::global_allocator::allocate_ptr(length * 2u + need_to_align);


        // Library job initialization
        lib_job->source_first_ptr       = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length          = length;
        lib_job->destination_first_ptr  = destination_buffer;
        lib_job->destination_second_ptr = destination_buffer + need_to_align + 1u;
        lib_job->destination_length     = length;
        lib_job->operation              = DML_OP_DUALCAST;


        // Reference job initialization
        ref_job.status = DML_STATUS_DUALCAST_ALIGN_ERROR;
    }


    /**
     * @brief Tests that if destination first overlap source first it leads to bad status
     */
    DML_JOB_API_TEST_GENERATOR(dml_dualcast_copy, tb_destination_first_overlap_source_first)
    {
        // Variables
        constexpr auto length = (2u * DUALCAST_ALIGNMENT);
        auto memory_buffer    = dml::test::global_allocator::allocate_ptr(length * 3u);


        // Library job initialization
        lib_job->source_first_ptr       = memory_buffer;
        lib_job->source_length          = length;
        lib_job->destination_first_ptr  = memory_buffer + length / 2u;
        lib_job->destination_second_ptr = memory_buffer + length;
        lib_job->destination_length     = length;
        lib_job->operation              = DML_OP_DUALCAST;
        

        // Reference job initialization
        ref_job.status = DML_STATUS_OVERLAPPING_BUFFER_ERROR;
    }


    /**
     * @brief Tests that if destination second overlap source first it leads to bad status
     */
    DML_JOB_API_TEST_GENERATOR(dml_dualcast_copy, tb_destination_second_overlap_source_first)
    {
        // Variables
        constexpr auto length = (2u * DUALCAST_ALIGNMENT);
        auto memory_buffer    = dml::test::global_allocator::allocate_ptr(length * 3u);


        // Library job initialization
        lib_job->source_first_ptr       = memory_buffer;
        lib_job->source_length          = length;
        lib_job->destination_second_ptr = memory_buffer + length / 2u;
        lib_job->destination_first_ptr  = memory_buffer + length;
        lib_job->destination_length     = length;
        lib_job->operation              = DML_OP_DUALCAST;
        

        // Reference job initialization
        ref_job.status = DML_STATUS_OVERLAPPING_BUFFER_ERROR;
    }


    /**
     * @brief Tests that if destination first overlap destination second it leads to bad status
     */
    DML_JOB_API_TEST_GENERATOR(dml_dualcast_copy, tb_destination_first_overlap_destination_second)
    {
        // Variables
        constexpr auto length = (2u * DUALCAST_ALIGNMENT);
        auto memory_buffer    = dml::test::global_allocator::allocate_ptr(length * 3u);


        // Library job initialization
        lib_job->source_first_ptr       = memory_buffer;
        lib_job->source_length          = length;
        lib_job->destination_second_ptr = memory_buffer + length;
        lib_job->destination_first_ptr  = memory_buffer + length + length / 2u;
        lib_job->destination_length     = length;
        lib_job->operation              = DML_OP_DUALCAST;
        

        // Reference job initialization
        ref_job.status = DML_STATUS_OVERLAPPING_BUFFER_ERROR;
    }


    /**
     * @brief Tests that if destination second overlap destination first it leads to bad status
     */
    DML_JOB_API_TEST_GENERATOR(dml_dualcast_copy, tb_destination_second_overlap_destination_first)
    {
        // Variables
        constexpr auto length = (2u * DUALCAST_ALIGNMENT);
        auto memory_buffer    = dml::test::global_allocator::allocate_ptr(length * 3u);


        // Library job initialization
        lib_job->source_first_ptr       = memory_buffer;
        lib_job->source_length          = length;
        lib_job->destination_first_ptr  = memory_buffer + length;
        lib_job->destination_second_ptr = memory_buffer + length + length / 2u;
        lib_job->destination_length     = length;
        lib_job->operation              = DML_OP_DUALCAST;
        

        // Reference job initialization
        ref_job.status = DML_STATUS_OVERLAPPING_BUFFER_ERROR;
    }


    /**
     * @brief Tests that if source first overlap destination first it leads to bad status
     */
    DML_JOB_API_TEST_GENERATOR(dml_dualcast_copy, tb_source_first_overlap_destination_first)
    {
        // Variables
        constexpr auto length = (2u * DUALCAST_ALIGNMENT);
        auto memory_buffer    = dml::test::global_allocator::allocate_ptr(length * 3u);


        // Library job initialization
        lib_job->destination_first_ptr  = memory_buffer;
        lib_job->source_first_ptr       = memory_buffer + length / 2u;
        lib_job->source_length          = length;
        lib_job->destination_second_ptr = memory_buffer + length;
        lib_job->destination_length     = length;
        lib_job->operation              = DML_OP_DUALCAST;
        

        // Reference job initialization
        ref_job.status = DML_STATUS_OVERLAPPING_BUFFER_ERROR;
    }


    /**
     * @brief Tests that if source first overlap destination second it leads to bad status
     */
    DML_JOB_API_TEST_GENERATOR(dml_dualcast_copy, tb_source_first_overlap_destination_second)
    {
        // Variables
        constexpr auto length = (2u * DUALCAST_ALIGNMENT);
        auto memory_buffer    = dml::test::global_allocator::allocate_ptr(length * 3u);


        // Library job initialization
        lib_job->destination_second_ptr = memory_buffer;
        lib_job->source_first_ptr       = memory_buffer + length / 2u;
        lib_job->source_length          = length;
        lib_job->destination_first_ptr  = memory_buffer + length;
        lib_job->destination_length     = length;
        lib_job->operation              = DML_OP_DUALCAST;
        

        // Reference job initialization
        ref_job.status = DML_STATUS_OVERLAPPING_BUFFER_ERROR;
    }


    // Tests registration
    DML_JOB_API_TEST_REGISTER(dml_dualcast_copy, tb_source_first_null);
    DML_JOB_API_TEST_REGISTER(dml_dualcast_copy, tb_destination_first_null);
    DML_JOB_API_TEST_REGISTER(dml_dualcast_copy, tb_destination_second_null);
    DML_JOB_API_TEST_REGISTER(dml_dualcast_copy, tb_not_aligned_first_destination);
    DML_JOB_API_TEST_REGISTER(dml_dualcast_copy, tb_not_aligned_second_destination);
    DML_JOB_API_TEST_REGISTER(dml_dualcast_copy, tb_destination_first_overlap_source_first);
    DML_JOB_API_TEST_REGISTER(dml_dualcast_copy, tb_destination_second_overlap_source_first);
    DML_JOB_API_TEST_REGISTER(dml_dualcast_copy, tb_destination_first_overlap_destination_second);
    DML_JOB_API_TEST_REGISTER(dml_dualcast_copy, tb_destination_second_overlap_destination_first);
    DML_JOB_API_TEST_REGISTER(dml_dualcast_copy, tb_source_first_overlap_destination_first);
    DML_JOB_API_TEST_REGISTER(dml_dualcast_copy, tb_source_first_overlap_destination_second);

}
