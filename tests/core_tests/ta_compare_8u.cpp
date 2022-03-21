/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contain Algorithmic tests for @ref dmlc_compare_8u
 * @details Test list:
 *      - @ref ta_compare_equal_vectors
 *      - @ref ta_compare_fully_different_vectors
 *      - @ref ta_compare_partially_equal_vectors
 *      - @ref ta_test_not_equal_last_byte
 *
 * @date 2/11/2020
 *
 */

#include "t_common.hpp"
#include "t_random_generator.hpp"
#include "t_random_parameters.hpp"
#include <optimization_dispatcher.hpp>

constexpr dml::test::range_t<uint32_t> DEFAULT_MEMORY_SIZE_RANGE = {TEST_MIN_MEMORY_BLOCK_SIZE,
                                                                    TEST_MAX_MEMORY_BLOCK_SIZE};

/**
 * @brief Test a @ref dmlc_compare_8u function with equal vectors
 */
auto ta_compare_equal_vectors() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    //Randomizers init
    dml::test::random_t<uint32_t> random_length(DEFAULT_MEMORY_SIZE_RANGE, seed);
    dml::test::random_t<uint8_t>  random_filler(seed);

    // Constant
    const auto vector_size = random_length.get_next();

    // Variables
    uint32_t             miss_match_offset = 0u;
    std::vector<uint8_t> base_vector(vector_size, random_filler.get_next());
    std::vector<uint8_t> compared_vector(base_vector);

    auto [mismatch, result] = dml::core::dispatch::compare(base_vector.data(), compared_vector.data(), vector_size);

    ASSERT_EQ(result, 0);
    ASSERT_EQ(mismatch, 0);
}

CORE_TEST_REGISTER(compare_8u, ta_compare_equal_vectors);


/**
 * @brief Test a @ref dmlc_compare_8u function with different vectors
 */
auto ta_compare_fully_different_vectors() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    //Randomizers init
    dml::test::random_t<uint32_t> random_length(DEFAULT_MEMORY_SIZE_RANGE, seed);
    dml::test::random_t<uint8_t> random_filler(seed);

    // Constants
    const auto     vector_size                = random_length.get_next();
    const auto     base_vector_filler         = (uint8_t) random_filler.get_next();
    const uint8_t  compared_vector_filler     = ~(base_vector_filler);
    const uint32_t expected_miss_match_offset = 0u;

    // Variables
    uint32_t             miss_match_offset = 0u;
    std::vector<uint8_t> base_vector(vector_size, base_vector_filler);
    std::vector<uint8_t> compared_vector(vector_size, compared_vector_filler);

    auto [mismatch, result] = dml::core::dispatch::compare(base_vector.data(), compared_vector.data(), vector_size);

    ASSERT_EQ(result, 1);
    ASSERT_EQ(mismatch, 0);
}

CORE_TEST_REGISTER(compare_8u, ta_compare_fully_different_vectors);


/**
 * @brief Test a @ref dmlc_compare_8u function with partially equal vectors
 */
auto ta_compare_partially_equal_vectors() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    //Randomizers init
    dml::test::random_t<uint32_t> random_length(DEFAULT_MEMORY_SIZE_RANGE, seed);
    dml::test::random_t<uint8_t> random_filler(seed);

    // Constants
    const auto     vector_size                = random_length.get_next();
    const auto     base_vector_filler         = (uint8_t) random_filler.get_next();
    const uint8_t  compared_vector_filler     = ~(base_vector_filler);
    const uint32_t expected_miss_match_offset = vector_size / 2u;

    // Variables
    std::vector<uint8_t> base_vector(vector_size, base_vector_filler);
    std::vector<uint8_t> compared_vector(base_vector);

    // Prepare compared vector
    std::fill(compared_vector.begin() + expected_miss_match_offset,
              compared_vector.end(),
              compared_vector_filler);

    auto [mismatch, result] = dml::core::dispatch::compare(base_vector.data(), compared_vector.data(), vector_size);

    ASSERT_EQ(result, 1);
    ASSERT_EQ(mismatch, expected_miss_match_offset);
}

CORE_TEST_REGISTER(compare_8u, ta_compare_partially_equal_vectors);


/**
 * @brief Test a @ref dmlc_compare_8u function with vectors, which last bytes are not equal
 */
auto ta_test_not_equal_last_byte() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    //Randomizers init
    dml::test::random_t<uint32_t> random_length(DEFAULT_MEMORY_SIZE_RANGE, seed);
    dml::test::random_t<uint8_t>  random_filler(seed);

    // Constants
    const auto     vector_size                = random_length.get_next();
    const auto     base_vector_filler         = (uint8_t) random_filler.get_next();
    const uint8_t  compared_vector_filler     = ~(base_vector_filler);
    const uint32_t expected_miss_match_offset = TEST_LAST_FROM(vector_size);

    // Variables
    std::vector<uint8_t> base_vector(vector_size, base_vector_filler);
    std::vector<uint8_t> compared_vector(base_vector);

    // Prepare compared vector
    compared_vector[TEST_LAST_FROM(vector_size)] = compared_vector_filler;

    auto [mismatch, result] = dml::core::dispatch::compare(base_vector.data(), compared_vector.data(), vector_size);

    ASSERT_EQ(result, 1);
    ASSERT_EQ(mismatch, expected_miss_match_offset);
}

CORE_TEST_REGISTER(compare_8u, ta_test_not_equal_last_byte);
