/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TEST_UTILS_COMPARE_HPP
#define DML_TEST_UTILS_COMPARE_HPP

#include <dml_test_utils/data_generators.hpp>
#include <dml_test_utils/data_storage.hpp>

#include "gtest/gtest.h"

namespace dml::testing
{
    struct compare_equal
    {
        explicit compare_equal(uint32_t seed, uint32_t length, uint32_t alignment = 64u):
            seed(seed), length(length), src1(length, alignment), src2(length, alignment)
        {
            src1.generate(random_generator(seed));
            src2 = src1;
        }

        const uint32_t seed;
        const uint32_t length;

        data_storage src1;
        data_storage src2;
    };

    struct compare_not_equal
    {
        explicit compare_not_equal(uint32_t seed, uint32_t length, uint32_t alignment = 64u):
            seed(seed), length(length), src1(length, alignment), src2(length, alignment), mismatch()
        {
            src1.generate(random_generator(seed));
            src2.generate(random_generator(seed + 1));

            for (uint32_t i = 0; i < length; ++i)
            {
                if (src1.at(i) != src2.at(i))
                {
                    mismatch = i;
                    break;
                }
            }
        }

        const uint32_t seed;
        const uint32_t length;

        data_storage src1;
        data_storage src2;
        uint32_t mismatch;
    };
}  // namespace dml::testing

#endif  //DML_TEST_UTILS_COMPARE_HPP
