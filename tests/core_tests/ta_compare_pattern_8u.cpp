/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contain algorithm tests to check compare with pattern algorithm
 * @detils Test list:
 * - @ref ta_compare_pattern_eq_aligned
 * - @ref ta_compare_pattern_eq_not_aligned
 * - @ref ta_compare_pattern_ne
 * - @ref ta_compare_pattern_partially_ne_aligned
 * - @ref ta_compare_pattern_partially_ne_not_aligned
 * - @ref ta_compare_pattern_last_byte_ne_aligned
 * - @ref ta_compare_pattern_last_byte_ne_not_aligned
 *
 * @date 2/12/2020
 */

#include <optimization_dispatcher.hpp>

#include "t_common.hpp"
#include "t_random_generator.hpp"
#include "t_random_parameters.hpp"

constexpr dml::test::range_t<uint64_t> MULTIPLY_BREAKER_RANGE = { 1u, 7u };
constexpr dml::test::range_t<uint64_t> PATTERN_COUNT          = { TEST_MIN_MEMORY_BLOCK_SIZE / sizeof(uint64_t),
                                                         TEST_MAX_MEMORY_BLOCK_SIZE / sizeof(uint64_t) };

/**
 * @brief Test dmlc_compare_pattern_8u in case if memory is fully consistent with 8-byte pattern.
 *
 * @note Memory region size is multiply to 8 bytes.
 */
auto ta_compare_pattern_eq_aligned() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    // Randomizers init
    dml::test::random_t<uint64_t> random_pattern(seed);
    dml::test::random_t<uint64_t> random_vector_size(PATTERN_COUNT, seed);

    // Constants
    const auto vector_size = random_vector_size.get_next();
    const auto pattern     = random_pattern.get_next();

    // Variables
    uint32_t              mismatch_offset = 0u;
    std::vector<uint64_t> test_vector(vector_size, pattern);

    auto [mismatch, result] = dml::core::dispatch::compare_pattern(pattern,
                                                                   reinterpret_cast<const uint8_t *>(test_vector.data()),
                                                                   vector_size * sizeof(uint64_t));

    ASSERT_EQ(result, 0);
    ASSERT_EQ(mismatch, 0);
}

CORE_TEST_REGISTER(compare_pattern_8u, ta_compare_pattern_eq_aligned);

/**
 * @brief Test dmlc_compare_pattern_8u in case if memory is fully consistent with 8-byte pattern.
 *
 * @note Memory region size is not multiply to 8 bytes.
 */
auto ta_compare_pattern_eq_not_aligned() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    // Randomizers init
    dml::test::random_t<uint64_t> random_pattern(seed);
    dml::test::random_t<uint64_t> random_vector_size(PATTERN_COUNT, seed);
    dml::test::random_t<uint64_t> random_multiply_breaker(MULTIPLY_BREAKER_RANGE, seed);

    // Constants
    const auto vector_size = random_vector_size.get_next();
    const auto pattern     = random_pattern.get_next();

    // Variables
    uint32_t              mismatch_offset = 0u;
    std::vector<uint64_t> test_vector(vector_size, pattern);

    auto [mismatch, result] = dml::core::dispatch::compare_pattern(pattern,
                                                                   reinterpret_cast<const uint8_t *>(test_vector.data()),
                                                                   vector_size * sizeof(uint64_t));

    ASSERT_EQ(result, 0);
    ASSERT_EQ(mismatch, 0);
}

CORE_TEST_REGISTER(compare_pattern_8u, ta_compare_pattern_eq_not_aligned);

/**
 * @brief Test dmlc_compare_pattern_8u in case if memory isn't consistent with 8-byte pattern.
 *
 * @note Mismatch position is aligned by pattern width.
 */
