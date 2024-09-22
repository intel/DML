/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml/dml.hpp>

#include "gtest/gtest.h"

#include <dml_test_utils/delta.hpp>

#include "own/path.hpp"
#include "utils/util.hpp"

// utils_common
#include "opcfg_checks.hpp"

DML_TESTING_HL_PARAMETRIZE(dmlhl_delta);
DML_TESTING_HL_PARAMETRIZE(dmlhl_create_delta);
DML_TESTING_HL_PARAMETRIZE(dmlhl_apply_delta);

TYPED_TEST(dmlhl_delta, success)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);
    DML_SKIP_TEST_FOR_EXPR_VERBOSE(dml::test::is_operation_disabled_on_all_wq_on_node(dml::test::opcode_delta_apply), "Test is skipped because operation is disabled.");


    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);

    auto create_result = this->run(dml::create_delta,
                                   dml::make_view(test_data.src1),
                                   dml::make_view(test_data.src2),
                                   dml::make_view(test_data.delta_record));

    ASSERT_EQ(create_result.status, dml::status_code::ok);
    ASSERT_EQ(create_result.result, dml::comparison_result::not_equal);
    ASSERT_EQ(create_result.bytes_completed, 0);
    ASSERT_NE(create_result.delta_record_size, 0);

    auto result = this->run(
        dml::apply_delta, dml::make_view(test_data.delta_record), dml::make_view(test_data.src1), create_result);

    ASSERT_EQ(result.status, dml::status_code::ok);

    ASSERT_TRUE(test_data.check());
}

TYPED_TEST(dmlhl_create_delta, src1_null)
{
    DML_SKIP_TEST_FOR_EXPR_VERBOSE(dml::test::is_operation_disabled_on_all_wq_on_node(dml::test::opcode_delta_create), "Test is skipped because operation is disabled.");
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);

    auto result = this->run(dml::create_delta,
                            dml::make_view<uint8_t>(nullptr, test_data.length),
                            dml::make_view(test_data.src2),
                            dml::make_view(test_data.delta_record));

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_create_delta, src2_null)
{
    DML_SKIP_TEST_FOR_EXPR_VERBOSE(dml::test::is_operation_disabled_on_all_wq_on_node(dml::test::opcode_delta_create), "Test is skipped because operation is disabled.");
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);

    auto result = this->run(dml::create_delta,
                            dml::make_view(test_data.src1),
                            dml::make_view<uint8_t>(nullptr, test_data.length),
                            dml::make_view(test_data.delta_record));

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_create_delta, delta_null)
{
    DML_SKIP_TEST_FOR_EXPR_VERBOSE(dml::test::is_operation_disabled_on_all_wq_on_node(dml::test::opcode_delta_create), "Test is skipped because operation is disabled.");
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);

    auto result = this->run(dml::create_delta,
                            dml::make_view(test_data.src1),
                            dml::make_view(test_data.src2),
                            dml::make_view<uint8_t>(nullptr, static_cast<dml::size_t>(test_data.delta_record.size())));

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_create_delta, inconsistent_size)
{
    DML_SKIP_TEST_FOR_EXPR_VERBOSE(dml::test::is_operation_disabled_on_all_wq_on_node(dml::test::opcode_delta_create), "Test is skipped because operation is disabled.");
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);

    auto result = this->run(dml::create_delta,
                            dml::make_view(test_data.src1),
                            dml::make_view(test_data.src2.data(), test_data.length - 1u),
                            dml::make_view(test_data.delta_record));

    ASSERT_EQ(result.status, dml::status_code::inconsistent_size);
}

TYPED_TEST(dmlhl_create_delta, bad_size_zero)
{
    DML_SKIP_TEST_FOR_EXPR_VERBOSE(dml::test::is_operation_disabled_on_all_wq_on_node(dml::test::opcode_delta_create), "Test is skipped because operation is disabled.");
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);

    auto result = this->run(dml::create_delta,
                            dml::make_view(test_data.src1.data(), 0u),
                            dml::make_view(test_data.src2.data(), 0u),
                            dml::make_view(test_data.delta_record));

    ASSERT_EQ(result.status, dml::status_code::bad_size);
}

TYPED_TEST(dmlhl_create_delta, bad_size_not_8_divided)
{
    DML_SKIP_TEST_FOR_EXPR_VERBOSE(dml::test::is_operation_disabled_on_all_wq_on_node(dml::test::opcode_delta_create), "Test is skipped because operation is disabled.");
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);

    auto result = this->run(dml::create_delta,
                            dml::make_view(test_data.src1.data(), test_data.length - 1u),
                            dml::make_view(test_data.src2.data(), test_data.length - 1u),
                            dml::make_view(test_data.delta_record));

    ASSERT_EQ(result.status, dml::status_code::bad_size);
}

TYPED_TEST(dmlhl_create_delta, delta_bad_size_less_than_80)
{
    DML_SKIP_TEST_FOR_EXPR_VERBOSE(dml::test::is_operation_disabled_on_all_wq_on_node(dml::test::opcode_delta_create), "Test is skipped because operation is disabled.");
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);

    auto result = this->run(dml::create_delta,
                            dml::make_view(test_data.src1),
                            dml::make_view(test_data.src2),
                            dml::make_view(test_data.delta_record.data(), 79u));

    ASSERT_EQ(result.status, dml::status_code::delta_bad_size);
}

TYPED_TEST(dmlhl_create_delta, delta_bad_size_not_10_divided)
{
    DML_SKIP_TEST_FOR_EXPR_VERBOSE(dml::test::is_operation_disabled_on_all_wq_on_node(dml::test::opcode_delta_create), "Test is skipped because operation is disabled.");
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);

    auto result = this->run(dml::create_delta,
                            dml::make_view(test_data.src1),
                            dml::make_view(test_data.src2),
                            dml::make_view(test_data.delta_record.data(), test_data.length + 1u));

    ASSERT_EQ(result.status, dml::status_code::delta_bad_size);
}

