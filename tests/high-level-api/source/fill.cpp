/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include "gtest/gtest.h"

#include <dml_test_utils/fill.hpp>

#include <dml/dml.hpp>

#include "own/path.hpp"

#include "t_utility_functions.hpp"

#include "t_test_case_generator.hpp"

#include "t_random_parameters.hpp"

DML_TESTING_HL_PARAMETRIZE(dmlhl_fill);

DML_MT_TESTING_HL_PARAMETRIZE(dmlhl_fill_mt);

TYPED_TEST(dmlhl_fill_mt, success) {
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto size = 16u;
    constexpr auto seed = 777u;

    using allocator_t = typename dml::default_execution_interface<typename TestFixture::execution_path>::allocator_type;
    using result_t = dml::handler<dml::fill_operation, allocator_t>;

    this->run([=](auto thread_count) {
        std::vector<dml::testing::fill> data;
        std::vector<result_t>           results;

        data.reserve(thread_count);
        results.reserve(thread_count);

        for (uint32_t i = 0u; i < thread_count; i++) {
            data.emplace_back(seed, size);
        }

        for (uint32_t i = 0u; i < thread_count; i++) {
            results.push_back(dml::submit<typename TestFixture::execution_path>(dml::fill,
                                                                                data[i].pattern,
                                                                                dml::make_view(data[i].dst)));
        }

        for (uint32_t i = 0u; i < thread_count; i++) {
            auto result = results[i].get();

            ASSERT_EQ(result.status, dml::status_code::ok);
            ASSERT_TRUE(data[i].check());
        }
    });
}

TYPED_TEST(dmlhl_fill, success) {
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    const auto seed = test_system::get_seed();
    auto test_cases = dml::test::test_case_generator<uint32_t>(dml::test::get_default_test_lengths);

    for (auto length : test_cases) {

        auto test_data = dml::testing::fill(seed, length);
        auto result = this->run(dml::fill, test_data.pattern, dml::make_view(test_data.dst));

        ASSERT_EQ(result.status, dml::status_code::ok) << test_cases.info(length);

        ASSERT_TRUE(test_data.check()) << test_cases.info(length);
    }
}

TYPED_TEST(dmlhl_fill, null) {
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::fill(seed, length);
    auto result    = this->run(dml::fill, test_data.pattern, dml::make_view<std::uint8_t>(nullptr, test_data.length));

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_fill, bad_size) {
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::fill(seed, length);
    auto result    = this->run(dml::fill, test_data.pattern, dml::make_view(test_data.dst.data(), 0u));

    ASSERT_EQ(result.status, dml::status_code::bad_size);
}

TEST(dmlhl_fill, bad_alloc) {
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto                  test_data = dml::testing::fill(seed, length);
    ASSERT_ANY_THROW(auto result    = dml::submit<dml::software>(dml::fill,
                                                                 test_data.pattern,
                                                                 dml::make_view(test_data.dst),
                                                                 dml::execution_interface(
                                                                         [](auto &&task) {
                                                                             task();
                                                                         },
                             exception_allocator<int>())));
}
