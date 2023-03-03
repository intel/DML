/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contain Algorithmic tests for @ref dmlc_fill_with_pattern_8u
 * @details Test list:
 *          - @ref ta_fill_with_pattern_with_length_multiple_of_element_size
 *          - @ref ta_fill_with_pattern_with_length_not_multiple_of_element_size
 *          - @ref ta_fill_with_pattern_with_zero_length
 *
 * @date 2/20/2020
 *
 */

#include "t_common.hpp"
#include "t_random_generator.hpp"
#include "t_random_parameters.hpp"
#include <optimization_dispatcher.hpp>


/** Default range for tests */
constexpr dml::test::range_t<uint32_t> DEFAULT_RANGE = {TEST_MIN_MEMORY_BLOCK_SIZE, TEST_MAX_MEMORY_BLOCK_SIZE};


/**
 * @brief Tests a @ref dmlc_fill_with_pattern_8u function with length multiple of element size
 */
auto ta_fill_with_pattern_with_length_multiple_of_element_size() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    //Randomizers init
    dml::test::random_t<uint32_t> random_length(DEFAULT_RANGE, seed);
    dml::test::random_t<uint8_t>  vector_filler(seed);
    dml::test::random_t<uint64_t> random_element(seed);

    // Constant
    const auto vector_size      = sizeof(uint64_t) * random_length.get_next();
    const auto element_to_fill  = random_element.get_next();

    // Variables
    std::vector<uint8_t> source_vector(vector_size, vector_filler.get_next());

    // Test
    dml::core::dispatch::fill(element_to_fill, source_vector.data(), (uint32_t)vector_size);

    const auto *array_element = (const uint8_t *)(&element_to_fill);
    for (uint32_t i = 0u; i < vector_size; ++i)
    {
        EXPECT_EQ(array_element[i % sizeof(uint64_t)], source_vector[i]);
    }
}

CORE_TEST_REGISTER(fill_with_pattern_8u, ta_fill_with_pattern_with_length_multiple_of_element_size);


/**
 * @brief Tests a @ref dmlc_fill_with_pattern_8u function with length not multiple of element size
 */
auto ta_fill_with_pattern_with_length_not_multiple_of_element_size() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    //Randomizers init
    dml::test::random_t<uint32_t> random_length(DEFAULT_RANGE, seed);
    dml::test::random_t<uint8_t>  vector_filler(seed);
    dml::test::random_t<uint64_t> random_element(seed);

    // Constant
    const auto vector_size      = sizeof(uint64_t) * random_length.get_next() -
                                 (1u + random_length.get_next() % (sizeof(uint64_t) - 1u));
    const auto element_to_fill  = random_element.get_next();

    // Variables
    std::vector<uint8_t> source_vector(vector_size, vector_filler.get_next());

    // Test
    dml::core::dispatch::fill(element_to_fill, source_vector.data(), (uint32_t)vector_size);

    const auto *array_element = (const uint8_t *)(&element_to_fill);
    for (uint32_t i = 0u; i < vector_size; ++i)
    {
        EXPECT_EQ(array_element[i % sizeof(uint64_t)], source_vector[i]);
    }
}

CORE_TEST_REGISTER(fill_with_pattern_8u, ta_fill_with_pattern_with_length_not_multiple_of_element_size);


/**
 * @brief Tests a @ref dmlc_fill_with_pattern_8u function with zero length
 */
auto ta_fill_with_pattern_with_zero_length() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    //Randomizers init
    dml::test::random_t<uint32_t> random_length(DEFAULT_RANGE, seed);
    dml::test::random_t<uint8_t>  vector_filler(seed);
    dml::test::random_t<uint64_t> random_element(seed);

    // Constant
    const auto vector_size      = random_length.get_next();
    const auto element_to_fill  = random_element.get_next();

    // Variables
    std::vector<uint8_t> source_vector(vector_size, vector_filler.get_next());
    const std::vector<uint8_t> model_vector = source_vector;

    // Test
    dml::core::dispatch::fill(element_to_fill, source_vector.data(), (uint32_t)0u);

    EXPECT_EQ(model_vector, source_vector);
}

CORE_TEST_REGISTER(fill_with_pattern_8u, ta_fill_with_pattern_with_zero_length);
