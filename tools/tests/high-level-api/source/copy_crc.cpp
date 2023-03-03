/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml/dml.hpp>

#include <dml_test_utils/copy_crc.hpp>
#include <ref_crc.hpp>

#include "gtest/gtest.h"

#include "own/path.hpp"

#include "t_utility_functions.hpp"

#include "t_test_case_generator.hpp"

#include "t_random_parameters.hpp"

DML_TESTING_HL_PARAMETRIZE(dmlhl_copy_crc);

TYPED_TEST(dmlhl_copy_crc, no_flags)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::copy_crc(seed, length);

    auto result =
        this->run(dml::copy_crc, dml::make_view(test_data.src), dml::make_view(test_data.dst), test_data.crc_seed);

    ASSERT_EQ(result.status, dml::status_code::ok);

    const auto ref = dml::reference::calculate_crc<uint32_t, 0>(
        test_data.src.data(), test_data.src.data() + length, test_data.crc_seed);

    ASSERT_EQ(result.crc_value, ref);

    ASSERT_TRUE(test_data.check());
}

TYPED_TEST(dmlhl_copy_crc, bypass_reflection)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    const auto seed = test_system::get_seed();
    auto test_cases = dml::test::test_case_generator<uint32_t>(dml::test::get_default_test_lengths);

    for (auto length : test_cases) {

        auto test_data = dml::testing::copy_crc(seed, length);

        auto result = this->run(dml::copy_crc.bypass_reflection(),
                                dml::make_view(test_data.src),
                                dml::make_view(test_data.dst),
                                test_data.crc_seed);

        ASSERT_EQ(result.status, dml::status_code::ok) << test_cases.info(length);

        const auto ref = dml::reference::calculate_crc<uint32_t, DML_FLAG_CRC_BYPASS_REFLECTION>(
                test_data.src.data(), test_data.src.data() + length, test_data.crc_seed);

        ASSERT_EQ(result.crc_value, ref) << test_cases.info(length);

        ASSERT_TRUE(test_data.check()) << test_cases.info(length);
    }
}

TYPED_TEST(dmlhl_copy_crc, bypass_data_reflection)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    const auto seed = test_system::get_seed();
    auto test_cases = dml::test::test_case_generator<uint32_t>(dml::test::get_default_test_lengths);

    for (auto length : test_cases) {

        auto test_data = dml::testing::copy_crc(seed, length);

        auto result = this->run(dml::copy_crc.bypass_data_reflection(),
                                dml::make_view(test_data.src),
                                dml::make_view(test_data.dst),
                                test_data.crc_seed);

        ASSERT_EQ(result.status, dml::status_code::ok) << test_cases.info(length);

        const auto ref = dml::reference::calculate_crc<uint32_t, DML_FLAG_CRC_BYPASS_DATA_REFLECTION>(
                test_data.src.data(), test_data.src.data() + length, test_data.crc_seed);

        ASSERT_EQ(result.crc_value, ref) << test_cases.info(length);

        ASSERT_TRUE(test_data.check()) << test_cases.info(length);
    }
}

TYPED_TEST(dmlhl_copy_crc, all_flags)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    const auto seed = test_system::get_seed();
    auto test_cases = dml::test::test_case_generator<uint32_t>(dml::test::get_default_test_lengths);

    for (auto length : test_cases) {

        auto test_data = dml::testing::copy_crc(seed, length);

        auto result = this->run(dml::copy_crc.bypass_reflection().bypass_data_reflection(),
                                dml::make_view(test_data.src),
                                dml::make_view(test_data.dst),
                                test_data.crc_seed);

        ASSERT_EQ(result.status, dml::status_code::ok) << test_cases.info(length);

        const auto ref =
                dml::reference::calculate_crc<uint32_t,
                        DML_FLAG_CRC_BYPASS_REFLECTION | DML_FLAG_CRC_BYPASS_DATA_REFLECTION>(
                        test_data.src.data(), test_data.src.data() + length, test_data.crc_seed);

        ASSERT_EQ(result.crc_value, ref) << test_cases.info(length);

        ASSERT_TRUE(test_data.check()) << test_cases.info(length);
    }
}

TYPED_TEST(dmlhl_copy_crc, src_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::copy_crc(seed, length);

    auto result = this->run(dml::copy_crc,
                            dml::make_view<uint8_t>(nullptr, test_data.length),
                            dml::make_view(test_data.dst),
                            test_data.crc_seed);

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_copy_crc, dst_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::copy_crc(seed, length);

    auto result = this->run(dml::copy_crc,
                            dml::make_view(test_data.src),
                            dml::make_view<uint8_t>(nullptr, test_data.length),
                            test_data.crc_seed);

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_copy_crc, inconsistent_size)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::copy_crc(seed, length);

    auto result = this->run(dml::copy_crc,
                            dml::make_view(test_data.src),
                            dml::make_view<uint8_t>(test_data.dst.data(), test_data.length - 1u),
                            test_data.crc_seed);

    ASSERT_EQ(result.status, dml::status_code::inconsistent_size);
}

TYPED_TEST(dmlhl_copy_crc, bad_size)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::copy_crc(seed, length);

    auto result = this->run(dml::copy_crc,
                            dml::make_view(test_data.src.data(), 0u),
                            dml::make_view(test_data.dst.data(), 0u),
                            test_data.crc_seed);

    ASSERT_EQ(result.status, dml::status_code::bad_size);
}

TEST(dmlhl_copy_crc, bad_alloc)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::copy_crc(seed, length);

    ASSERT_ANY_THROW(auto result = dml::submit<dml::software>(dml::copy_crc,
                                                              dml::make_view(test_data.src),
                                                              dml::make_view(test_data.dst),
                                                              test_data.crc_seed,
                                                              dml::execution_interface(
                                                                  [](auto &&task)
                                                                  {
                                                                      task();
                                                                  },
                                                                  exception_allocator<int>())));
}
