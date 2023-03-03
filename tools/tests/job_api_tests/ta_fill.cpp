/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains algorithmic tests for DML_OP_FILL operation
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


    /**
     * @brief Tests the operation with random parameters
     */
    DML_JOB_API_TEST_GENERATOR(dml_fill, ta_fill_with_random_parameters)
    {
        // Variables
        const auto seed    = test_system::get_seed();
        auto random_length = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_value  = dml::test::random_t<uint8_t>(seed);
        const auto length  = random_length.get_next();
        const auto value   = random_value.get_next();


        // Library job initialization
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length    = length;
        lib_job->operation             = DML_OP_FILL;


        // Vectors initialization
        std::generate(lib_job->destination_first_ptr, 
                      lib_job->destination_first_ptr + lib_job->destination_length, 
                      random_value);        
        std::fill(lib_job->pattern, 
                  lib_job->pattern + sizeof(lib_job->pattern), 
                  value);
 

        // Reference job initialization
        ref_job.destination_first = dml::test::global_allocator::allocate(length, value);
    }


    // Tests registration
    DML_JOB_API_TEST_REGISTER(dml_fill, ta_fill_with_random_parameters);
    
}
