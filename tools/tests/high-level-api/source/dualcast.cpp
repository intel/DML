/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml/dml.hpp>

#include "gtest/gtest.h"

#include <dml_test_utils/dualcast.hpp>

#include "own/path.hpp"

DML_TESTING_HL_PARAMETRIZE(dmlhl_dualcast);

DML_MT_TESTING_HL_PARAMETRIZE(dmlhl_dualcast_mt);

TYPED_TEST(dmlhl_dualcast_mt, success) {
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto size = 16u;
    constexpr auto seed = 777u;

    using allocator_t = typename dml::default_execution_interface<typename TestFixture::execution_path>::allocator_type;
    using result_t = dml::handler<dml::dualcast_operation, allocator_t>;

    this->run([=](auto thread_count) {
        std::vector<dml::testing::dualcast> data;
        std::vector<result_t>               results;

        data.reserve(thread_count);
        results.reserve(thread_count);

        for (uint32_t i = 0u; i < thread_count; i++) {
            data.emplace_back(seed, size);
        }

        for (uint32_t i = 0u; i < thread_count; i++) {
            results.push_back(
                    dml::submit<typename TestFixture::execution_path>(dml::dualcast,
                                                                      dml::make_view(data[i].src),
                                                                      dml::make_view(data[i].dst1, data[i].length),
                                                                      dml::make_view(data[i].dst2, data[i].length)));
        }

        for (uint32_t i = 0u; i < thread_count; i++) {
            auto result = results[i].get();

            ASSERT_EQ(result.status, dml::status_code::ok);
            ASSERT_TRUE(data[i].check());
        }
    });
}

TYPED_TEST(dmlhl_dualcast, success) {
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::dualcast(seed, length);

    auto result = this->run(dml::dualcast,
                            dml::make_view(test_data.src),
                            dml::make_view(test_data.dst1, test_data.length),
                            dml::make_view(test_data.dst2, test_data.length));

    ASSERT_EQ(result.status, dml::status_code::ok);

    ASSERT_TRUE(test_data.check());
}

TYPED_TEST(dmlhl_dualcast, src_null) {
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::dualcast(seed, length);

    auto result = this->run(dml::dualcast,
                            dml::make_view<uint8_t>(nullptr, test_data.length),
                            dml::make_view(test_data.dst1, test_data.length),
                            dml::make_view(test_data.dst2, test_data.length));

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_dualcast, dst1_null) {
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::dualcast(seed, length);

    auto result = this->run(dml::dualcast,
                            dml::make_view(test_data.src),
                            dml::make_view<uint8_t>(nullptr, test_data.length),
                            dml::make_view(test_data.dst2, test_data.length));

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_dualcast, dst2_null) {
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::dualcast(seed, length);

    auto result = this->run(dml::dualcast,
                            dml::make_view(test_data.src),
                            dml::make_view(test_data.dst1, test_data.length),
                            dml::make_view<uint8_t>(nullptr, test_data.length));

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_dualcast, inconsistent_size_1) {
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::dualcast(seed, length);

    auto result = this->run(dml::dualcast,
                            dml::make_view(test_data.src),
                            dml::make_view(test_data.dst1, test_data.length - 1u),
                            dml::make_view(test_data.dst2, test_data.length));

    ASSERT_EQ(result.status, dml::status_code::inconsistent_size);
}

TYPED_TEST(dmlhl_dualcast, inconsistent_size_2) {
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::dualcast(seed, length);

    auto result = this->run(dml::dualcast,
                            dml::make_view(test_data.src),
                            dml::make_view(test_data.dst1, test_data.length),
                            dml::make_view(test_data.dst2, test_data.length - 1u));

    ASSERT_EQ(result.status, dml::status_code::inconsistent_size);
}

TYPED_TEST(dmlhl_dualcast, bad_size) {
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::dualcast(seed, length);

    auto result = this->run(dml::dualcast,
                            dml::make_view(test_data.src.data(), 0u),
                            dml::make_view(test_data.dst1, 0u),
                            dml::make_view(test_data.dst2, 0u));

    ASSERT_EQ(result.status, dml::status_code::bad_size);
}

TYPED_TEST(dmlhl_dualcast, bad_padding) {
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::dualcast(seed, length);

    auto result = this->run(dml::dualcast,
                            dml::make_view(test_data.src),
                            dml::make_view(test_data.dst1, test_data.length),
                            dml::make_view(test_data.dst2 - 1u, test_data.length));

    ASSERT_EQ(result.status, dml::status_code::dualcast_bad_padding);
}

TEST(dmlhl_dualcast, bad_alloc) {
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::dualcast(seed, length);

    ASSERT_ANY_THROW(auto result = dml::submit<dml::software>(dml::dualcast,
                                                              dml::make_view(test_data.src),
                                                              dml::make_view(test_data.dst1, test_data.length),
                                                              dml::make_view(test_data.dst2, test_data.length),
                                                              dml::execution_interface(
                                                                      [](auto &&task) {
                                                                          task();
                                                                      },
                             exception_allocator<int>())));
}