auto ta_compare_pattern_ne() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    // Randomizers init
    dml::test::random_t<uint64_t> random_pattern(seed);
    dml::test::random_t<uint64_t> random_vector_size(PATTERN_COUNT, seed);

    // Constants
    const auto     vector_size  = random_vector_size.get_next();
    const auto     pattern      = random_pattern.get_next();
    const uint64_t anti_pattern = ~pattern;

    // Define a first mismatch byte offsets
    const uint64_t expected_mismatch_offset         = 0u;
    const uint64_t expected_mismatch_pattern_offset = expected_mismatch_offset;

    // Variables
    std::vector<uint64_t> test_vector(vector_size, anti_pattern);

    auto [mismatch, result] = dml::core::dispatch::compare_pattern(pattern,
                                                                   reinterpret_cast<const uint8_t *>(test_vector.data()),
                                                                   vector_size * sizeof(uint64_t));

    ASSERT_EQ(result, 1);
    ASSERT_GE(expected_mismatch_offset, mismatch);
    ASSERT_LE(expected_mismatch_pattern_offset, mismatch);
}

CORE_TEST_REGISTER(compare_pattern_8u, ta_compare_pattern_ne);

/**
 * @brief Test dmlc_compare_pattern_8u in case if memory isn't consistent with 8-byte pattern.
 *
 * @note Mismatch position is'not aligned by pattern width.
 *
 */
auto ta_compare_pattern_partially_ne_aligned() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    // Randomizers init
    dml::test::random_t<uint64_t> random_pattern(seed);
    dml::test::random_t<uint64_t> random_vector_size(PATTERN_COUNT, seed);

    // Constants
    const auto     vector_size                   = random_vector_size.get_next();
    const uint64_t pattern                       = random_pattern.get_next();
    const uint64_t anti_pattern                  = ~pattern;
    const uint32_t anti_pattern_offset           = vector_size / 2u;
    const uint32_t expected_mismatch_byte_offset = anti_pattern_offset * sizeof(uint64_t);

    // Variables
    uint32_t              mismatch_byte_offset = 0u;
    std::vector<uint64_t> test_vector(vector_size);

    // Vector preparation
    std::fill(test_vector.begin(), test_vector.begin() + anti_pattern_offset, pattern);
    std::fill(test_vector.begin() + anti_pattern_offset, test_vector.end(), anti_pattern);

    // Checks
    auto [mismatch, result] = dml::core::dispatch::compare_pattern(pattern,
                                                                   reinterpret_cast<const uint8_t *>(test_vector.data()),
                                                                   vector_size * sizeof(uint64_t));

    ASSERT_EQ(result, 1);
    ASSERT_GE(expected_mismatch_byte_offset, mismatch);
}

CORE_TEST_REGISTER(compare_pattern_8u, ta_compare_pattern_partially_ne_aligned);

/**
 * @brief Test dmlc_compare_pattern_8u in case if memory isn't consistent with 8-byte pattern
 * and mismatch is not aligned to pattern byte size
 */
auto ta_compare_pattern_partially_ne_not_aligned() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    // Randomizers init
    dml::test::random_t<uint64_t> random_pattern(seed);
    dml::test::random_t<uint64_t> random_vector_size(PATTERN_COUNT, seed);
    dml::test::random_t<uint64_t> random_byte_offset(MULTIPLY_BREAKER_RANGE, seed);

    // Vectors size presented as patters length and byte length
    const auto vector_size      = random_vector_size.get_next();
    const auto byte_vector_size = vector_size * sizeof(uint64_t);

    // Pattern to memory filling
    auto pattern = random_pattern.get_next();

    // Calculate mismatch parameters: mismatch offset inside pattern and mismatch value
    const uint32_t mismatch_pattern_offset          = random_byte_offset.get_next();
    const uint32_t expected_mismatch_pattern_offset = (vector_size / 2u) * sizeof(uint64_t);
    const uint32_t expected_mismatch_byte_offset    = expected_mismatch_pattern_offset + mismatch_pattern_offset;

    const uint64_t anti_pattern  = ~pattern;
    const auto     mismatch_byte = (uint8_t)(anti_pattern >> (mismatch_pattern_offset * BYTE_BIT_LENGTH));

    // Variables
    std::vector<uint64_t> test_vector(vector_size, pattern);
    auto *const           byte_vector_ptr = reinterpret_cast<uint8_t *>(test_vector.data());

    // Add mismatch
    byte_vector_ptr[expected_mismatch_byte_offset] = mismatch_byte;

    auto [mismatch, result] = dml::core::dispatch::compare_pattern(pattern,
                                                                   reinterpret_cast<const uint8_t *>(test_vector.data()),
                                                                   vector_size * sizeof(uint64_t));

    ASSERT_EQ(result, 1);
    ASSERT_GE(expected_mismatch_byte_offset, mismatch);
    ASSERT_GE(expected_mismatch_pattern_offset, mismatch);
}

