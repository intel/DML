/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TEST_UTILS_COPY_CRC_HPP
#define DML_TEST_UTILS_COPY_CRC_HPP

#include <dml_test_utils/data_generators.hpp>
#include <dml_test_utils/data_storage.hpp>

namespace dml::testing
{
    struct copy_crc
    {
        explicit copy_crc(uint32_t seed, uint32_t length, uint32_t alignment = 64u):
            seed(seed), length(length), src(length, alignment), dst(length, alignment), crc_seed(0)
        {
            src.generate(random_generator(seed));
            dst.generate(random_generator(seed + 1));
        }

        [[nodiscard]] bool check() const noexcept
        {
            return src == dst;
        }

        const uint32_t seed;
        const uint32_t length;

        data_storage src;
        data_storage dst;
        uint32_t crc_seed;
    };
}  // namespace dml::testing

#endif  //DML_TEST_UTILS_COPY_CRC_HPP
