/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include "t_utility_functions.hpp"

namespace dml::test {
auto get_default_test_lengths() -> std::vector<uint32_t> {
    std::vector<uint32_t> result;

    for (uint32_t i = 1; i < 128; i++) {
        result.push_back(i);
    }

    for (size_t i = 7; i <= 15; i++) {
        auto leftIndex = 1 << i;
        auto rightIndex = leftIndex << 1;
        result.push_back((rightIndex + leftIndex) >> 1);
    }

    return result;
}
}
