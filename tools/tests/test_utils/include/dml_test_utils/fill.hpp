/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TEST_UTILS_FILL_HPP
#define DML_TEST_UTILS_FILL_HPP

#include <dml_test_utils/data_generators.hpp>
#include <dml_test_utils/data_storage.hpp>

#include "gtest/gtest.h"

namespace dml::testing
{
    struct fill
    {
        explicit fill(uint32_t seed, uint32_t length, uint32_t alignment = 64u):
            seed(seed), length(length), pattern(0xFFFFDEADFFFFDEAD), dst(length, alignment), ref(length, alignment)
        {
            dst.generate(random_generator(seed));
            ref.generate(fill_generator(pattern));
        }

        [[nodiscard]] bool check() const noexcept
        {
            return ref == dst;
        }

        const uint32_t seed;
        const uint32_t length;

        uint64_t     pattern;
        data_storage dst;

    private:
        data_storage ref;
    };
}  // namespace dml::testing

#endif  //DML_TEST_UTILS_FILL_HPP
