/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contain algorithm tests to check dmlc_create_delta_record_8u function
 * @details Test list:
 *      - @ref ta_create_delta_record_for_equals_vectors
 *      - @ref ta_create_delta_record_delta_format
 *      - @ref ta_create_delta_record_bad_positive_small_delta_record_max_size
 *      - @ref ta_create_delta_record_several_delta_note_expected
 *
 * @date 2/17/2020
 *
 */

#include <optimization_dispatcher.hpp>

#include "t_delta_record_feature_defines.hpp"
#include "t_random_generator.hpp"

/* ------ TEST HELPER FUNCTIONS ------ */

/**
 * @brief Compares a delta note with expected offset and delta
 *
 * @param[in] delta_note_ptr             pointer to checked delta note
 * @param[in] expected_delta_note_ptr    pointer to expected delta
 * @param[in] expected_offset            expected offset
 *
 * @return true if delta correct or false in the other case
 */
bool is_correct_delta(const uint8_t *const delta_note_ptr, const uint8_t *const expected_delta_ptr, const offset_t expected_offset)
{
    // Constants
    const auto           offset    = *((offset_t *)delta_note_ptr);
    const uint8_t *const delta_ptr = delta_note_ptr + DELTA_OFFSET_FIELD_SIZE;

    // Variable
    bool delta_is_equal = true;

    // Check
    for (uint32_t i = 0u; i < DELTA_DATA_FIELD_SIZE; i++)
    {
        if (delta_ptr[i] != expected_delta_ptr[i])
        {
            delta_is_equal = false;
            break;
        }
    }

    return (expected_offset == offset && delta_is_equal);
}

/**
 * @brief Compares a delta record notes with reference offsets and deltas
 *
 * @param[in] delta_record     delta record, which are generated with dmlc_create_delta_record_8u
 * @param[in] offset_vector    reference offset vector provided by @ref create_mismatched_vector
 * @param[in] delta_vector     reference delta  vector provided by @ref create_mismatched_vector
 *
 * @return true if delta record correct or false in the other case
 */
bool is_correct_delta_record(const std::vector<uint8_t>  &delta_record,
                             const std::vector<uint32_t> &offset_vector,
                             const std::vector<uint8_t>  &delta_vector)
{
    for (uint32_t i = 0u; i < offset_vector.size(); i++)
    {
        const offset_t       expected_offset    = offset_vector[i];
        const uint8_t *const expected_delta_ptr = &delta_vector[DELTA_DATA_FIELD_SIZE * i];
        const uint8_t *const delta_note_ptr     = &delta_record[DELTA_NOTE_BYTE_SIZE * i];

        const bool check_result = is_correct_delta(delta_note_ptr, expected_delta_ptr, expected_offset);

        if (!check_result)
        {
            return false;
        }
    }

    return true;
}

/**
 * @brief Creates mismatched vector, and reference offset map and delta vector
 *
 * @param[in]  base_vector     standard vector
 * @param[out] offset_vector   will contain offsets
 * @param[out] delta_vector    will contain deltas
 * @param[in]  seed            salt for randomizer
 *
 * @return mismatched vector
 */
std::vector<uint8_t> create_mismatched_vector(const std::vector<uint8_t> &base_vector,
                                              std::vector<uint32_t>      &offset_vector,
                                              std::vector<uint8_t>       &delta_vector,
                                              const uint32_t              seed)
{
    // Constants
    const uint32_t region_count = (uint32_t)base_vector.size() / DELTA_DATA_FIELD_SIZE;

    // Variables
    dml::test::random_t<float> random_mismatch_possibility({ 0.0f, 1.0f }, seed);
    std::vector<uint8_t>       mismatched_vector(base_vector);

    for (uint32_t current_region = 0u; current_region < region_count; current_region++)
    {
        if (random_mismatch_possibility.get_next() > 0.3f)
        {
            // Create offset
            const auto offset = (current_region) ? (current_region * DELTA_DATA_FIELD_SIZE) : current_region;
            offset_vector.push_back(current_region);

            // Create mismatch
            for (uint32_t i = 0u; i < DELTA_DATA_FIELD_SIZE; i++)
            {
                const auto byte_index = offset + i;

                // Create mismatch
                const uint8_t real_byte = base_vector[byte_index];
                const uint8_t anti_byte = ~real_byte;

                mismatched_vector[byte_index] = anti_byte;

                // Write delta
                delta_vector.push_back(real_byte);
            }
        }
    }

    return std::move(mismatched_vector);
}

/* ------ TEST FUNCTIONS ------ */

/**
 * @brief Tests a @ref dmlc_create_delta_record_8u function with equal vectors
 */
auto ta_create_delta_record_eq_vectors() -> void
{
    // Initial Constants
    const auto seed                = test_system::get_seed();
    const auto regions_count_range = REGIONS_COUNT;

    // Randomizers
    dml::test::random_t<uint32_t> random_region_count(regions_count_range, seed);
    dml::test::random_t<uint8_t>  random_byte(seed);

    // Actual constants
    const auto regions_count         = random_region_count.get_next();
    const auto vector_size           = regions_count * DELTA_DATA_FIELD_SIZE;
    const auto max_delta_record_size = regions_count * DELTA_NOTE_BYTE_SIZE;

    // Output Data
    std::vector<uint8_t> delta_record(max_delta_record_size);

    // Input Data preparing
    std::vector<uint8_t> base_vector(vector_size);
    std::generate(base_vector.begin(), base_vector.end(), random_byte);
    std::vector<uint8_t> vector_to_compare(base_vector);

    // Run function
    const auto [delta_record_size, result] = dml::core::dispatch::create_delta(vector_to_compare.data(),
                                                                               base_vector.data(),
                                                                               vector_size,
                                                                               delta_record.data(),
                                                                               max_delta_record_size);

    // Check
    ASSERT_EQ(0, result);
    ASSERT_EQ(0, delta_record_size);
}

