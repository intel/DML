/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TEST_UTILS_DELTA_HPP
#define DML_TEST_UTILS_DELTA_HPP

#include <dml_test_utils/data_generators.hpp>
#include <dml_test_utils/data_storage.hpp>

#include "gtest/gtest.h"

namespace dml::testing
{
    struct delta
    {
        explicit delta(uint32_t seed, uint32_t length, uint32_t alignment = 64u):
            seed(seed),
            length(length),
            src1(length, alignment),
            src2(length, alignment),
            delta_record(((length * 10u / 8u) < 80u) ? 80u : length * 10u / 8u, alignment)
        {
            src1.generate(random_generator(seed));
            src2.generate(random_generator(seed + 1));
            delta_record.generate(random_generator(seed + 2));
        }

        [[nodiscard]] bool check() const noexcept
        {
            return src1 == src2;
        }

        const uint32_t seed;
        const uint32_t length;

        data_storage src1;
        data_storage src2;
        data_storage delta_record;
    };
}  // namespace dml::testing

#endif  //DML_TEST_UTILS_DELTA_HPP
