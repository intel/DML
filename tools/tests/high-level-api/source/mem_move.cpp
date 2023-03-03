/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include "gtest/gtest.h"

#include <dml_test_utils/mem_move.hpp>

#include <dml/dml.hpp>

#include "own/path.hpp"

#include "t_utility_functions.hpp"

#include "t_test_case_generator.hpp"

#include "t_random_parameters.hpp"

DML_TESTING_HL_PARAMETRIZE(dmlhl_mem_move);

DML_MT_TESTING_HL_PARAMETRIZE(dmlhl_mem_move_mt);

TYPED_TEST(dmlhl_mem_move_mt, success) {
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto size = 16u;
    constexpr auto seed = 777u;

    using allocator_t = typename dml::default_execution_interface<typename TestFixture::execution_path>::allocator_type;
    using result_t = dml::handler<dml::mem_move_operation, allocator_t>;

    this->run([=](auto thread_count) {
        std::vector<dml::testing::mem_move> data;
        std::vector<result_t>               results;

        data.reserve(thread_count);
        results.reserve(thread_count);

        for (uint32_t i = 0u; i < thread_count; i++) {
            data.emplace_back(seed, size);
        }

        for (uint32_t i = 0u; i < thread_count; i++) {
            results.push_back(dml::submit<typename TestFixture::execution_path>(dml::mem_move,
                                                                                dml::make_view(data[i].src),
                                                                                dml::make_view(data[i].dst)));
        }

        for (uint32_t i = 0u; i < thread_count; i++) {
            auto result = results[i].get();

            ASSERT_EQ(result.status, dml::status_code::ok);

            ASSERT_TRUE(data[i].check());
        }
    });
}

TYPED_TEST(dmlhl_mem_move, success) {
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    const auto seed = test_system::get_seed();
    auto test_cases = dml::test::test_case_generator<uint32_t>(dml::test::get_default_test_lengths);

    for (auto size: test_cases) {

        auto test_data = dml::testing::mem_move(seed, size);

        auto result = this->run(dml::mem_move, dml::make_view(test_data.src), dml::make_view(test_data.dst));

        ASSERT_EQ(result.status, dml::status_code::ok) << test_cases.info(size);

        ASSERT_TRUE(test_data.check()) << test_cases.info(size);
    }
}

TYPED_TEST(dmlhl_mem_move, src_null) {
    constexpr auto size = 16u;
    constexpr auto seed = 777u;

    auto test_data = dml::testing::mem_move(seed, size);

    auto result = this->run(dml::mem_move, dml::make_view<uint8_t>(nullptr, size), dml::make_view(test_data.dst));

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_mem_move, dst_null) {
    constexpr auto size = 16u;
    constexpr auto seed = 777u;

    auto test_data = dml::testing::mem_move(seed, size);

    auto result = this->run(dml::mem_move, dml::make_view(test_data.src), dml::make_view<uint8_t>(nullptr, size));

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_mem_move, inconsistent_size) {
    constexpr auto size = 16u;
    constexpr auto seed = 777u;

    auto test_data = dml::testing::mem_move(seed, size);

    auto result = this->run(
            dml::mem_move, dml::make_view(test_data.src.data(), test_data.length - 1u), dml::make_view(test_data.dst));

    ASSERT_EQ(result.status, dml::status_code::inconsistent_size);
}

TYPED_TEST(dmlhl_mem_move, bad_size) {
    constexpr auto size = 16u;
    constexpr auto seed = 777u;

    auto test_data = dml::testing::mem_move(seed, size);

    auto result = this->run(dml::mem_move,
                            dml::make_view(test_data.src.data(), 0u),
                            dml::make_view(test_data.dst.data(), 0u));

    ASSERT_EQ(result.status, dml::status_code::bad_size);
}

TEST(dmlhl_mem_move, bad_alloc) {
    constexpr auto size = 16u;
    constexpr auto seed = 777u;

    auto test_data = dml::testing::mem_move(seed, size);

    ASSERT_ANY_THROW(auto result = dml::submit<dml::software>(dml::mem_move,
                                                              dml::make_view(test_data.src),
                                                              dml::make_view(test_data.dst),
                                                              dml::execution_interface(
                                                                      [](auto &&task) {
                                                                          task();
                                                                      },
                             exception_allocator<int>())));
}
