/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contain Algorithmic tests for @ref dmlc_calculate_crc_reflected_32u
 * @date 3/5/2020
 *
 */

#include "t_common.hpp"
#include <optimization_dispatcher.hpp>


/**
 * @brief Tests a @ref dmlc_calculate_crc_reflected_32u function with predefined results
 */
auto ta_calculate_crc_reflected_32u_with_predefined_results() -> void
{
    using ustring = std::vector<uint8_t>;

    // Constants
    constexpr uint32_t number_of_tests = 5u;

    const ustring sources[number_of_tests] = {{0x8cu, 0x4cu, 0xccu, 0x2cu, 0xacu},
                                              {0xc6u, 0x4eu, 0xc6u, 0xfau, 0x2eu, 0xa6u, 0xceu, 0x2eu},
                                              {0xb4u, 0xb4u, 0xb4u, 0xb4u, 0xb4u, 0xb4u},
                                              {0x8cu, 0x4cu, 0xccu, 0x2cu, 0xacu, 0x6cu, 0xecu, 0x1cu, \
                                               0x9cu, 0xcu, 0x4cu, 0x8cu, 0xccu, 0x2cu, 0xacu, 0xccu, 0x6cu, 0x1cu},
                                              {0x26u, 0xceu, 0x86u, 0x7au, 0x36u, 0x96u, 0x46u, 0x4eu, 0x86u, 0x4eu, 0x9eu}};

    const std::array<uint32_t, number_of_tests> polynomials = {0x8005,
                                                               0x1021,
                                                               0xC867,
                                                               0x0589,
                                                               0x3D65 };

    const std::array<uint32_t, number_of_tests> initial_values = {0x0000,
                                                                  0xFFFF,
                                                                  0x1D0F,
                                                                  0x800D,
                                                                  0xB2AA};

    const std::array<uint32_t, number_of_tests> results = {0x2C6F9A81,
                                                           0x9DB44EEB,
                                                           0xC0547E6A,
                                                           0x4531409C,
                                                           0xB23A9F4C};

    for(uint32_t i = 0u; i < number_of_tests; i++)
    {
        auto crc_value = dml::core::dispatch::crc_reflected(reinterpret_cast<const uint8_t *>(sources[i].data()),
                                                  static_cast<uint32_t>(sources[i].size()),
                                                  initial_values[i],
                                                  polynomials[i]);

        EXPECT_EQ(results[i], crc_value);
    }
}

CORE_TEST_REGISTER(crc_reflected_32u, ta_calculate_crc_reflected_32u_with_predefined_results);
