/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TEST_UTILS_CACHE_FLUSH_HPP
#define DML_TEST_UTILS_CACHE_FLUSH_HPP

#include <dml_test_utils/data_generators.hpp>
#include <dml_test_utils/data_storage.hpp>

#include "gtest/gtest.h"

namespace dml::testing
{
    struct cache_flush
    {
        explicit cache_flush(uint32_t seed, uint32_t length, uint32_t alignment = 64u):
            seed(seed), length(length), dst(length, alignment)
        {
            dst.generate(random_generator(seed));
        }

        const uint32_t seed;
        const uint32_t length;

        data_storage dst;
    };
}  // namespace dml::testing

#endif  //DML_TEST_UTILS_CACHE_FLUSH_HPP
