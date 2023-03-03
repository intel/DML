/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TEST_UTILS_DATA_GENERATORS_HPP
#define DML_TEST_UTILS_DATA_GENERATORS_HPP

#include <random>

namespace dml::testing
{
    class random_generator
    {
        static constexpr auto min = std::numeric_limits<uint8_t>::min();
        static constexpr auto max = std::numeric_limits<uint8_t>::max();

    public:
        explicit random_generator(uint32_t seed): gen_(seed), distr_(min, max) { }

        auto operator()() { return static_cast<uint8_t>(distr_(gen_)); }

    private:
        std::minstd_rand                gen_;
        std::uniform_int_distribution<> distr_;
    };

    class fill_generator
    {
        static constexpr auto min = std::numeric_limits<std ::uint8_t>::min();
        static constexpr auto max = std::numeric_limits<std ::uint8_t>::max();

    public:
        explicit fill_generator(uint64_t pattern)
        {
            auto p = reinterpret_cast<uint8_t *>(&pattern);
            auto i = int(0);
            for (auto &elem : pattern_)
            {
                elem = p[i++];
            }
        }

        auto operator()() noexcept { return pattern_[(current_index_ += 1) %= 8]; }

    private:
        uint8_t pattern_[8]{};
        int     current_index_{-1};
    };

}  // namespace dml::testing

#endif  //DML_TEST_UTILS_DATA_GENERATORS_HPP
