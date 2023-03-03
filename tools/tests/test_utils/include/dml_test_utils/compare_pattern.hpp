/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TEST_UTILS_COMPARE_PATTERN_HPP
#define DML_TEST_UTILS_COMPARE_PATTERN_HPP

#include <dml_test_utils/data_generators.hpp>
#include <dml_test_utils/data_storage.hpp>

#include "gtest/gtest.h"

namespace dml::testing
{
    struct compare_pattern_equal
    {
        explicit compare_pattern_equal(uint32_t seed, uint32_t length, uint32_t alignment = 64u):
            seed(seed), length(length), pattern(0xFFFFABCDFFFFABCD), src(length, alignment)
        {
            src.generate(fill_generator(pattern));
        }

        const uint32_t seed;
        const uint32_t length;

        uint64_t pattern;
        data_storage src;
    };

    struct compare_pattern_not_equal
    {
        explicit compare_pattern_not_equal(uint32_t seed, uint32_t length, uint32_t alignment = 64u):
        seed(seed), length(length), pattern(0xFFFFABCDFFFFABCD), src(length, alignment), mismatch()
        {
            src.generate(fill_generator(pattern));

            mismatch = random_generator(seed)() % (length / 8);
            src.at(mismatch)++;
        }

        const uint32_t seed;
        const uint32_t length;

        uint64_t pattern;
        data_storage src;
        uint32_t mismatch;
    };
}  // namespace dml::testing

#endif  //DML_TEST_UTILS_COMPARE_PATTERN_HPP
