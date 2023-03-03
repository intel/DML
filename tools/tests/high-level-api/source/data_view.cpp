/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include "gtest/gtest.h"

#include <dml_test_utils/mem_move.hpp>

#include <dml/dml.hpp>

#include "own/path.hpp"

/**
 * @brief Random struct for testing purposes.
 */
struct testing_struct
{
    int a; /**< Some field 1 */
    int b; /**< Some field 2 */

    /**
     * @brief Equality operation for testing purposes
     *
     * @param rhs Other structure instance
     *
     * @return Boolean result of comparison
     */
    bool operator==(const testing_struct &rhs) const { return std::tie(a, b) == std::tie(rhs.a, rhs.b); }

    /**
     * @brief Non-Equality operation for testing purposes
     *
     * @param rhs Other structure instance
     *
     * @return Boolean result of comparison
     */
    bool operator!=(const testing_struct &rhs) const { return !(rhs == *this); }
};

template <typename T>
class dmlhl_data_view: public ::testing::Test
{
};

using testing_types = ::testing::Types<char, int, float, testing_struct>;
TYPED_TEST_SUITE(dmlhl_data_view, testing_types);

TYPED_TEST(dmlhl_data_view, pointer_and_size)
{
    constexpr auto length = 16u;

    const auto src = std::vector<TypeParam>(length, TypeParam{1});
    auto dst = std::vector<TypeParam>(length);

    auto result = dml::execute<dml::software>(
        dml::mem_move, dml::make_view(src.data(), src.size()), dml::make_view(dst.data(), dst.size()));

    ASSERT_EQ(result.status, dml::status_code::ok);

    ASSERT_EQ(src, dst);
}

TYPED_TEST(dmlhl_data_view, iterators)
{
    constexpr auto length = 16u;

    const auto src = std::vector<TypeParam>(length, TypeParam{1});
    auto dst = std::vector<TypeParam>(length);

    auto result = dml::execute<dml::software>(
        dml::mem_move, dml::make_view(src.begin(), src.end()), dml::make_view(dst.begin(), dst.end()));

    ASSERT_EQ(result.status, dml::status_code::ok);

    ASSERT_EQ(src, dst);
}

TYPED_TEST(dmlhl_data_view, range)
{
    constexpr auto length = 16u;

    const auto src = std::vector<TypeParam>(length, TypeParam{1});
    auto dst = std::vector<TypeParam>(length);

    auto result = dml::execute<dml::software>(dml::mem_move, dml::make_view(src), dml::make_view(dst));

    ASSERT_EQ(result.status, dml::status_code::ok);

    ASSERT_EQ(src, dst);
}
