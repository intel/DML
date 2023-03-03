/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains bad argument tests for the dml_batch_get<parameter_name> operations
 * @date 9/23/2020
 *
 */
#include "t_common.hpp"


namespace dml
{

    /** Byte length of the pattern */
    constexpr auto PATTERN_SIZE = 8u;


    /**
     * @brief Tests the function with null dml_job_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_get_batch_size, tb_job_ptr_null)
    {
        constexpr auto task_count = 4u;
        auto byte_size            = 0u;

        const auto status = dml_get_batch_size(nullptr, task_count, &byte_size);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, status);
    }


    /**
     * @brief Tests the function with null byte_size_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_get_batch_size, tb_byte_size_ptr_null)
    {
        auto job_size             = 0u;
        constexpr auto task_count = 4u;

        const auto get_job_size_status  = dml_get_job_size(dml::test::variables_t::path, &job_size);
        EXPECT_EQ(DML_STATUS_OK, get_job_size_status);

        auto dml_job_ptr = reinterpret_cast<dml_job_t *>(dml::test::global_allocator::allocate_ptr(job_size));
        EXPECT_NE(nullptr, dml_job_ptr);

        const auto init_job_status = dml_init_job(dml::test::variables_t::path, dml_job_ptr);
        EXPECT_EQ(DML_STATUS_OK, init_job_status);

        const auto status = dml_get_batch_size(dml_job_ptr, task_count, nullptr);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, status);
    }


    /**
     * @brief Tests the function with zero task
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_get_batch_size, tb_zero_tasks)
    {
        auto job_size             = 0u;
        auto byte_size            = 0u;
        constexpr auto task_count = 0u;

        const auto get_job_size_status  = dml_get_job_size(dml::test::variables_t::path, &job_size);
        EXPECT_EQ(DML_STATUS_OK, get_job_size_status);

        auto dml_job_ptr = reinterpret_cast<dml_job_t *>(dml::test::global_allocator::allocate_ptr(job_size));
        EXPECT_NE(nullptr, dml_job_ptr);

        const auto init_job_status = dml_init_job(dml::test::variables_t::path, dml_job_ptr);
        EXPECT_EQ(DML_STATUS_OK, init_job_status);

        const auto status = dml_get_batch_size(dml_job_ptr, task_count, &byte_size);
        EXPECT_EQ(DML_STATUS_BATCH_SIZE_ERROR, status);
    }


    /**
     * @brief Tests the function with one task count
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_get_batch_size, tb_one_task)
    {
        auto job_size             = 0u;
        auto byte_size            = 0u;
        constexpr auto task_count = 1u;

        const auto get_job_size_status  = dml_get_job_size(dml::test::variables_t::path, &job_size);
        EXPECT_EQ(DML_STATUS_OK, get_job_size_status);

        auto dml_job_ptr = reinterpret_cast<dml_job_t *>(dml::test::global_allocator::allocate_ptr(job_size));
        EXPECT_NE(nullptr, dml_job_ptr);

        const auto init_job_status = dml_init_job(dml::test::variables_t::path, dml_job_ptr);
        EXPECT_EQ(DML_STATUS_OK, init_job_status);

        const auto status = dml_get_batch_size(dml_job_ptr, task_count, &byte_size);
        EXPECT_EQ(DML_STATUS_BATCH_SIZE_ERROR, status);
    }



    /**
     * @brief Tests the function with null job_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_get_result, tb_job_ptr_null)
    {
        auto job_size             = 0u;
        auto byte_size            = 0u;
        constexpr auto task_count = 4u;
        constexpr auto task_index = 0u;

        const auto get_job_size_status  = dml_get_job_size(dml::test::variables_t::path, &job_size);
        EXPECT_EQ(DML_STATUS_OK, get_job_size_status);

        auto dml_job_ptr = reinterpret_cast<dml_job_t *>(dml::test::global_allocator::allocate_ptr(job_size));
        EXPECT_NE(nullptr, dml_job_ptr);

        const auto init_job_status = dml_init_job(dml::test::variables_t::path, dml_job_ptr);
        EXPECT_EQ(DML_STATUS_OK, init_job_status);

        const auto status = dml_get_batch_size(dml_job_ptr, task_count, &byte_size);
        EXPECT_EQ(DML_STATUS_OK, status);

        dml_job_ptr->destination_first_ptr = dml::test::global_allocator::allocate_ptr(byte_size);
        dml_job_ptr->destination_length    = byte_size;
        EXPECT_NE(nullptr, dml_job_ptr->destination_first_ptr);

        auto result = dml_meta_result_t();

        const auto get_op_status = dml_batch_get_result(nullptr, task_index, &result);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, get_op_status);
    }


    /**
     * @brief Tests the function with null result_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_get_result, tb_result_ptr_null)
    {
        auto job_size             = 0u;
        auto byte_size            = 0u;
        constexpr auto task_count = 4u;
        constexpr auto task_index = 0u;

        const auto get_job_size_status  = dml_get_job_size(dml::test::variables_t::path, &job_size);
        EXPECT_EQ(DML_STATUS_OK, get_job_size_status);

        auto dml_job_ptr = reinterpret_cast<dml_job_t *>(dml::test::global_allocator::allocate_ptr(job_size));
        EXPECT_NE(nullptr, dml_job_ptr);

        const auto init_job_status = dml_init_job(dml::test::variables_t::path, dml_job_ptr);
        EXPECT_EQ(DML_STATUS_OK, init_job_status);

        const auto status = dml_get_batch_size(dml_job_ptr, task_count, &byte_size);
        EXPECT_EQ(DML_STATUS_OK, status);

        dml_job_ptr->destination_first_ptr = dml::test::global_allocator::allocate_ptr(PATTERN_SIZE);
        EXPECT_NE(nullptr, dml_job_ptr->destination_first_ptr);

        const auto get_op_status = dml_batch_get_result(dml_job_ptr, task_index, nullptr);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, get_op_status);
    }


    /**
     * @brief Tests the function with index overflow
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_get_result, tb_index_overflow)
    {
        auto job_size             = 0u;
        auto byte_size            = 0u;
        constexpr auto task_count = 4u;
        constexpr auto task_index = 4u;

        const auto get_job_size_status  = dml_get_job_size(dml::test::variables_t::path, &job_size);
        EXPECT_EQ(DML_STATUS_OK, get_job_size_status);

        auto dml_job_ptr = reinterpret_cast<dml_job_t *>(dml::test::global_allocator::allocate_ptr(job_size));
        EXPECT_NE(nullptr, dml_job_ptr);

        const auto init_job_status = dml_init_job(dml::test::variables_t::path, dml_job_ptr);
        EXPECT_EQ(DML_STATUS_OK, init_job_status);

        const auto status = dml_get_batch_size(dml_job_ptr, task_count, &byte_size);
        EXPECT_EQ(DML_STATUS_OK, status);

        dml_job_ptr->destination_first_ptr = dml::test::global_allocator::allocate_ptr(byte_size);
        dml_job_ptr->destination_length    = byte_size;
        EXPECT_NE(nullptr, dml_job_ptr->destination_first_ptr);

        auto result = dml_meta_result_t();

        const auto get_op_status = dml_batch_get_result(dml_job_ptr, task_index, &result);
        EXPECT_EQ(DML_STATUS_BATCH_TASK_INDEX_OVERFLOW, get_op_status);
    }


    /**
     * @brief Tests the function with null job_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_get_status, tb_job_ptr_null)
    {
        auto job_size             = 0u;
        auto byte_size            = 0u;
        constexpr auto task_count = 4u;
        constexpr auto task_index = 0u;

        const auto get_job_size_status  = dml_get_job_size(dml::test::variables_t::path, &job_size);
        EXPECT_EQ(DML_STATUS_OK, get_job_size_status);

        auto dml_job_ptr = reinterpret_cast<dml_job_t *>(dml::test::global_allocator::allocate_ptr(job_size));
        EXPECT_NE(nullptr, dml_job_ptr);

        const auto init_job_status = dml_init_job(dml::test::variables_t::path, dml_job_ptr);
        EXPECT_EQ(DML_STATUS_OK, init_job_status);

        const auto status = dml_get_batch_size(dml_job_ptr, task_count, &byte_size);
        EXPECT_EQ(DML_STATUS_OK, status);

        dml_job_ptr->destination_first_ptr = dml::test::global_allocator::allocate_ptr(byte_size);
        dml_job_ptr->destination_length    = byte_size;
        EXPECT_NE(nullptr, dml_job_ptr->destination_first_ptr);

        auto result_status = dml_status_t();

        const auto get_op_status = dml_batch_get_status(nullptr, task_index, &result_status);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, get_op_status);
    }


    /**
     * @brief Tests the function with null status_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_get_status, tb_status_ptr_null)
    {
        auto job_size             = 0u;
        auto byte_size            = 0u;
        constexpr auto task_count = 4u;
        constexpr auto task_index = 0u;

        const auto get_job_size_status  = dml_get_job_size(dml::test::variables_t::path, &job_size);
        EXPECT_EQ(DML_STATUS_OK, get_job_size_status);

        auto dml_job_ptr = reinterpret_cast<dml_job_t *>(dml::test::global_allocator::allocate_ptr(job_size));
        EXPECT_NE(nullptr, dml_job_ptr);

        const auto init_job_status = dml_init_job(dml::test::variables_t::path, dml_job_ptr);
        EXPECT_EQ(DML_STATUS_OK, init_job_status);

        const auto status = dml_get_batch_size(dml_job_ptr, task_count, &byte_size);
        EXPECT_EQ(DML_STATUS_OK, status);

        dml_job_ptr->destination_first_ptr = dml::test::global_allocator::allocate_ptr(PATTERN_SIZE);
        EXPECT_NE(nullptr, dml_job_ptr->destination_first_ptr);

        const auto get_op_status = dml_batch_get_status(dml_job_ptr, task_index, nullptr);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, get_op_status);
    }


    /**
     * @brief Tests the function with index overflow
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_get_status, tb_index_overflow)
    {
        auto job_size             = 0u;
        auto byte_size            = 0u;
        constexpr auto task_count = 4u;
        constexpr auto task_index = 4u;

        const auto get_job_size_status  = dml_get_job_size(dml::test::variables_t::path, &job_size);
        EXPECT_EQ(DML_STATUS_OK, get_job_size_status);

        auto dml_job_ptr = reinterpret_cast<dml_job_t *>(dml::test::global_allocator::allocate_ptr(job_size));
        EXPECT_NE(nullptr, dml_job_ptr);

        const auto init_job_status = dml_init_job(dml::test::variables_t::path, dml_job_ptr);
        EXPECT_EQ(DML_STATUS_OK, init_job_status);

        const auto status = dml_get_batch_size(dml_job_ptr, task_count, &byte_size);
        EXPECT_EQ(DML_STATUS_OK, status);

        dml_job_ptr->destination_first_ptr = dml::test::global_allocator::allocate_ptr(byte_size);
        dml_job_ptr->destination_length    = byte_size;
        EXPECT_NE(nullptr, dml_job_ptr->destination_first_ptr);

        auto result_status = dml_status_t();

        const auto get_op_status = dml_batch_get_status(dml_job_ptr, task_index, &result_status);
        EXPECT_EQ(DML_STATUS_BATCH_TASK_INDEX_OVERFLOW, get_op_status);
    }


    // Test registers
    DML_UNIT_TEST_REGISTER(unit_dml_get_batch_size, tb_job_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_get_batch_size, tb_byte_size_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_get_batch_size, tb_zero_tasks);
    DML_UNIT_TEST_REGISTER(unit_dml_get_batch_size, tb_one_task);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_get_result, tb_job_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_get_result, tb_result_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_get_result, tb_index_overflow);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_get_status, tb_job_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_get_status, tb_status_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_get_status, tb_index_overflow);

}
