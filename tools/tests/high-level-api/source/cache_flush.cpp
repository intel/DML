/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml/dml.hpp>

#include <dml_test_utils/cache_flush.hpp>

#include "gtest/gtest.h"

#include "own/path.hpp"

DML_TESTING_HL_PARAMETRIZE(dmlhl_cache_flush);

TYPED_TEST(dmlhl_cache_flush, invalidate)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::cache_flush(seed, length);

    auto result = this->run(dml::cache_flush, dml::make_view(test_data.dst));

    ASSERT_EQ(result.status, dml::status_code::ok);
}

TYPED_TEST(dmlhl_cache_flush, dst_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::cache_flush(seed, length);

    auto result = this->run(dml::cache_flush, dml::make_view<uint8_t>(nullptr, test_data.length));

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_cache_flush, bad_size)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::cache_flush(seed, length);

    auto result = this->run(dml::cache_flush, dml::make_view(test_data.dst.data(), 0u));

    ASSERT_EQ(result.status, dml::status_code::bad_size);
}

TEST(dmlhl_cache_flush, bad_alloc)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::cache_flush(seed, length);

    ASSERT_ANY_THROW(auto result = dml::submit<dml::software>(dml::cache_flush,
                                                              dml::make_view(test_data.dst),
                                                              dml::execution_interface(
                                                                  [](auto &&task)
                                                                  {
                                                                      task();
                                                                  },
                                                                  exception_allocator<int>())));
}
