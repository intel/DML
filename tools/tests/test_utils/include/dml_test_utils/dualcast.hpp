/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 * Copyright (C) 2023 Hamish Nicholson
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TEST_UTILS_DUALCAST_HPP
#define DML_TEST_UTILS_DUALCAST_HPP

#include <dml_test_utils/data_generators.hpp>
#include <dml_test_utils/data_storage.hpp>

#include "gtest/gtest.h"

namespace dml::testing
{
    struct dualcast
    {
        static constexpr auto padding = 0x1000;

        explicit dualcast(uint32_t seed, uint32_t length, uint32_t alignment = 64u):
            seed(seed), length(length), src(length, alignment), dst_memory_(length + padding, alignment)
        {
            src.generate(random_generator(seed));
            dst_memory_.generate(random_generator(seed + 1));
            dst1 = dst_memory_.data();
            dst2 = dst_memory_.data() + padding;
        }

        [[nodiscard]] bool check() const noexcept
        {
            auto res1 = std::vector<uint8_t>(src.begin(), src.end()) == std::vector<uint8_t>(dst1, dst1 + length);
            auto res2 = std::vector<uint8_t>(src.begin(), src.end()) == std::vector<uint8_t>(dst2, dst2 + length);

            return res1 && res2;
        }

        const uint32_t seed;
        const uint32_t length;

        data_storage src;
        uint8_t *    dst1;
        uint8_t *    dst2;

    private:
        data_storage dst_memory_;
    };
}  // namespace dml::testing

#endif  //DML_TEST_UTILS_DUALCAST_HPP
