/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contain Algorithmic tests for @ref dmlc_calculate_crc_32u
 *
 * @date 2/24/2020
 *
 */

#include <optimization_dispatcher.hpp>

#include "t_common.hpp"

/**
 * @brief Tests a @ref dmlc_calculate_crc_32u function with predefined results
 */
auto ta_calculate_crc_32u_with_predefined_results() -> void
{
    constexpr uint32_t number_of_tests = 4u;

    const std::array<std::string, number_of_tests> sources = { "12345", "crc_test", "------", "123456789021345368" };

    const std::array<uint32_t, std::size(sources)> polynomials    = { 0x8005, 0x1021, 0xC867, 0x0589 };
    const std::array<uint32_t, std::size(sources)> initial_values = { 0x0000, 0xFFFF, 0x1D0F, 0x800D };

    const std::array<uint32_t, std::size(sources)> results = { 0x2C6F9A81, 0x9DB44EEB, 0xC0547E6A, 0x4531409C };

    for (uint32_t i = 0u; i < number_of_tests; i++)
    {
        auto crc_value = dml::core::dispatch::crc(reinterpret_cast<const uint8_t *>(sources[i].data()),
                                                  static_cast<uint32_t>(sources[i].size()),
                                                  initial_values[i],
                                                  polynomials[i]);

        EXPECT_EQ(results[i], crc_value);
    }
}

CORE_TEST_REGISTER(crc_32u, ta_calculate_crc_32u_with_predefined_results);
