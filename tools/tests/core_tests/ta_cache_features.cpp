/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contain algorithmic tests to check cache manipulation features
 * @details Contains the follow tests:
 *      - @ref ta_dmlc_move_cache_to_memory_run_without_errors
 *      - @ref ta_dmlc_copy_cache_to_memory_run_without_errors
 *
 * @date 2/25/2020
 */

#include "t_common.hpp"
#include "t_random_generator.hpp"
#include "t_random_parameters.hpp"

#include <optimization_dispatcher.hpp>


constexpr dml::test::range_t<uint32_t> DEFAULT_MEMORY_SIZE_RANGE = {TEST_MIN_MEMORY_BLOCK_SIZE,
                                                                    TEST_MAX_MEMORY_BLOCK_SIZE};

/**
 * @brief Tests @dmlc_move_cache_to_memory_8u function on case of exception has been thrown
 */
auto ta_dmlc_move_cache_to_memory_run_without_errors() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    //Randomizers init
    dml::test::random_t<uint32_t> random_length(DEFAULT_MEMORY_SIZE_RANGE, seed);
    dml::test::random_t<uint8_t>  random_byte(seed);

    // Constants
    const auto vector_size = random_length.get_next();

    // Variables
    std::vector<uint8_t> vector(vector_size);

    // Load array into Cache
    std::generate(vector.begin(), vector.end(), random_byte);

    // Check
    dml::core::dispatch::cache_flush(vector.data(), vector_size);

    // Nothing to check
    ASSERT_TRUE(true);
}

CORE_TEST_REGISTER(cache_features, ta_dmlc_move_cache_to_memory_run_without_errors);


/**
 * @brief Tests @dmlc_copy_cache_to_memory_8u function on case of exception has been thrown
 */
auto ta_dmlc_copy_cache_to_memory_run_without_errors() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    //Randomizers init
    dml::test::random_t<uint32_t> random_length(DEFAULT_MEMORY_SIZE_RANGE, seed);
    dml::test::random_t<uint8_t>  random_byte(seed);

    // Constants
    const auto vector_size = random_length.get_next();

    // Variables
    std::vector<uint8_t> vector(vector_size);

    // Load array into Cache
    std::generate(vector.begin(), vector.end(), random_byte);

    // Check
    dml::core::dispatch::cache_write_back(vector.data(), vector_size);

    // Nothing to check
    ASSERT_TRUE(true);
}

CORE_TEST_REGISTER(cache_features, ta_dmlc_copy_cache_to_memory_run_without_errors);
