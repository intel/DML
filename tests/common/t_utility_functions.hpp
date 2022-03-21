/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_T_UTILITY_FUNCTIONS_HPP
#define DML_T_UTILITY_FUNCTIONS_HPP

#include <vector>
#include <cstdint>

namespace dml::test {
auto get_default_test_lengths() -> std::vector<uint32_t>;
}

#endif //DML_T_UTILITY_FUNCTIONS_HPP