CORE_TEST_REGISTER(create_delta_record_8u, ta_create_delta_record_eq_vectors);

/**
 * @brief Tests a @ref dmlc_create_delta_record_8u function on delta record format error
 */
auto ta_create_delta_record_delta_format() -> void
{
    // Randomizer
    dml::test::random_t<uint8_t> random_byte(test_system::get_seed());

    // Input Data preparing
    std::vector<uint8_t> base_vector(DELTA_DATA_FIELD_SIZE);
    std::vector<uint8_t> vector_to_compare(DELTA_DATA_FIELD_SIZE);
    std::generate(base_vector.begin(), base_vector.end(), random_byte);

    for (uint8_t i = 0u; i < DELTA_DATA_FIELD_SIZE; i++)
    {
        vector_to_compare[i] = ~base_vector[i];
    }

    // Output Data
    std::array<uint8_t, DELTA_NOTE_BYTE_SIZE> delta_record{};

    // Real delta calculation
    std::array<uint8_t, DELTA_DATA_FIELD_SIZE> expected_delta{};

    for (uint32_t i = 0u; i < DELTA_DATA_FIELD_SIZE; i++)
    {
        expected_delta[i] = base_vector[i];
    }

    // Run function
    const auto [delta_record_size, result] = dml::core::dispatch::create_delta(vector_to_compare.data(),
                                                                               base_vector.data(),
                                                                               DELTA_DATA_FIELD_SIZE,
                                                                               delta_record.data(),
                                                                               DELTA_NOTE_BYTE_SIZE);
    // Base function checks
    EXPECT_EQ(1, result);

    EXPECT_EQ(DELTA_NOTE_BYTE_SIZE, delta_record_size);

    // Check delta note
    const bool delta_is_correct = is_correct_delta(delta_record.data(), expected_delta.data(), 0u);

    EXPECT_EQ(true, delta_is_correct);
}

CORE_TEST_REGISTER(create_delta_record_8u, ta_create_delta_record_delta_format);

/**
 * @brief Tests a @ref dmlc_create_delta_record_8u function
 * on not enough available bytes count to write next delta note
 */
auto ta_create_delta_record_bad_positive_small_delta_record_max_size() -> void
{
    // Randomizer
    dml::test::random_t<uint8_t> random_byte(test_system::get_seed());

    // Input Data preparing
    constexpr auto                   vector_size = DELTA_DATA_FIELD_SIZE * 2u;
    std::array<uint8_t, vector_size> base_vector{};
    std::array<uint8_t, vector_size> vector_to_compare{};
    std::generate(base_vector.begin(), base_vector.end(), random_byte);

    for (uint8_t i = 0u; i < vector_size; i++)
    {
        vector_to_compare[i] = ~base_vector[i];
    }

    // Output Data
    std::array<uint8_t, DELTA_NOTE_BYTE_SIZE> delta_record{};

    //Run function
    const auto [delta_record_size, result] = dml::core::dispatch::create_delta(vector_to_compare.data(),
                                                                               base_vector.data(),
                                                                               vector_size,
                                                                               delta_record.data(),
                                                                               DELTA_NOTE_BYTE_SIZE);

    EXPECT_EQ(2, result);
}

CORE_TEST_REGISTER(create_delta_record_8u, ta_create_delta_record_bad_positive_small_delta_record_max_size);

/**
 * @brief Tests a @ref dmlc_create_delta_record_8u function in case then several delta notes
 * must be generated
 */
auto ta_create_delta_record_several_delta_note_expected() -> void
{
    // Initial constants
    const auto seed               = test_system::get_seed();
    const auto region_count_range = REGIONS_COUNT;

    //Randomizers
    dml::test::random_t<uint8_t>  random_byte(seed);
    dml::test::random_t<uint32_t> random_region_count(region_count_range, seed);

    // Sizes
    const auto region_count          = random_region_count.get_next();
    const auto vector_size           = region_count * DELTA_DATA_FIELD_SIZE;
    const auto max_delta_record_size = region_count * DELTA_NOTE_BYTE_SIZE;

    //Create base vector and delta record one
    std::vector<uint8_t> base_vector(vector_size);
    std::generate(base_vector.begin(), base_vector.end(), random_byte);
    std::vector<uint8_t> delta_record(max_delta_record_size);

    //Create mismatch vector and reference test data
    std::vector<uint8_t>  mismatched_vector;
    std::vector<uint32_t> offsets;
    std::vector<uint8_t>  deltas;

    mismatched_vector = create_mismatched_vector(base_vector, offsets, deltas, seed);

    const auto [delta_record_size, result] = dml::core::dispatch::create_delta(mismatched_vector.data(),
                                                                               base_vector.data(),
                                                                               vector_size,
                                                                               delta_record.data(),
                                                                               max_delta_record_size);
    EXPECT_EQ(1, result);

    const auto expected_delta_record_size = offsets.size() * DELTA_NOTE_BYTE_SIZE;

    EXPECT_EQ(expected_delta_record_size, delta_record_size);

    const bool delta_record_is_correct = is_correct_delta_record(delta_record, offsets, deltas);

    EXPECT_EQ(true, delta_record_is_correct);
}

CORE_TEST_REGISTER(create_delta_record_8u, ta_create_delta_record_several_delta_note_expected);
