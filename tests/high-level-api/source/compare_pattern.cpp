/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml/dml.hpp>

#include <dml_test_utils/compare_pattern.hpp>

#include "gtest/gtest.h"

#include "own/path.hpp"

DML_TESTING_HL_PARAMETRIZE(dmlhl_compare_pattern);

TYPED_TEST(dmlhl_compare_pattern, equal)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_pattern_equal(seed, length);

    auto result = this->run(dml::compare_pattern, test_data.pattern, dml::make_view(test_data.src));

    ASSERT_EQ(result.status, dml::status_code::ok);
    ASSERT_EQ(result.result, dml::comparison_result::equal);
    ASSERT_EQ(result.mismatch, 0u);
}

TYPED_TEST(dmlhl_compare_pattern, equal_expect_equal)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_pattern_equal(seed, length);

    auto result = this->run(dml::compare_pattern.expect_equal(), test_data.pattern, dml::make_view(test_data.src));

    ASSERT_EQ(result.status, dml::status_code::ok);
    ASSERT_EQ(result.result, dml::comparison_result::equal);
    ASSERT_EQ(result.mismatch, 0u);
}

TYPED_TEST(dmlhl_compare_pattern, equal_expect_not_equal)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_pattern_equal(seed, length);

    auto result = this->run(dml::compare_pattern.expect_not_equal(), test_data.pattern, dml::make_view(test_data.src));

    ASSERT_EQ(result.status, dml::status_code::false_predicate);
    ASSERT_EQ(result.result, dml::comparison_result::equal);
    ASSERT_EQ(result.mismatch, 0u);
}

TYPED_TEST(dmlhl_compare_pattern, not_equal)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_pattern_not_equal(seed, length);

    auto result = this->run(dml::compare_pattern, test_data.pattern, dml::make_view(test_data.src));

    ASSERT_EQ(result.status, dml::status_code::ok);
    ASSERT_EQ(result.result, dml::comparison_result::not_equal);
    ASSERT_EQ(result.mismatch, test_data.mismatch);
}

TYPED_TEST(dmlhl_compare_pattern, not_equal_expect_equal)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_pattern_not_equal(seed, length);

    auto result = this->run(dml::compare_pattern.expect_equal(), test_data.pattern, dml::make_view(test_data.src));

    ASSERT_EQ(result.status, dml::status_code::false_predicate);
    ASSERT_EQ(result.result, dml::comparison_result::not_equal);
    ASSERT_EQ(result.mismatch, test_data.mismatch);
}

TYPED_TEST(dmlhl_compare_pattern, not_equal_expect_not_equal)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_pattern_not_equal(seed, length);

    auto result = this->run(dml::compare_pattern.expect_not_equal(), test_data.pattern, dml::make_view(test_data.src));

    ASSERT_EQ(result.status, dml::status_code::ok);
    ASSERT_EQ(result.result, dml::comparison_result::not_equal);
    ASSERT_EQ(result.mismatch, test_data.mismatch);
}

TYPED_TEST(dmlhl_compare_pattern, src_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_pattern_equal(seed, length);

    auto result =
        this->run(dml::compare_pattern, test_data.pattern, dml::make_view<uint8_t>(nullptr, test_data.length));

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_compare_pattern, bad_size)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_pattern_equal(seed, length);

    auto result = this->run(dml::compare_pattern, test_data.pattern, dml::make_view(test_data.src.data(), 0u));

    ASSERT_EQ(result.status, dml::status_code::bad_size);
}

TEST(dmlhl_compare_pattern, bad_alloc)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_pattern_equal(seed, length);

    ASSERT_ANY_THROW(auto result = dml::submit<dml::software>(dml::compare_pattern,
                                                              test_data.pattern,
                                                              dml::make_view(test_data.src),
                                                              dml::execution_interface(
                                                                  [](auto &&task)
                                                                  {
                                                                      task();
                                                                  },
                                                                  exception_allocator<int>())));
}
