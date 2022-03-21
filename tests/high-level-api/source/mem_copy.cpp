/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include "gtest/gtest.h"

#include <dml_test_utils/mem_copy.hpp>

#include <dml/dml.hpp>

#include "own/path.hpp"

#include "t_utility_functions.hpp"

#include "t_test_case_generator.hpp"

#include "t_random_parameters.hpp"

DML_TESTING_HL_PARAMETRIZE(dmlhl_mem_copy);

TYPED_TEST(dmlhl_mem_copy, success)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    const auto seed = test_system::get_seed();
    auto test_cases = dml::test::test_case_generator<uint32_t>(dml::test::get_default_test_lengths);

    for (auto size : test_cases) {

        auto test_data = dml::testing::mem_copy(seed, size);

        auto result = this->run(dml::mem_copy, dml::make_view(test_data.src), dml::make_view(test_data.dst));

        ASSERT_EQ(result.status, dml::status_code::ok) << test_cases.info(size);

        ASSERT_TRUE(test_data.check()) << test_cases.info(size);
    }
}

TYPED_TEST(dmlhl_mem_copy, src_null)
{
    constexpr auto size = 16u;
    constexpr auto seed = 777u;

    auto test_data = dml::testing::mem_copy(seed, size);

    auto result = this->run(dml::mem_copy, dml::make_view<uint8_t>(nullptr, size), dml::make_view(test_data.dst));

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_mem_copy, dst_null)
{
    constexpr auto size = 16u;
    constexpr auto seed = 777u;

    auto test_data = dml::testing::mem_copy(seed, size);

    auto result = this->run(dml::mem_copy, dml::make_view(test_data.src), dml::make_view<uint8_t>(nullptr, size));

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_mem_copy, inconsistent_size)
{
    constexpr auto size = 16u;
    constexpr auto seed = 777u;

    auto test_data = dml::testing::mem_copy(seed, size);

    auto result = this->run(
        dml::mem_copy, dml::make_view(test_data.src.data(), test_data.length - 1u), dml::make_view(test_data.dst));

    ASSERT_EQ(result.status, dml::status_code::inconsistent_size);
}

TYPED_TEST(dmlhl_mem_copy, bad_size)
{
    constexpr auto size = 16u;
    constexpr auto seed = 777u;

    auto test_data = dml::testing::mem_copy(seed, size);

    auto result =
        this->run(dml::mem_copy, dml::make_view(test_data.src.data(), 0u), dml::make_view(test_data.dst.data(), 0u));

    ASSERT_EQ(result.status, dml::status_code::bad_size);
}

TEST(dmlhl_mem_copy, bad_alloc)
{
    constexpr auto size = 16u;
    constexpr auto seed = 777u;

    auto test_data = dml::testing::mem_copy(seed, size);

    ASSERT_ANY_THROW(auto result = dml::submit<dml::software>(dml::mem_copy,
                                                              dml::make_view(test_data.src),
                                                              dml::make_view(test_data.dst),
                                                              dml::execution_interface(
                                                                  [](auto &&task)
                                                                  {
                                                                      task();
                                                                  },
                                                                  exception_allocator<int>())));
}
