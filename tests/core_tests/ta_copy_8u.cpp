/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contain Algorithmic tests for @ref dmlc_copy_8u
 * @details Test list:
 *      - @ref ta_copy_equal_length
 *      - @ref ta_copy_zero_length
 *      - @ref ta_copy_overlapping_test
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
 * @brief Tests a @ref dmlc_copy_8u function with non zero vectors length
 */
auto ta_copy_equal_length() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    //Randomizers init
    dml::test::random_t<uint32_t> random_length(DEFAULT_RANGE, seed);
    dml::test::random_t<uint8_t>  random_filler(seed);

    // Constant
    const auto vector_size = random_length.get_next();

    // Variables
    std::vector<uint8_t> source_vector(vector_size, random_filler.get_next());
    std::vector<uint8_t> destination_vector(vector_size, random_filler.get_next());

    // Test
    dml::core::dispatch::mem_move(source_vector.data(), destination_vector.data(), vector_size);

    EXPECT_EQ(source_vector, destination_vector);
}

CORE_TEST_REGISTER(copy_8u, ta_copy_equal_length);


/**
 * @brief Tests a @ref dmlc_copy_8u function with zero length
 */
auto ta_copy_zero_length() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    //Randomizers init
    dml::test::random_t<uint32_t> random_length(DEFAULT_RANGE, seed);
    dml::test::random_t<uint8_t>  random_filler(seed);

    // Constant
    const auto vector_size  = random_length.get_next();
    const auto compare_size = 0u;

    // Variables
    std::vector<uint8_t> source_vector(vector_size, random_filler.get_next());
    std::vector<uint8_t> destination_vector(vector_size, random_filler.get_next());
    std::vector<uint8_t> model_vector = destination_vector;

    // Test
    dml::core::dispatch::mem_move(source_vector.data(), destination_vector.data(), compare_size);

    EXPECT_EQ(model_vector, destination_vector);
}

CORE_TEST_REGISTER(copy_8u, ta_copy_zero_length);


/**
 * @brief Tests a @ref dmlc_copy_8u function with memory overlapping
 */
auto ta_copy_overlapping_test() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    //Randomizers init
    dml::test::random_t<uint32_t> random_length(DEFAULT_RANGE, seed);
    dml::test::random_t<uint8_t>  random_filler(seed);

    // Constant
    const auto vector_size  = random_length.get_next();
    const auto shift_size   = vector_size / 3u;
    const auto compare_size = vector_size / 2u;

    // Variables
    std::vector<uint8_t> vector(vector_size, random_filler.get_next());

    // Test
    dml::core::dispatch::mem_move(vector.data() + shift_size, vector.data(), compare_size);

    const std::vector<uint8_t> source_compare_vector(vector.begin() + shift_size,
                                                     vector.begin() + shift_size + compare_size);
    const std::vector<uint8_t> destination_compare_vector(vector.begin(),
                                                          vector.begin() + compare_size);

    EXPECT_EQ(source_compare_vector, destination_compare_vector);
}

CORE_TEST_REGISTER(copy_8u, ta_copy_overlapping_test);

