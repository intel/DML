/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml/dml.hpp>

#include <dml_test_utils/crc.hpp>

#include "gtest/gtest.h"

#include "own/path.hpp"

#include <ref_crc.hpp>

#include "t_utility_functions.hpp"

#include "t_test_case_generator.hpp"

#include "t_random_parameters.hpp"

DML_TESTING_HL_PARAMETRIZE(dmlhl_crc);

DML_MT_TESTING_HL_PARAMETRIZE(dmlhl_crc_mt);

TYPED_TEST(dmlhl_crc, no_flags) {
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    const auto seed       = test_system::get_seed();
    auto       test_cases = dml::test::test_case_generator<uint32_t>(dml::test::get_default_test_lengths);

    for (auto length: test_cases) {

        auto test_data = dml::testing::crc(seed, length);

        auto result = this->run(dml::crc, dml::make_view(test_data.src), test_data.crc_seed);

        ASSERT_EQ(result.status, dml::status_code::ok) << test_cases.info(length);

        const auto ref = dml::reference::calculate_crc<uint32_t, 0>(
                test_data.src.data(), test_data.src.data() + length, test_data.crc_seed);

        ASSERT_EQ(result.crc_value, ref) << test_cases.info(length);
    }
}

TYPED_TEST(dmlhl_crc_mt, no_flags) {
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    using allocator_t = typename dml::default_execution_interface<typename TestFixture::execution_path>::allocator_type;
    using result_t = dml::handler<dml::crc_operation, allocator_t>;

    const auto seed       = test_system::get_seed();
    auto       test_cases = dml::test::test_case_generator<uint32_t>(dml::test::get_default_test_lengths);

    for (auto length: test_cases) {
        this->run([seed, &length, &test_cases](uint32_t thread_count) {
            std::vector<dml::testing::crc> data;
            std::vector<result_t>          results;

            data.reserve(thread_count);
            results.reserve(thread_count);

            for (uint32_t i = 0; i < thread_count; i++) {
                data.emplace_back(seed, length);
            }

            for (uint32_t i = 0; i < thread_count; i++) {
                results.push_back(dml::submit<typename TestFixture::execution_path>(dml::crc,
                                                                                    dml::make_view(data[i].src),
                                                                                    data[i].crc_seed));
            }

            for (uint32_t i = 0; i < thread_count; i++) {
                auto result = results[i].get();

                ASSERT_EQ(result.status, dml::status_code::ok) << test_cases.info(length);

                const auto ref = dml::reference::calculate_crc<uint32_t, 0>(
                        data[i].src.data(), data[i].src.data() + length, data[i].crc_seed);

                ASSERT_EQ(result.crc_value, ref) << test_cases.info(length);
            }
        });
    }
}

TYPED_TEST(dmlhl_crc, bypass_reflection) {
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    const auto seed       = test_system::get_seed();
    auto       test_cases = dml::test::test_case_generator<uint32_t>(dml::test::get_default_test_lengths);

    for (auto length : test_cases) {

        auto test_data = dml::testing::crc(seed, length);

        auto result = this->run(dml::crc.bypass_reflection(), dml::make_view(test_data.src), test_data.crc_seed);

        ASSERT_EQ(result.status, dml::status_code::ok) << test_cases.info(length);

        const auto ref = dml::reference::calculate_crc<uint32_t, DML_FLAG_CRC_BYPASS_REFLECTION>(
                test_data.src.data(), test_data.src.data() + length, test_data.crc_seed);

        ASSERT_EQ(result.crc_value, ref) << test_cases.info(length);
    }
}

TYPED_TEST(dmlhl_crc, bypass_data_reflection) {
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    const auto seed       = test_system::get_seed();
    auto       test_cases = dml::test::test_case_generator<uint32_t>(dml::test::get_default_test_lengths);

    for (auto length : test_cases) {

        auto test_data = dml::testing::crc(seed, length);

        auto result = this->run(dml::crc.bypass_data_reflection(), dml::make_view(test_data.src), test_data.crc_seed);

        ASSERT_EQ(result.status, dml::status_code::ok) << test_cases.info(length);

        const auto ref = dml::reference::calculate_crc<uint32_t, DML_FLAG_CRC_BYPASS_DATA_REFLECTION>(
                test_data.src.data(), test_data.src.data() + length, test_data.crc_seed);

        ASSERT_EQ(result.crc_value, ref) << test_cases.info(length);
    }
}

TYPED_TEST(dmlhl_crc, all_flags) {
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    const auto seed       = test_system::get_seed();
    auto       test_cases = dml::test::test_case_generator<uint32_t>(dml::test::get_default_test_lengths);

    for (auto length : test_cases) {

        auto test_data = dml::testing::crc(seed, length);

        auto result = this->run(
                dml::crc.bypass_reflection().bypass_data_reflection(), dml::make_view(test_data.src),
                test_data.crc_seed);

        ASSERT_EQ(result.status, dml::status_code::ok) << test_cases.info(length);

        const auto ref =
                           dml::reference::calculate_crc<uint32_t,
                                   DML_FLAG_CRC_BYPASS_REFLECTION | DML_FLAG_CRC_BYPASS_DATA_REFLECTION>(
                                   test_data.src.data(), test_data.src.data() + length, test_data.crc_seed);

        ASSERT_EQ(result.crc_value, ref) << test_cases.info(length);
    }
}

TYPED_TEST(dmlhl_crc, src_null) {
    constexpr auto length = 16u;
    const auto     seed   = test_system::get_seed();;

    auto test_data = dml::testing::crc(seed, length);

    auto result = this->run(dml::crc, dml::make_view<uint8_t>(nullptr, test_data.length), test_data.crc_seed);

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_crc, bad_size) {
    constexpr auto length = 16u;
    const auto     seed   = test_system::get_seed();

    auto test_data = dml::testing::crc(seed, length);

    auto result = this->run(dml::crc, dml::make_view<uint8_t>(test_data.src.data(), 0u), test_data.crc_seed);

    ASSERT_EQ(result.status, dml::status_code::bad_size);
}

TEST(dmlhl_crc, bad_alloc) {
    constexpr auto length = 16u;
    const auto     seed   = test_system::get_seed();;

    auto test_data = dml::testing::crc(seed, length);

    ASSERT_ANY_THROW(auto result = dml::submit<dml::software>(dml::crc,
                                                              dml::make_view(test_data.src),
                                                              test_data.crc_seed,
                                                              dml::execution_interface(
                                                                      [](auto &&task) {
                                                                          task();
                                                                      },
                             exception_allocator<int>())));
}
