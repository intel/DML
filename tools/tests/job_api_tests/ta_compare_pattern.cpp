/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains algorithmic tests for DML_OP_COMPARE_PATTERN operation
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
     * @brief Tests the vector and pattern for equality with random parameters
     */
    DML_JOB_API_TEST_GENERATOR(dml_compare_pattern, ta_compare_equal_with_random_parameters)
    {
        // Variables
        const auto seed    = test_system::get_seed();
        auto random_length = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_value  = dml::test::random_t<uint8_t>(seed);
        const auto length  = random_length.get_next();
        const auto filler  = random_value.get_next();


        // Library job initialization
        lib_job->source_first_ptr = dml::test::global_allocator::allocate_ptr(length, filler);
        lib_job->source_length    = length;
        lib_job->operation        = DML_OP_COMPARE_PATTERN;


        // Vectors initialization
        std::fill(lib_job->pattern,
                  lib_job->pattern + sizeof(uint64_t),
                  filler);
    }


    /**
     * @brief Tests the vector and pattern for not equality with random parameters
     */
    DML_JOB_API_TEST_GENERATOR(dml_compare_pattern, ta_compare_not_equal_with_random_parameters)
    {
        // Variables
        const auto seed     = test_system::get_seed();
        auto random_length  = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_value   = dml::test::random_t<uint8_t>(seed);
        const auto length   = random_length.get_next();
        const auto filler   = random_value.get_next();
        const auto position = (random_length.get_next() % length);


        // Library job initialization
        lib_job->source_first_ptr = dml::test::global_allocator::allocate_ptr(length, filler);
        lib_job->source_length    = length;
        lib_job->operation        = DML_OP_COMPARE_PATTERN;


        // Vectors initialization
        std::fill(lib_job->pattern,
                  lib_job->pattern + sizeof(uint64_t),
                  filler);
        lib_job->source_first_ptr[position] -= 1u;


        // Reference job initialization
        if ((length - (length % sizeof(uint64_t))) < position)
        {
            // Mismatch in "tail"
            ref_job.offset = position;
        }
        else
        {
            ref_job.offset = position - (position % sizeof(uint64_t));
        }

        ref_job.status = DML_STATUS_OK;
        ref_job.result = 1u;
    }


    /**
     * @brief Tests the vector and pattern for equality with random parameters
     *        with expected equal result
     */
    DML_JOB_API_TEST_GENERATOR(dml_compare_pattern, ta_compare_equal_with_check_result_expect_equal)
    {
        // Variables
        const auto seed    = test_system::get_seed();
        auto random_length = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_value  = dml::test::random_t<uint8_t>(seed);
        const auto length  = random_length.get_next();
        const auto filler  = random_value.get_next();


        // Library job initialization
        lib_job->source_first_ptr = dml::test::global_allocator::allocate_ptr(length, filler);
        lib_job->source_length    = length;
        lib_job->operation        = DML_OP_COMPARE_PATTERN;
        lib_job->flags            = DML_FLAG_CHECK_RESULT;


        // Vectors initialization
        std::fill(lib_job->pattern,
                  lib_job->pattern + sizeof(uint64_t),
                  filler);
    }


    /**
     * @brief Tests the vector and pattern for equality with random parameters
     *        with expected not equal result
     */
    DML_JOB_API_TEST_GENERATOR(dml_compare_pattern, ta_compare_equal_with_check_result_expect_not_equal)
    {
        // Variables
        const auto seed    = test_system::get_seed();
        auto random_length = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_value  = dml::test::random_t<uint8_t>(seed);
        const auto length  = random_length.get_next();
        const auto filler  = random_value.get_next();


        // Library job initialization
        lib_job->source_first_ptr = dml::test::global_allocator::allocate_ptr(length, filler);
        lib_job->source_length    = length;
        lib_job->operation        = DML_OP_COMPARE_PATTERN;
        lib_job->flags            = DML_FLAG_CHECK_RESULT;
        lib_job->expected_result  = EXPECT_MISMATCH;


        // Vectors initialization
        std::fill(lib_job->pattern,
                  lib_job->pattern + sizeof(uint64_t),
                  filler);


        // Reference job initialization
        ref_job.status = DML_STATUS_FALSE_PREDICATE_OK;
    }


    /**
     * @brief Tests the vector and pattern for not equality with random parameters
     *        with expected equal result
     */
    DML_JOB_API_TEST_GENERATOR(dml_compare_pattern, ta_compare_not_equal_with_check_result_expect_equal)
    {
        // Variables
        const auto seed     = test_system::get_seed();
        auto random_length  = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_value   = dml::test::random_t<uint8_t>(seed);
        const auto length   = random_length.get_next();
        const auto filler   = random_value.get_next();
        const auto position = (random_length.get_next() % length);


        // Library job initialization
        lib_job->source_first_ptr = dml::test::global_allocator::allocate_ptr(length, filler);
        lib_job->source_length    = length;
        lib_job->operation        = DML_OP_COMPARE_PATTERN;
        lib_job->flags            = DML_FLAG_CHECK_RESULT;


        // Vectors initialization
        std::fill(lib_job->pattern,
                  lib_job->pattern + sizeof(uint64_t),
                  filler);
        lib_job->source_first_ptr[position] -= 1u;


        // Reference job initialization
        if ((length - (length % sizeof(uint64_t))) < position)
        {
            // Mismatch in "tail"
            ref_job.offset = position;
        }
        else
        {
            ref_job.offset = position - (position % sizeof(uint64_t));
        }

        ref_job.status = DML_STATUS_FALSE_PREDICATE_OK;
        ref_job.result = 1u;
    }


    /**
     * @brief Tests the vector and pattern for not equality with random parameters
     *        with expected not equal result
     */
    DML_JOB_API_TEST_GENERATOR(dml_compare_pattern, ta_compare_not_equal_with_check_result_expect_not_equal)
    {
        // Variables
        const auto seed     = test_system::get_seed();
        auto random_length  = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_value   = dml::test::random_t<uint8_t>(seed);
        const auto length   = random_length.get_next();
        const auto filler   = random_value.get_next();
        const auto position = (random_length.get_next() % length);


        // Library job initialization
        lib_job->source_first_ptr = dml::test::global_allocator::allocate_ptr(length, filler);
        lib_job->source_length    = length;
        lib_job->operation        = DML_OP_COMPARE_PATTERN;
        lib_job->flags            = DML_FLAG_CHECK_RESULT;
        lib_job->expected_result  = EXPECT_MISMATCH;


        // Vectors initialization
        std::fill(lib_job->pattern,
                  lib_job->pattern + sizeof(uint64_t),
                  filler);
        lib_job->source_first_ptr[position] -= 1u;


        // Reference job initialization
        if ((length - (length % sizeof(uint64_t))) < position)
        {
            // Mismatch in "tail"
            ref_job.offset = position;
        }
        else
        {
            ref_job.offset = position - (position % sizeof(uint64_t));
        }

        ref_job.status = DML_STATUS_OK;
        ref_job.result = 1u;
    }


    // Tests registration
    DML_JOB_API_TEST_REGISTER(dml_compare_pattern, ta_compare_equal_with_random_parameters);
    DML_JOB_API_TEST_REGISTER(dml_compare_pattern, ta_compare_not_equal_with_random_parameters);
    DML_JOB_API_TEST_REGISTER(dml_compare_pattern, ta_compare_equal_with_check_result_expect_equal);
    DML_JOB_API_TEST_REGISTER(dml_compare_pattern, ta_compare_equal_with_check_result_expect_not_equal);
    DML_JOB_API_TEST_REGISTER(dml_compare_pattern, ta_compare_not_equal_with_check_result_expect_equal);
    DML_JOB_API_TEST_REGISTER(dml_compare_pattern, ta_compare_not_equal_with_check_result_expect_not_equal);

}
