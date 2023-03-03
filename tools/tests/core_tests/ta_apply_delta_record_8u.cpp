/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contain test to check @ref dmlc_apply_delta_record_8u
 * @details Test list:
 *          - @ref ta_apply_delta_record_format
 *          - @ref ta_apply_delta_record_large_offset
 *          - @ref ta_apply_delta_record_cross
 *
 * @date 2/25/2020
 */

#include "t_delta_record_feature_defines.hpp"
#include "t_random_generator.hpp"
#include <optimization_dispatcher.hpp>

/**
 * @brief Tests what delta record apply is a works correctly in accordance with format
 */
auto ta_apply_delta_record_format() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    // Randomizers
    dml::test::random_t<uint8_t> random_byte(seed);

    // Constants
    constexpr auto vector_size        = DELTA_DATA_FIELD_SIZE;
    constexpr auto delta_record_size  = DELTA_NOTE_BYTE_SIZE;

    // Variables
    std::vector<uint8_t> base_array(vector_size);
    std::vector<uint8_t> mismatched_array(vector_size);
    std::vector<uint8_t> delta_record(delta_record_size);

    // Vectors init
    std::generate(base_array.begin(), base_array.end(), random_byte);

    for (std::size_t i = 0u; i < DELTA_DATA_FIELD_SIZE; i++)
    {
        const auto pattern      = base_array[i];
        const auto anti_pattern = ~pattern;
        mismatched_array[i] = anti_pattern;
    }

    // Create Delta note.
    // 1. Zero Offset set.
    delta_record[0] = 0u;
    delta_record[1] = 0u;

    // 2. Write deltas.
    for (std::size_t j = 0u; j < DELTA_DATA_FIELD_SIZE; j++)
    {
        delta_record[j + sizeof(offset_t)] = base_array[j];
    }

    // Apply delta record
    dml::core::dispatch::apply_delta(delta_record.data(), mismatched_array.data(), delta_record_size);
    ASSERT_EQ(base_array, mismatched_array);
}

CORE_TEST_REGISTER(apply_delta_record_8u, ta_apply_delta_record_format);


/**
 * @brief Tests what delta record functions works correctly in pair
 */
auto ta_apply_delta_record_cross() -> void
{
    // Seed
    const auto seed = test_system::get_seed();
    const auto pattern_count_range = REGIONS_COUNT;

    // Randomizers
    dml::test::random_t<uint8_t>  random_byte(seed);
    dml::test::random_t<uint32_t> random_pattern_count(pattern_count_range, seed);

    // Constants
    const auto data_regions_count    = random_pattern_count.get_next();
    const auto vector_size           = DELTA_DATA_FIELD_SIZE * data_regions_count;
    const auto delta_record_max_size = DELTA_NOTE_BYTE_SIZE * data_regions_count;

    // Variables
    std::vector<uint8_t> base_vector(vector_size);
    std::vector<uint8_t> mismatched_vector(base_vector);
    std::vector<uint8_t> delta_record_vector(delta_record_max_size);

    // Vectors init
    std::generate(base_vector.begin(), base_vector.end(), random_byte);

    for (std::size_t i = 0u; i < base_vector.size(); i++)
    {
        const auto pattern      = base_vector[i];
        const auto anti_pattern = ~pattern;
        mismatched_vector[i]    = anti_pattern;
    }

    // Create delta record
    const auto [delta_record_size, result] = dml::core::dispatch::create_delta(mismatched_vector.data(),
                                                                        base_vector.data(),
                                                                        vector_size,
                                                                        delta_record_vector.data(),
                                                                        delta_record_max_size);
    ASSERT_EQ(result, 1);
    ASSERT_EQ(delta_record_max_size, delta_record_size);

    // Apply delta record
    dml::core::dispatch::apply_delta(delta_record_vector.data(), mismatched_vector.data(), delta_record_size);

    ASSERT_EQ(base_vector, mismatched_vector);
}

CORE_TEST_REGISTER(apply_delta_record_8u, ta_apply_delta_record_cross);
