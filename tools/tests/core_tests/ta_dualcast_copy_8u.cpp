/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contain Algorithmic tests for @ref dmlc_dualcast_copy_8u
 * @details Test list:
 *          - @ref ta_dualcast_copy_equal_length
 *          - @ref ta_dualcast_copy_zero_length
 *
 * @date 2/21/2020
 *
 */

#include <optimization_dispatcher.hpp>

#include "t_common.hpp"
#include "t_random_generator.hpp"
#include "t_random_parameters.hpp"

/** Default range for tests */
constexpr dml::test::range_t<uint32_t> DEFAULT_RANGE = { TEST_MIN_MEMORY_BLOCK_SIZE, TEST_MAX_MEMORY_BLOCK_SIZE };

/** Specific alignment for the operation **/
constexpr auto DUALCAST_ALIGNMENT = 0x1000u;

/**
 * @brief Tests a @ref dmlc_dualcast_copy_8u function with non zero vectors length
 */
auto ta_dualcast_copy_equal_length() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    //Randomizers init
    dml::test::random_t<uint32_t> random_length(DEFAULT_RANGE, seed);
    dml::test::random_t<uint8_t>  random_filler(seed);

    // Constant
    const auto vector_size   = random_length.get_next();
    const auto need_to_align = DUALCAST_ALIGNMENT + (vector_size / DUALCAST_ALIGNMENT) * DUALCAST_ALIGNMENT;

    // Variables
    std::vector<uint8_t> source_vector(vector_size, random_filler.get_next());
    std::vector<uint8_t> destination_buffer(vector_size * 2u + need_to_align, random_filler.get_next());

    // Test
    dml::core::dispatch::dualcast(source_vector.data(), destination_buffer.data(), destination_buffer.data() + need_to_align, vector_size);

    // Constants for checking
    const std::vector<uint8_t> destination_vector1(destination_buffer.begin(), destination_buffer.begin() + vector_size);
    const std::vector<uint8_t> destination_vector2(destination_buffer.begin() + need_to_align,
                                                   destination_buffer.begin() + need_to_align + vector_size);

    EXPECT_EQ(source_vector, destination_vector1);
    EXPECT_EQ(source_vector, destination_vector2);
}

CORE_TEST_REGISTER(dualcast_copy_8u, ta_dualcast_copy_equal_length);

/**
 * @brief Tests a @ref dmlc_dualcast_copy_8u function with zero vectors length
 */
auto ta_dualcast_copy_zero_length() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    //Randomizers init
    dml::test::random_t<uint32_t> random_length(DEFAULT_RANGE, seed);
    dml::test::random_t<uint8_t>  random_filler(seed);

    // Constant
    const auto vector_size   = random_length.get_next();
    const auto need_to_align = DUALCAST_ALIGNMENT + (vector_size / DUALCAST_ALIGNMENT) * DUALCAST_ALIGNMENT;
    const auto compare_size  = 0u;

    // Variables
    std::vector<uint8_t> source_vector(vector_size, random_filler.get_next());
    std::vector<uint8_t> destination_buffer(vector_size * 2u + need_to_align, random_filler.get_next());

    const std::vector<uint8_t> model_vector1(destination_buffer.begin(), destination_buffer.begin() + vector_size);
    const std::vector<uint8_t> model_vector2(destination_buffer.begin() + need_to_align,
                                             destination_buffer.begin() + need_to_align + vector_size);

    // Test
    dml::core::dispatch::dualcast(source_vector.data(), destination_buffer.data(), destination_buffer.data() + need_to_align, compare_size);

    // Constants for checking
    const std::vector<uint8_t> destination_vector1(destination_buffer.begin(), destination_buffer.begin() + vector_size);
    const std::vector<uint8_t> destination_vector2(destination_buffer.begin() + need_to_align,
                                                   destination_buffer.begin() + need_to_align + vector_size);

    EXPECT_EQ(model_vector1, destination_vector1);
    EXPECT_EQ(model_vector2, destination_vector2);
}

CORE_TEST_REGISTER(dualcast_copy_8u, ta_dualcast_copy_zero_length);
