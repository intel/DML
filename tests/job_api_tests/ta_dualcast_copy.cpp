/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains algorithmic tests for DML_OP_DUALCAST operation
 * @date 4/17/2020
 *
 */
#include "t_common.hpp"
#include "t_random_generator.hpp"
#include "t_random_parameters.hpp"


namespace dml
{

    /** Default range for tests */
    constexpr dml::test::range_t<uint32_t> DEFAULT_RANGE = {TEST_MIN_MEMORY_BLOCK_SIZE, TEST_MAX_MEMORY_BLOCK_SIZE};

    /** Specific alignment for the operation **/
    constexpr auto DUALCAST_ALIGNMENT = 0x1000u;


    /**
     * @brief Tests the operation with random parameters
     */
    DML_JOB_API_TEST_GENERATOR(dml_dualcast_copy, ta_copy_with_random_parameters)
    {
        // Variables
        const auto seed          = test_system::get_seed();
        auto random_length       = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_value        = dml::test::random_t<uint8_t>(seed);
        const auto length        = random_length.get_next();
        const auto need_to_align = DUALCAST_ALIGNMENT + (length / DUALCAST_ALIGNMENT) * DUALCAST_ALIGNMENT;
        auto destination_buffer  = dml::test::global_allocator::allocate_ptr(length * 2u + need_to_align);


        // Library job initialization
        lib_job->source_first_ptr       = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length          = length;
        lib_job->destination_first_ptr  = destination_buffer;
        lib_job->destination_second_ptr = destination_buffer + need_to_align;
        lib_job->destination_length     = length;
        lib_job->operation              = DML_OP_DUALCAST;
 

        // Vectors initialization
        std::generate(lib_job->source_first_ptr, 
                      lib_job->source_first_ptr + lib_job->source_length, 
                      random_value);
        std::generate(lib_job->destination_first_ptr, 
                      lib_job->destination_first_ptr + lib_job->destination_length, 
                      random_value);
        std::generate(lib_job->destination_second_ptr, 
                      lib_job->destination_second_ptr + lib_job->destination_length, 
                      random_value);


        // Reference job initialization
        ref_job.destination_first  = dml::test::global_allocator::allocate(
                                        lib_job->source_first_ptr, 
                                        lib_job->source_first_ptr + lib_job->source_length);
        ref_job.destination_second = dml::test::global_allocator::allocate(
                                        lib_job->source_first_ptr, 
                                        lib_job->source_first_ptr + lib_job->source_length);
    }


    // Tests registration
    DML_JOB_API_TEST_REGISTER(dml_dualcast_copy, ta_copy_with_random_parameters);

}
