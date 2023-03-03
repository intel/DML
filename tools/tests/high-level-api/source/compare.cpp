/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml/dml.hpp>

#include <dml_test_utils/compare.hpp>

#include "gtest/gtest.h"

#include "own/path.hpp"

DML_TESTING_HL_PARAMETRIZE(dmlhl_compare);

TYPED_TEST(dmlhl_compare, equal)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_equal(seed, length);

    auto result = this->run(dml::compare, dml::make_view(test_data.src1), dml::make_view(test_data.src2));

    ASSERT_EQ(result.status, dml::status_code::ok);
    ASSERT_EQ(result.result, dml::comparison_result::equal);
    ASSERT_EQ(result.mismatch, 0u);
}

TYPED_TEST(dmlhl_compare, equal_expect_equal)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_equal(seed, length);

    auto result =
        this->run(dml::compare.expect_equal(), dml::make_view(test_data.src1), dml::make_view(test_data.src2));

    ASSERT_EQ(result.status, dml::status_code::ok);
    ASSERT_EQ(result.result, dml::comparison_result::equal);
    ASSERT_EQ(result.mismatch, 0u);
}

TYPED_TEST(dmlhl_compare, equal_expect_not_equal)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_equal(seed, length);

    auto result =
        this->run(dml::compare.expect_not_equal(), dml::make_view(test_data.src1), dml::make_view(test_data.src2));

    ASSERT_EQ(result.status, dml::status_code::false_predicate);
    ASSERT_EQ(result.result, dml::comparison_result::equal);
    ASSERT_EQ(result.mismatch, 0u);
}

TYPED_TEST(dmlhl_compare, not_equal)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_not_equal(seed, length);

    auto result = this->run(dml::compare, dml::make_view(test_data.src1), dml::make_view(test_data.src2));

    ASSERT_EQ(result.status, dml::status_code::ok);
    ASSERT_EQ(result.result, dml::comparison_result::not_equal);
    ASSERT_EQ(result.mismatch, test_data.mismatch);
}

TYPED_TEST(dmlhl_compare, not_equal_expect_equal)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_not_equal(seed, length);

    auto result =
        this->run(dml::compare.expect_equal(), dml::make_view(test_data.src1), dml::make_view(test_data.src2));

    ASSERT_EQ(result.status, dml::status_code::false_predicate);
    ASSERT_EQ(result.result, dml::comparison_result::not_equal);
    ASSERT_EQ(result.mismatch, test_data.mismatch);
}

TYPED_TEST(dmlhl_compare, not_equal_expect_not_equal)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_not_equal(seed, length);

    auto result =
        this->run(dml::compare.expect_not_equal(), dml::make_view(test_data.src1), dml::make_view(test_data.src2));

    ASSERT_EQ(result.status, dml::status_code::ok);
    ASSERT_EQ(result.result, dml::comparison_result::not_equal);
    ASSERT_EQ(result.mismatch, test_data.mismatch);
}

TYPED_TEST(dmlhl_compare, src1_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_equal(seed, length);

    auto result =
        this->run(dml::compare, dml::make_view<uint8_t>(nullptr, test_data.length), dml::make_view(test_data.src2));

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_compare, src2_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_equal(seed, length);

    auto result =
        this->run(dml::compare, dml::make_view(test_data.src1), dml::make_view<uint8_t>(nullptr, test_data.length));

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_compare, inconsistent_size)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_equal(seed, length);

    auto result = this->run(
        dml::compare, dml::make_view(test_data.src1), dml::make_view(test_data.src2.data(), test_data.length - 1u));

    ASSERT_EQ(result.status, dml::status_code::inconsistent_size);
}

TYPED_TEST(dmlhl_compare, bad_size)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_equal(seed, length);

    auto result =
        this->run(dml::compare, dml::make_view(test_data.src1.data(), 0u), dml::make_view(test_data.src2.data(), 0u));

    ASSERT_EQ(result.status, dml::status_code::bad_size);
}

TYPED_TEST(dmlhl_compare, bad_alloc)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_equal(seed, length);

    ASSERT_ANY_THROW(auto result = dml::submit<dml::software>(dml::compare,
                                                              dml::make_view(test_data.src1),
                                                              dml::make_view(test_data.src2),
                                                              dml::execution_interface(
                                                                  [](auto &&task)
                                                                  {
                                                                      task();
                                                                  },
                                                                  exception_allocator<int>())));
}
