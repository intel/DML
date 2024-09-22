/*******************************************************************************
 * Copyright (C) 2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_UTIL_HPP
#define DML_TESTING_UTIL_HPP

#include "gtest/gtest.h"

namespace dml::test {

// Skips entire test for specified path, will also skip all remaining test cases
#define DML_SKIP_TEST_FOR(path) \
    if (dml::test::variables_t::path == (path)) GTEST_SKIP()

// Skips entire test for specified path, will also skip all remaining test cases
// Prints a message w/ skipped test
#define DML_SKIP_TEST_FOR_VERBOSE(path, message) \
    if (dml::test::variables_t::path == (path)) GTEST_SKIP_(message)

// Will skip entire test case if expr evaluates to true, will also skip all remaining test cases
// Prints a message w/ skipped testcase
#define DML_SKIP_TEST_FOR_EXPR_VERBOSE(expr, message) \
    if (expr) { GTEST_SKIP_(message); }

// Will skip a specific test case, will continue with running other test cases in test
// Prints a message w/ skipped testcase
#define DML_SKIP_TC_TEST(expr, message)                          \
    if (expr) {                                                  \
        std::cout << "skipped test case: " << (message) << '\n'; \
        return;                                                  \
    }

} // namespace dml::test

#endif // DML_TESTING_UTIL_HPP