CORE_TEST_REGISTER(compare_pattern_8u, ta_compare_pattern_partially_ne_not_aligned);

/**
 * @brief Test dmlc_compare_pattern_8u in case if memory is consistent with 8-byte pattern, except last byte
 */
auto ta_compare_pattern_last_byte_ne_aligned() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    // Randomizers init
    dml::test::random_t<uint64_t> random_pattern(seed);
    dml::test::random_t<uint64_t> random_vector_size(PATTERN_COUNT, seed);

    // Define test vector actual size(to fill) and size with delta(to test)
    const auto vector_size      = random_vector_size.get_next();
    const auto byte_vector_size = vector_size * sizeof(uint64_t);

    // Define mismatch offset position
    const uint32_t expected_mismatch_pattern_offset = (vector_size - 1u) * sizeof(uint64_t);
    const uint32_t expected_mismatch_byte_offset    = TEST_LAST_FROM(byte_vector_size);

    // Define fill pattern and last byte value
    const uint64_t pattern     = random_pattern.get_next();
    const uint64_t broken_byte = (~pattern) >> (sizeof(uint64_t) - 1u) * BYTE_BIT_LENGTH;

    // Variables
    std::vector<uint64_t> test_vector(vector_size, pattern);
    auto *const           byte_vector_ptr = reinterpret_cast<uint8_t *>(test_vector.data());

    // Vector preparation
    byte_vector_ptr[expected_mismatch_byte_offset] = broken_byte;

    auto [mismatch, result] = dml::core::dispatch::compare_pattern(pattern,
                                                                   reinterpret_cast<const uint8_t *>(test_vector.data()),
                                                                   vector_size * sizeof(uint64_t));

    ASSERT_EQ(result, 1);
    ASSERT_GE(expected_mismatch_byte_offset, mismatch);
    ASSERT_LE(expected_mismatch_pattern_offset, mismatch);
}

CORE_TEST_REGISTER(compare_pattern_8u, ta_compare_pattern_last_byte_ne_aligned);

/**
 * @brief Test dmlc_compare_pattern_8u in case if memory is consistent with 8-byte pattern, except last byte
 */
auto ta_compare_pattern_last_byte_ne_not_aligned() -> void
{
    // Seed
    const auto seed = test_system::get_seed();

    // Randomizers init
    dml::test::random_t<uint64_t> random_pattern(seed);
    dml::test::random_t<uint64_t> random_vector_size(PATTERN_COUNT, seed);
    dml::test::random_t<uint64_t> random_multiply_breaker(MULTIPLY_BREAKER_RANGE, seed);

    // Define test vector actual size(to fill) and size with delta(to test)
    const auto vector_size       = random_vector_size.get_next();
    const auto vector_size_delta = random_multiply_breaker.get_next();
    const auto byte_vector_size  = vector_size * sizeof(uint64_t) - vector_size_delta;

    // Define mismatch offset position
    const auto expected_mismatch_pattern_offset = (vector_size - 1u) * sizeof(uint64_t);
    const auto expected_mismatch_byte_offset    = TEST_LAST_FROM(byte_vector_size);

    // Define fill pattern
    const auto pattern = random_pattern.get_next();

    // Variables
    std::vector<uint64_t> test_vector(vector_size, pattern);
    auto *const           byte_vector_ptr = reinterpret_cast<uint8_t *>(test_vector.data());

    // Replace last byte
    byte_vector_ptr[expected_mismatch_byte_offset] = ~byte_vector_ptr[expected_mismatch_byte_offset];

    auto [mismatch, result] = dml::core::dispatch::compare_pattern(pattern,
                                                                   reinterpret_cast<const uint8_t *>(test_vector.data()),
                                                                   vector_size * sizeof(uint64_t));

    ASSERT_EQ(result, 1);
    ASSERT_GE(expected_mismatch_byte_offset, mismatch);
    ASSERT_LE(expected_mismatch_pattern_offset, mismatch);
}

CORE_TEST_REGISTER(compare_pattern_8u, ta_compare_pattern_last_byte_ne_not_aligned);
