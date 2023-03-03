/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains algorithmic tests for DML_OP_COMPARE operation
 * @date 4/17/2020
 *
 */
#include "t_common.hpp"
#include "t_random_generator.hpp"
#include "t_random_parameters.hpp"

constexpr dml_meta_result_t EXPECT_MISMATCH = 1u; /** Expect that compared streams are not equal */

namespace dml
{

    /** Default range for tests */
    constexpr dml::test::range_t<uint32_t> DEFAULT_RANGE = {TEST_MIN_MEMORY_BLOCK_SIZE, TEST_MAX_MEMORY_BLOCK_SIZE};


    /**
     * @brief Tests the vectors for equality with random parameters
     */
    DML_JOB_API_TEST_GENERATOR(dml_compare, ta_compare_equal_with_random_parameters)
    {
        // Variables
        const auto seed    = test_system::get_seed();
        auto random_length = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_value  = dml::test::random_t<uint8_t>(seed);
        const auto length  = random_length.get_next();


        // Library job initialization
        lib_job->source_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length    = length;
        lib_job->operation        = DML_OP_COMPARE;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr, 
                      lib_job->source_first_ptr + lib_job->source_length, 
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr, 
                                        lib_job->source_first_ptr + 
                                        lib_job->source_length);
    }


    /**
     * @brief Tests the vectors for not equality with random parameters
     */
    DML_JOB_API_TEST_GENERATOR(dml_compare, ta_compare_not_equal_with_random_parameters)
    {
        // Variables
        const auto seed     = test_system::get_seed();
        auto random_length  = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_value   = dml::test::random_t<uint8_t>(seed);
        const auto length   = random_length.get_next();
        const auto position = (random_length.get_next() % length);


        // Library job initialization
        lib_job->source_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length    = length;
        lib_job->operation        = DML_OP_COMPARE;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr, 
                      lib_job->source_first_ptr + lib_job->source_length, 
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr, 
                                        lib_job->source_first_ptr + 
                                        lib_job->source_length);
        lib_job->source_second_ptr[position] -= 1u;


        // Reference job initialization
        ref_job.status = DML_STATUS_OK;
        ref_job.offset = position;
        ref_job.result = 1u;
    }


    /**
     * @brief Tests the vectors for equality with random parameters
     *        with expected equal result
     */
    DML_JOB_API_TEST_GENERATOR(dml_compare, ta_compare_equal_with_check_result_expect_equal)
    {
        // Variables
        const auto seed    = test_system::get_seed();
        auto random_length = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_value  = dml::test::random_t<uint8_t>(seed);
        const auto length  = random_length.get_next();


        // Library job initialization
        lib_job->source_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length    = length;
        lib_job->operation        = DML_OP_COMPARE;
        lib_job->flags            = DML_FLAG_CHECK_RESULT;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr, 
                      lib_job->source_first_ptr + lib_job->source_length, 
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr, 
                                        lib_job->source_first_ptr + 
                                        lib_job->source_length);
    }


    /**
     * @brief Tests the vectors for equality with random parameters
     *        with expected not equal result
     */
    DML_JOB_API_TEST_GENERATOR(dml_compare, ta_compare_equal_with_check_result_expect_not_equal)
    {
        // Variables
        const auto seed    = test_system::get_seed();
        auto random_length = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_value  = dml::test::random_t<uint8_t>(seed);
        const auto length  = random_length.get_next();


        // Library job initialization
        lib_job->source_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length    = length;
        lib_job->operation        = DML_OP_COMPARE;
        lib_job->flags            = DML_FLAG_CHECK_RESULT;
        lib_job->expected_result  = EXPECT_MISMATCH;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr, 
                      lib_job->source_first_ptr + lib_job->source_length, 
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr, 
                                        lib_job->source_first_ptr + 
                                        lib_job->source_length);

        
        // Reference job initialization
        ref_job.status = DML_STATUS_FALSE_PREDICATE_OK;
    }


    /**
     * @brief Tests the vectors for not equality with random parameters
     *        with expected equal result
     */
    DML_JOB_API_TEST_GENERATOR(dml_compare, ta_compare_not_equal_with_check_result_expect_equal)
    {
        // Variables
        const auto seed     = test_system::get_seed();
        auto random_length  = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_value   = dml::test::random_t<uint8_t>(seed);
        const auto length   = random_length.get_next();
        const auto position = (random_length.get_next() % length);


        // Library job initialization
        lib_job->source_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length    = length;
        lib_job->operation        = DML_OP_COMPARE;
        lib_job->flags            = DML_FLAG_CHECK_RESULT;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr, 
                      lib_job->source_first_ptr + lib_job->source_length, 
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr, 
                                        lib_job->source_first_ptr + 
                                        lib_job->source_length);
        lib_job->source_second_ptr[position] -= 1u;


        // Reference job initialization
        ref_job.status = DML_STATUS_FALSE_PREDICATE_OK;
        ref_job.offset = position;
        ref_job.result = 1u;
    }


    /**
     * @brief Tests the vectors for not equality with random parameters
     *        with expected not equal result
     */
    DML_JOB_API_TEST_GENERATOR(dml_compare, ta_compare_not_equal_with_check_result_expect_not_equal)
    {
        // Variables
        const auto seed     = test_system::get_seed();
        auto random_length  = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_value   = dml::test::random_t<uint8_t>(seed);
        const auto length   = random_length.get_next();
        const auto position = (random_length.get_next() % length);


        // Library job initialization
        lib_job->source_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length    = length;
        lib_job->operation        = DML_OP_COMPARE;
        lib_job->flags            = DML_FLAG_CHECK_RESULT;
        lib_job->expected_result  = EXPECT_MISMATCH;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr, 
                      lib_job->source_first_ptr + lib_job->source_length, 
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr, 
                                        lib_job->source_first_ptr + 
                                        lib_job->source_length);
        lib_job->source_second_ptr[position] -= 1u;


        // Reference job initialization
        ref_job.offset = position;
        ref_job.result = 1u;
    }


    // Tests registration
    DML_JOB_API_TEST_REGISTER(dml_compare, ta_compare_equal_with_random_parameters);
    DML_JOB_API_TEST_REGISTER(dml_compare, ta_compare_not_equal_with_random_parameters);
    DML_JOB_API_TEST_REGISTER(dml_compare, ta_compare_equal_with_check_result_expect_equal);
    DML_JOB_API_TEST_REGISTER(dml_compare, ta_compare_equal_with_check_result_expect_not_equal);
    DML_JOB_API_TEST_REGISTER(dml_compare, ta_compare_not_equal_with_check_result_expect_equal);
    DML_JOB_API_TEST_REGISTER(dml_compare, ta_compare_not_equal_with_check_result_expect_not_equal);

}