TYPED_TEST(dmlhl_apply_delta, src_null)
{
    DML_SKIP_TEST_FOR_EXPR_VERBOSE(dml::test::is_operation_disabled_on_all_wq_on_node(dml::test::opcode_delta_apply), "Test is skipped because operation is disabled.");
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);

    auto create_result = dml::create_delta_result{dml::status_code::ok, dml::comparison_result::not_equal, 0u, 10u};

    auto result = this->run(dml::apply_delta,
                            dml::make_view(test_data.delta_record),
                            dml::make_view<uint8_t>(nullptr, test_data.length),
                            create_result);

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_apply_delta, delta_null)
{
    DML_SKIP_TEST_FOR_EXPR_VERBOSE(dml::test::is_operation_disabled_on_all_wq_on_node(dml::test::opcode_delta_apply), "Test is skipped because operation is disabled.");
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);

    auto create_result = dml::create_delta_result{dml::status_code::ok, dml::comparison_result::not_equal, 0u, 10u};

    auto result = this->run(dml::apply_delta,
                            dml::make_view<uint8_t>(nullptr, static_cast<dml::size_t>(test_data.delta_record.size())),
                            dml::make_view(test_data.src1),
                            create_result);

    ASSERT_EQ(result.status, dml::status_code::nullptr_error);
}

TYPED_TEST(dmlhl_apply_delta, bad_size_src)
{
    DML_SKIP_TEST_FOR_EXPR_VERBOSE(dml::test::is_operation_disabled_on_all_wq_on_node(dml::test::opcode_delta_apply), "Test is skipped because operation is disabled.");
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);

    auto create_result = dml::create_delta_result{dml::status_code::ok, dml::comparison_result::not_equal, 0u, 10u};

    auto result = this->run(dml::apply_delta,
                            dml::make_view(test_data.delta_record),
                            dml::make_view(test_data.src1.data(), 0u),
                            create_result);

    ASSERT_EQ(result.status, dml::status_code::bad_size);
}

TYPED_TEST(dmlhl_apply_delta, delta_bad_size_0)
{
    DML_SKIP_TEST_FOR_EXPR_VERBOSE(dml::test::is_operation_disabled_on_all_wq_on_node(dml::test::opcode_delta_apply), "Test is skipped because operation is disabled.");
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);

    auto create_result = dml::create_delta_result{dml::status_code::ok, dml::comparison_result::not_equal, 0u, 0u};

    auto result = this->run(
        dml::apply_delta, dml::make_view(test_data.delta_record), dml::make_view(test_data.src1), create_result);

    ASSERT_EQ(result.status, dml::status_code::bad_size);
}

TYPED_TEST(dmlhl_apply_delta, delta_bad_size_not_10_divided)
{
    DML_SKIP_TEST_FOR_EXPR_VERBOSE(dml::test::is_operation_disabled_on_all_wq_on_node(dml::test::opcode_delta_apply), "Test is skipped because operation is disabled.");
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);

    auto create_result = dml::create_delta_result{dml::status_code::ok, dml::comparison_result::not_equal, 0u, 9u};

    auto result = this->run(
        dml::apply_delta, dml::make_view(test_data.delta_record), dml::make_view(test_data.src1), create_result);

    ASSERT_EQ(result.status, dml::status_code::delta_bad_size);
}

TYPED_TEST(dmlhl_apply_delta, delta_empty)
{
    DML_SKIP_TEST_FOR_EXPR_VERBOSE(dml::test::is_operation_disabled_on_all_wq_on_node(dml::test::opcode_delta_apply), "Test is skipped because operation is disabled.");
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);

    auto create_result = dml::create_delta_result{dml::status_code::ok, dml::comparison_result::equal, 0u, 10u};

    auto result = this->run(
        dml::apply_delta, dml::make_view(test_data.delta_record), dml::make_view(test_data.src1), create_result);

    ASSERT_EQ(result.status, dml::status_code::delta_delta_empty);
}

TEST(dmlhl_create_delta, bad_alloc)
{
    DML_SKIP_TEST_FOR_EXPR_VERBOSE(dml::test::is_operation_disabled_on_all_wq_on_node(dml::test::opcode_delta_create), "Test is skipped because operation is disabled.");
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);

    ASSERT_ANY_THROW(auto result = dml::submit<dml::software>(dml::create_delta,
                                                              dml::make_view(test_data.src1),
                                                              dml::make_view(test_data.src2),
                                                              dml::make_view(test_data.delta_record),
                                                              dml::execution_interface(
                                                                  [](auto &&task)
                                                                  {
                                                                      task();
                                                                  },
                                                                  exception_allocator<int>())));
}

TEST(dmlhl_apply_delta, bad_alloc)
{
    DML_SKIP_TEST_FOR_EXPR_VERBOSE(dml::test::is_operation_disabled_on_all_wq_on_node(dml::test::opcode_delta_apply), "Test is skipped because operation is disabled.");
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);

    auto create_result = dml::create_delta_result{dml::status_code::ok, dml::comparison_result::not_equal, 0u, 10u};

    ASSERT_ANY_THROW(auto result = dml::submit<dml::software>(dml::apply_delta,
                                                              dml::make_view(test_data.delta_record),
                                                              dml::make_view(test_data.src1),
                                                              create_result,
                                                              dml::execution_interface(
                                                                  [](auto &&task)
                                                                  {
                                                                      task();
                                                                  },
                                                                  exception_allocator<int>())));
}
