/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains bad argument tests for the dml_batch_set<parameter_name> operations
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
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_nop_by_index, tb_job_ptr_null)
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

        const auto set_op_status = dml_batch_set_nop_by_index(nullptr, task_index, 0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with index overflow
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_nop_by_index, tb_index_overflow)
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

        const auto set_op_status = dml_batch_set_nop_by_index(dml_job_ptr, task_index, 0u);
        EXPECT_EQ(DML_STATUS_BATCH_TASK_INDEX_OVERFLOW, set_op_status);
    }


    /**
     * @brief Tests the function with null dml_job_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_mem_move_by_index, tb_job_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, destination_ptr);

        const auto set_op_status = dml_batch_set_mem_move_by_index(nullptr, task_index, source_ptr, destination_ptr, length, 0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null source_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_mem_move_by_index, tb_source_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = nullptr;
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, destination_ptr);

        const auto set_op_status = dml_batch_set_mem_move_by_index(dml_job_ptr, task_index, source_ptr, destination_ptr, length, 0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null destination_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_mem_move_by_index, tb_destination_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto destination_ptr = nullptr;

        EXPECT_NE(nullptr, source_ptr);

        const auto set_op_status = dml_batch_set_mem_move_by_index(dml_job_ptr, task_index, source_ptr, destination_ptr, length, 0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with index overflow
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_mem_move_by_index, tb_index_overflow)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, destination_ptr);

        const auto set_op_status = dml_batch_set_mem_move_by_index(dml_job_ptr, task_index, source_ptr, destination_ptr, length, 0u);
        EXPECT_EQ(DML_STATUS_BATCH_TASK_INDEX_OVERFLOW, set_op_status);
    }


    /**
     * @brief Tests the function with null dml_job_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dualcast_by_index, tb_job_ptr_null)
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

        auto length                 = 10u;
        auto source_ptr             = dml::test::global_allocator::allocate_ptr(length);
        auto destination_first_ptr  = dml::test::global_allocator::allocate_ptr(length);
        auto destination_second_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, destination_first_ptr);
        EXPECT_NE(nullptr, destination_second_ptr);

        const auto set_op_status = dml_batch_set_dualcast_by_index(nullptr,
                                                                   task_index,
                                                                   source_ptr,
                                                                   destination_first_ptr,
                                                                   destination_second_ptr,
                                                                   length,
                                                                   0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null source_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dualcast_by_index, tb_source_ptr_null)
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

        auto length                 = 10u;
        auto source_ptr             = nullptr;
        auto destination_first_ptr  = dml::test::global_allocator::allocate_ptr(length);
        auto destination_second_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, destination_first_ptr);
        EXPECT_NE(nullptr, destination_second_ptr);

        const auto set_op_status = dml_batch_set_dualcast_by_index(dml_job_ptr,
                                                                   task_index,
                                                                   source_ptr,
                                                                   destination_first_ptr,
                                                                   destination_second_ptr,
                                                                   length,
                                                                   0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null destination_first_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dualcast_by_index, tb_destination_first_ptr_null)
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

        auto length                 = 10u;
        auto source_ptr             = dml::test::global_allocator::allocate_ptr(length);
        auto destination_first_ptr  = nullptr;
        auto destination_second_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, destination_second_ptr);

        const auto set_op_status = dml_batch_set_dualcast_by_index(dml_job_ptr,
                                                                   task_index,
                                                                   source_ptr,
                                                                   destination_first_ptr,
                                                                   destination_second_ptr,
                                                                   length,
                                                                   0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null destination_second_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dualcast_by_index, tb_destination_second_ptr_null)
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

        auto length                 = 10u;
        auto source_ptr             = dml::test::global_allocator::allocate_ptr(length);
        auto destination_first_ptr  = dml::test::global_allocator::allocate_ptr(length);
        auto destination_second_ptr = nullptr;

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, destination_first_ptr);

        const auto set_op_status = dml_batch_set_dualcast_by_index(dml_job_ptr,
                                                                   task_index,
                                                                   source_ptr,
                                                                   destination_first_ptr,
                                                                   destination_second_ptr,
                                                                   length,
                                                                   0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with index overflow
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dualcast_by_index, tb_index_overflow)
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

        auto length                 = 10u;
        auto source_ptr             = dml::test::global_allocator::allocate_ptr(length);
        auto destination_first_ptr  = dml::test::global_allocator::allocate_ptr(length);
        auto destination_second_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, destination_first_ptr);
        EXPECT_NE(nullptr, destination_second_ptr);

        const auto set_op_status = dml_batch_set_dualcast_by_index(dml_job_ptr,
                                                                   task_index,
                                                                   source_ptr,
                                                                   destination_first_ptr,
                                                                   destination_second_ptr,
                                                                   length,
                                                                   0u);
        EXPECT_EQ(DML_STATUS_BATCH_TASK_INDEX_OVERFLOW, set_op_status);
    }


    /**
     * @brief Tests the function with null job_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_compare_by_index, tb_job_ptr_null)
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

        auto length            = 10u;
        auto source_first_ptr  = dml::test::global_allocator::allocate_ptr(length);
        auto source_second_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, source_first_ptr);
        EXPECT_NE(nullptr, source_second_ptr);

        const auto set_op_status = dml_batch_set_compare_by_index(nullptr,
                                                                  task_index,
                                                                  source_first_ptr,
                                                                  source_second_ptr,
                                                                  length,
                                                                  0u,
                                                                  0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null source_first_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_compare_by_index, tb_source_first_ptr)
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

        auto length            = 10u;
        auto source_first_ptr  = nullptr;
        auto source_second_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, source_second_ptr);

        const auto set_op_status = dml_batch_set_compare_by_index(dml_job_ptr,
                                                                  task_index,
                                                                  source_first_ptr,
                                                                  source_second_ptr,
                                                                  length,
                                                                  0u,
                                                                  0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null source_second_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_compare_by_index, tb_source_second_ptr)
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

        auto length            = 10u;
        auto source_first_ptr  = dml::test::global_allocator::allocate_ptr(length);
        auto source_second_ptr = nullptr;

        EXPECT_NE(nullptr, source_first_ptr);

        const auto set_op_status = dml_batch_set_compare_by_index(dml_job_ptr,
                                                                  task_index,
                                                                  source_first_ptr,
                                                                  source_second_ptr,
                                                                  length,
                                                                  0u,
                                                                  0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with index overflow
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_compare_by_index, tb_index_overflow)
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

        auto length            = 10u;
        auto source_first_ptr  = dml::test::global_allocator::allocate_ptr(length);
        auto source_second_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, source_first_ptr);
        EXPECT_NE(nullptr, source_second_ptr);

        const auto set_op_status = dml_batch_set_compare_by_index(dml_job_ptr,
                                                                  task_index,
                                                                  source_first_ptr,
                                                                  source_second_ptr,
                                                                  length,
                                                                  0u,
                                                                  0u);
        EXPECT_EQ(DML_STATUS_BATCH_TASK_INDEX_OVERFLOW, set_op_status);
    }


    /**
     * @brief Tests the function with null job_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_compare_pattern_by_index, tb_job_ptr_null)
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

        auto length      = 10u;
        auto source_ptr  = dml::test::global_allocator::allocate_ptr(length);
        auto pattern_ptr = dml::test::global_allocator::allocate_ptr(PATTERN_SIZE);

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, pattern_ptr);

        const auto set_op_status = dml_batch_set_compare_pattern_by_index(nullptr,
                                                                          task_index,
                                                                          source_ptr,
                                                                          pattern_ptr,
                                                                          length,
                                                                          0u,
                                                                          0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null source_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_compare_pattern_by_index, tb_source_ptr_null)
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

        auto length      = 10u;
        auto source_ptr  = nullptr;
        auto pattern_ptr = dml::test::global_allocator::allocate_ptr(PATTERN_SIZE);

        EXPECT_NE(nullptr, pattern_ptr);

        const auto set_op_status = dml_batch_set_compare_pattern_by_index(dml_job_ptr,
                                                                          task_index,
                                                                          source_ptr,
                                                                          pattern_ptr,
                                                                          length,
                                                                          0u,
                                                                          0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null pattern_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_compare_pattern_by_index, tb_pattern_ptr_null)
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

        auto length      = 10u;
        auto source_ptr  = dml::test::global_allocator::allocate_ptr(length);
        auto pattern_ptr = nullptr;

        EXPECT_NE(nullptr, source_ptr);

        const auto set_op_status = dml_batch_set_compare_pattern_by_index(dml_job_ptr,
                                                                          task_index,
                                                                          source_ptr,
                                                                          pattern_ptr,
                                                                          length,
                                                                          0u,
                                                                          0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with index overflow
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_compare_pattern_by_index, tb_index_overflow)
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

        auto length      = 10u;
        auto source_ptr  = dml::test::global_allocator::allocate_ptr(length);
        auto pattern_ptr = dml::test::global_allocator::allocate_ptr(PATTERN_SIZE);

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, pattern_ptr);

        const auto set_op_status = dml_batch_set_compare_pattern_by_index(dml_job_ptr,
                                                                          task_index,
                                                                          source_ptr,
                                                                          pattern_ptr,
                                                                          length,
                                                                          0u,
                                                                          0u);
        EXPECT_EQ(DML_STATUS_BATCH_TASK_INDEX_OVERFLOW, set_op_status);
    }


    /**
     * @brief Tests the function with null job_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_crc_by_index, tb_job_ptr_null)
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

        auto length       = 10u;
        auto source_ptr   = dml::test::global_allocator::allocate_ptr(length);
        auto crc_seed_ptr = reinterpret_cast<uint32_t *>(dml::test::global_allocator::allocate_ptr(sizeof(uint32_t)));

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, crc_seed_ptr);

        (*crc_seed_ptr) = 0u;

        const auto set_op_status = dml_batch_set_crc_by_index(nullptr, task_index, source_ptr, length, crc_seed_ptr, 0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null source_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_crc_by_index, tb_source_ptr_null)
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

        auto length       = 10u;
        auto source_ptr   = nullptr;
        auto crc_seed_ptr = reinterpret_cast<uint32_t *>(dml::test::global_allocator::allocate_ptr(sizeof(uint32_t)));

        EXPECT_NE(nullptr, crc_seed_ptr);

        (*crc_seed_ptr) = 0u;

        const auto set_op_status = dml_batch_set_crc_by_index(dml_job_ptr, task_index, source_ptr, length, crc_seed_ptr, 0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null crc_seed_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_crc_by_index, tb_crc_seed_ptr_null)
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

        auto length       = 10u;
        auto source_ptr   = dml::test::global_allocator::allocate_ptr(length);
        auto crc_seed_ptr = nullptr;

        const auto set_op_status = dml_batch_set_crc_by_index(dml_job_ptr, task_index, source_ptr, length, crc_seed_ptr, 0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with index overflow
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_crc_by_index, tb_index_overflow)
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

        auto length       = 10u;
        auto source_ptr   = dml::test::global_allocator::allocate_ptr(length);
        auto crc_seed_ptr = reinterpret_cast<uint32_t *>(dml::test::global_allocator::allocate_ptr(sizeof(uint32_t)));

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, crc_seed_ptr);

        (*crc_seed_ptr) = 0u;

        const auto set_op_status = dml_batch_set_crc_by_index(dml_job_ptr, task_index, source_ptr, length, crc_seed_ptr, 0u);
        EXPECT_EQ(DML_STATUS_BATCH_TASK_INDEX_OVERFLOW, set_op_status);
    }


    /**
     * @brief Tests the function with null job_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_copy_crc_by_index, tb_job_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto crc_seed_ptr    = reinterpret_cast<uint32_t *>(dml::test::global_allocator::allocate_ptr(sizeof(uint32_t)));
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, crc_seed_ptr);
        EXPECT_NE(nullptr, destination_ptr);

        (*crc_seed_ptr) = 0u;

        const auto set_op_status = dml_batch_set_copy_crc_by_index(nullptr,
                                                                   task_index,
                                                                   source_ptr,
                                                                   length,
                                                                   crc_seed_ptr,
                                                                   destination_ptr,
                                                                   0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null source_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_copy_crc_by_index, tb_source_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = nullptr;
        auto crc_seed_ptr    = reinterpret_cast<uint32_t *>(dml::test::global_allocator::allocate_ptr(sizeof(uint32_t)));
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, crc_seed_ptr);
        EXPECT_NE(nullptr, destination_ptr);

        (*crc_seed_ptr) = 0u;

        const auto set_op_status = dml_batch_set_copy_crc_by_index(dml_job_ptr,
                                                                   task_index,
                                                                   source_ptr,
                                                                   length,
                                                                   crc_seed_ptr,
                                                                   destination_ptr,
                                                                   0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null crc_seed_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_copy_crc_by_index, tb_crc_seed_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto crc_seed_ptr    = nullptr;
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, destination_ptr);

        const auto set_op_status = dml_batch_set_copy_crc_by_index(dml_job_ptr,
                                                                   task_index,
                                                                   source_ptr,
                                                                   length,
                                                                   crc_seed_ptr,
                                                                   destination_ptr,
                                                                   0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null destination_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_copy_crc_by_index, tb_destination_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto crc_seed_ptr    = reinterpret_cast<uint32_t *>(dml::test::global_allocator::allocate_ptr(sizeof(uint32_t)));
        auto destination_ptr = nullptr;

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, crc_seed_ptr);

        (*crc_seed_ptr);

        const auto set_op_status = dml_batch_set_copy_crc_by_index(dml_job_ptr,
                                                                   task_index,
                                                                   source_ptr,
                                                                   length,
                                                                   crc_seed_ptr,
                                                                   destination_ptr,
                                                                   0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with index overflow
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_copy_crc_by_index, tb_index_overflow)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto crc_seed_ptr    = reinterpret_cast<uint32_t *>(dml::test::global_allocator::allocate_ptr(sizeof(uint32_t)));
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, crc_seed_ptr);
        EXPECT_NE(nullptr, destination_ptr);

        (*crc_seed_ptr) = 0u;

        const auto set_op_status = dml_batch_set_copy_crc_by_index(dml_job_ptr,
                                                                   task_index,
                                                                   source_ptr,
                                                                   length,
                                                                   crc_seed_ptr,
                                                                   destination_ptr,
                                                                   0u);
        EXPECT_EQ(DML_STATUS_BATCH_TASK_INDEX_OVERFLOW, set_op_status);
    }


    /**
     * @brief Tests the function with null job_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_fill_by_index, tb_job_ptr_null)
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

        auto length          = 10u;
        auto pattern_ptr     = dml::test::global_allocator::allocate_ptr(PATTERN_SIZE);
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, pattern_ptr);
        EXPECT_NE(nullptr, destination_ptr);

        const auto set_op_status = dml_batch_set_fill_by_index(nullptr, task_index, pattern_ptr, destination_ptr, length, 0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null pattern_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_fill_by_index, tb_pattern_ptr_null)
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

        auto length          = 10u;
        auto pattern_ptr     = nullptr;
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, destination_ptr);

        const auto set_op_status = dml_batch_set_fill_by_index(dml_job_ptr, task_index, pattern_ptr, destination_ptr, length, 0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null destination_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_fill_by_index, tb_destination_ptr_null)
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

        auto length          = 10u;
        auto pattern_ptr     = dml::test::global_allocator::allocate_ptr(PATTERN_SIZE);
        auto destination_ptr = nullptr;

        EXPECT_NE(nullptr, pattern_ptr);

        const auto set_op_status = dml_batch_set_fill_by_index(dml_job_ptr, task_index, pattern_ptr, destination_ptr, length, 0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with index overflow
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_fill_by_index, tb_index_overflow)
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

        auto length          = 10u;
        auto pattern_ptr     = dml::test::global_allocator::allocate_ptr(PATTERN_SIZE);
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, pattern_ptr);
        EXPECT_NE(nullptr, destination_ptr);

        const auto set_op_status = dml_batch_set_fill_by_index(dml_job_ptr, task_index, pattern_ptr, destination_ptr, length, 0u);
        EXPECT_EQ(DML_STATUS_BATCH_TASK_INDEX_OVERFLOW, set_op_status);
    }


    /**
     * @brief Tests the function with null job_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_cache_flush_by_index, tb_job_ptr_null)
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

        auto length          = 10u;
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, destination_ptr);

        const auto set_op_status = dml_batch_set_cache_flush_by_index(nullptr, task_index, destination_ptr, length, 0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null destination_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_cache_flush_by_index, tb_destination_ptr_null)
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

        auto length          = 10u;
        auto destination_ptr = nullptr;

        const auto set_op_status = dml_batch_set_cache_flush_by_index(dml_job_ptr, task_index, destination_ptr, length, 0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with index overflow
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_cache_flush_by_index, tb_index_overflow)
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

        auto length          = 10u;
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, destination_ptr);

        const auto set_op_status = dml_batch_set_cache_flush_by_index(dml_job_ptr, task_index, destination_ptr, length, 0u);
        EXPECT_EQ(DML_STATUS_BATCH_TASK_INDEX_OVERFLOW, set_op_status);
    }


    /**
     * @brief Tests the function with null job_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_delta_create_by_index, tb_job_ptr_null)
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

        auto length           = 10u;
        auto source_ptr       = dml::test::global_allocator::allocate_ptr(length);
        auto reference_ptr    = dml::test::global_allocator::allocate_ptr(length);
        auto delta_record_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, reference_ptr);
        EXPECT_NE(nullptr, delta_record_ptr);

        const auto set_op_status = dml_batch_set_delta_create_by_index(nullptr,
                                                                       task_index,
                                                                       source_ptr,
                                                                       reference_ptr,
                                                                       length,
                                                                       delta_record_ptr,
                                                                       length,
                                                                       0u,
                                                                       0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null source_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_delta_create_by_index, tb_source_ptr_null)
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

        auto length           = 10u;
        auto source_ptr       = nullptr;
        auto reference_ptr    = dml::test::global_allocator::allocate_ptr(length);
        auto delta_record_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, reference_ptr);
        EXPECT_NE(nullptr, delta_record_ptr);

        const auto set_op_status = dml_batch_set_delta_create_by_index(dml_job_ptr,
                                                                       task_index,
                                                                       source_ptr,
                                                                       reference_ptr,
                                                                       length,
                                                                       delta_record_ptr,
                                                                       length,
                                                                       0u,
                                                                       0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null reference_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_delta_create_by_index, tb_reference_ptr_null)
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

        auto length           = 10u;
        auto source_ptr       = dml::test::global_allocator::allocate_ptr(length);
        auto reference_ptr    = nullptr;
        auto delta_record_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, delta_record_ptr);

        const auto set_op_status = dml_batch_set_delta_create_by_index(dml_job_ptr,
                                                                       task_index,
                                                                       source_ptr,
                                                                       reference_ptr,
                                                                       length,
                                                                       delta_record_ptr,
                                                                       length,
                                                                       0u,
                                                                       0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null delta_record_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_delta_create_by_index, tb_delta_record_ptr_null)
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

        auto length           = 10u;
        auto source_ptr       = dml::test::global_allocator::allocate_ptr(length);
        auto reference_ptr    = dml::test::global_allocator::allocate_ptr(length);
        auto delta_record_ptr = nullptr;

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, reference_ptr);

        const auto set_op_status = dml_batch_set_delta_create_by_index(dml_job_ptr,
                                                                       task_index,
                                                                       source_ptr,
                                                                       reference_ptr,
                                                                       length,
                                                                       delta_record_ptr,
                                                                       length,
                                                                       0u,
                                                                       0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with index overflow
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_delta_create_by_index, tb_index_overflow)
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

        auto length           = 10u;
        auto source_ptr       = dml::test::global_allocator::allocate_ptr(length);
        auto reference_ptr    = dml::test::global_allocator::allocate_ptr(length);
        auto delta_record_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, reference_ptr);
        EXPECT_NE(nullptr, delta_record_ptr);

        const auto set_op_status = dml_batch_set_delta_create_by_index(dml_job_ptr,
                                                                       task_index,
                                                                       source_ptr,
                                                                       reference_ptr,
                                                                       length,
                                                                       delta_record_ptr,
                                                                       length,
                                                                       0u,
                                                                       0u);
        EXPECT_EQ(DML_STATUS_BATCH_TASK_INDEX_OVERFLOW, set_op_status);
    }


    /**
     * @brief Tests the function with null job_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_delta_apply_by_index, tb_job_ptr_null)
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

        auto length           = 10u;
        auto destination_ptr  = dml::test::global_allocator::allocate_ptr(length);
        auto delta_record_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, destination_ptr);
        EXPECT_NE(nullptr, delta_record_ptr);

        const auto set_op_status = dml_batch_set_delta_apply_by_index(nullptr,
                                                                      task_index,
                                                                      delta_record_ptr,
                                                                      length,
                                                                      destination_ptr,
                                                                      length,
                                                                      0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null destination_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_delta_apply_by_index, tb_destination_ptr_null)
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

        auto length           = 10u;
        auto destination_ptr  = nullptr;
        auto delta_record_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, delta_record_ptr);

        const auto set_op_status = dml_batch_set_delta_apply_by_index(dml_job_ptr,
                                                                      task_index,
                                                                      delta_record_ptr,
                                                                      length,
                                                                      destination_ptr,
                                                                      length,
                                                                      0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null delta_record_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_delta_apply_by_index, tb_delta_record_ptr_null)
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

        auto length           = 10u;
        auto destination_ptr  = dml::test::global_allocator::allocate_ptr(length);
        auto delta_record_ptr = nullptr;

        EXPECT_NE(nullptr, destination_ptr);

        const auto set_op_status = dml_batch_set_delta_apply_by_index(dml_job_ptr,
                                                                      task_index,
                                                                      delta_record_ptr,
                                                                      length,
                                                                      destination_ptr,
                                                                      length,
                                                                      0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with index overflow
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_delta_apply_by_index, tb_index_overflow)
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

        auto length           = 10u;
        auto destination_ptr  = dml::test::global_allocator::allocate_ptr(length);
        auto delta_record_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, destination_ptr);
        EXPECT_NE(nullptr, delta_record_ptr);

        const auto set_op_status = dml_batch_set_delta_apply_by_index(dml_job_ptr,
                                                                      task_index,
                                                                      delta_record_ptr,
                                                                      length,
                                                                      destination_ptr,
                                                                      length,
                                                                      0u);
        EXPECT_EQ(DML_STATUS_BATCH_TASK_INDEX_OVERFLOW, set_op_status);
    }


    /**
     * @brief Tests the function with null job_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_check_by_index, tb_job_ptr_null)
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

        auto length         = 10u;
        auto source_ptr     = dml::test::global_allocator::allocate_ptr(length);
        auto dif_config_ptr = dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t));

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, dif_config_ptr);

        const auto set_op_status = dml_batch_set_dif_check_by_index(nullptr,
                                                                    task_index,
                                                                    source_ptr,
                                                                    length,
                                                                    reinterpret_cast<dml_dif_config_t *>(dif_config_ptr),
                                                                    0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null source_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_check_by_index, tb_source_ptr_null)
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

        auto length         = 10u;
        auto source_ptr     = nullptr;
        auto dif_config_ptr = dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t));

        EXPECT_NE(nullptr, dif_config_ptr);

        const auto set_op_status = dml_batch_set_dif_check_by_index(dml_job_ptr,
                                                                    task_index,
                                                                    source_ptr,
                                                                    length,
                                                                    reinterpret_cast<dml_dif_config_t *>(dif_config_ptr),
                                                                    0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null dif_config_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_check_by_index, tb_dif_config_ptr_null)
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

        auto length         = 10u;
        auto source_ptr     = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, source_ptr);

        const auto set_op_status = dml_batch_set_dif_check_by_index(dml_job_ptr,
                                                                    task_index,
                                                                    source_ptr,
                                                                    length,
                                                                    nullptr,
                                                                    0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with index overflow
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_check_by_index, tb_index_overflow)
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

        auto length         = 10u;
        auto source_ptr     = dml::test::global_allocator::allocate_ptr(length);
        auto dif_config_ptr = dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t));

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, dif_config_ptr);

        const auto set_op_status = dml_batch_set_dif_check_by_index(dml_job_ptr,
                                                                    task_index,
                                                                    source_ptr,
                                                                    length,
                                                                    reinterpret_cast<dml_dif_config_t *>(dif_config_ptr),
                                                                    0u);
        EXPECT_EQ(DML_STATUS_BATCH_TASK_INDEX_OVERFLOW, set_op_status);
    }


    /**
     * @brief Tests the function with null job_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_update_by_index, tb_job_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);
        auto dif_config_ptr  = dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t));

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, destination_ptr);
        EXPECT_NE(nullptr, dif_config_ptr);

        const auto set_op_status = dml_batch_set_dif_update_by_index(nullptr,
                                                                     task_index,
                                                                     source_ptr,
                                                                     length,
                                                                     reinterpret_cast<dml_dif_config_t *>(dif_config_ptr),
                                                                     destination_ptr,
                                                                     length,
                                                                     0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null source_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_update_by_index, tb_source_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = nullptr;
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);
        auto dif_config_ptr  = dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t));

        EXPECT_NE(nullptr, destination_ptr);
        EXPECT_NE(nullptr, dif_config_ptr);

        const auto set_op_status = dml_batch_set_dif_update_by_index(dml_job_ptr,
                                                                     task_index,
                                                                     source_ptr,
                                                                     length,
                                                                     reinterpret_cast<dml_dif_config_t *>(dif_config_ptr),
                                                                     destination_ptr,
                                                                     length,
                                                                     0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null destination_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_update_by_index, tb_destination_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto destination_ptr = nullptr;
        auto dif_config_ptr  = dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t));

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, dif_config_ptr);

        const auto set_op_status = dml_batch_set_dif_update_by_index(dml_job_ptr,
                                                                     task_index,
                                                                     source_ptr,
                                                                     length,
                                                                     reinterpret_cast<dml_dif_config_t *>(dif_config_ptr),
                                                                     destination_ptr,
                                                                     length,
                                                                     0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null dif_config_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_update_by_index, tb_dif_config_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, destination_ptr);

        const auto set_op_status = dml_batch_set_dif_update_by_index(dml_job_ptr,
                                                                     task_index,
                                                                     source_ptr,
                                                                     length,
                                                                     nullptr,
                                                                     destination_ptr,
                                                                     length,
                                                                     0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with index overflow
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_update_by_index, tb_index_overflow)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);
        auto dif_config_ptr  = dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t));

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, destination_ptr);
        EXPECT_NE(nullptr, dif_config_ptr);

        const auto set_op_status = dml_batch_set_dif_update_by_index(dml_job_ptr,
                                                                     task_index,
                                                                     source_ptr,
                                                                     length,
                                                                     reinterpret_cast<dml_dif_config_t *>(dif_config_ptr),
                                                                     destination_ptr,
                                                                     length,
                                                                     0u);
        EXPECT_EQ(DML_STATUS_BATCH_TASK_INDEX_OVERFLOW, set_op_status);
    }


    /**
     * @brief Tests the function with null job_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_insert_by_index, tb_job_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);
        auto dif_config_ptr  = dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t));

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, destination_ptr);
        EXPECT_NE(nullptr, dif_config_ptr);

        const auto set_op_status = dml_batch_set_dif_insert_by_index(nullptr,
                                                                     task_index,
                                                                     source_ptr,
                                                                     length,
                                                                     reinterpret_cast<dml_dif_config_t *>(dif_config_ptr),
                                                                     destination_ptr,
                                                                     length,
                                                                     0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null source_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_insert_by_index, tb_source_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = nullptr;
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);
        auto dif_config_ptr  = dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t));

        EXPECT_NE(nullptr, destination_ptr);
        EXPECT_NE(nullptr, dif_config_ptr);

        const auto set_op_status = dml_batch_set_dif_insert_by_index(dml_job_ptr,
                                                                     task_index,
                                                                     source_ptr,
                                                                     length,
                                                                     reinterpret_cast<dml_dif_config_t *>(dif_config_ptr),
                                                                     destination_ptr,
                                                                     length,
                                                                     0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null destination_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_insert_by_index, tb_destination_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto destination_ptr = nullptr;
        auto dif_config_ptr  = dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t));

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, dif_config_ptr);

        const auto set_op_status = dml_batch_set_dif_insert_by_index(dml_job_ptr,
                                                                     task_index,
                                                                     source_ptr,
                                                                     length,
                                                                     reinterpret_cast<dml_dif_config_t *>(dif_config_ptr),
                                                                     destination_ptr,
                                                                     length,
                                                                     0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null dif_config_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_insert_by_index, tb_dif_config_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, destination_ptr);

        const auto set_op_status = dml_batch_set_dif_insert_by_index(dml_job_ptr,
                                                                     task_index,
                                                                     source_ptr,
                                                                     length,
                                                                     nullptr,
                                                                     destination_ptr,
                                                                     length,
                                                                     0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with index overflow
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_insert_by_index, tb_index_overflow)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);
        auto dif_config_ptr  = dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t));

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, destination_ptr);
        EXPECT_NE(nullptr, dif_config_ptr);

        const auto set_op_status = dml_batch_set_dif_insert_by_index(dml_job_ptr,
                                                                     task_index,
                                                                     source_ptr,
                                                                     length,
                                                                     reinterpret_cast<dml_dif_config_t *>(dif_config_ptr),
                                                                     destination_ptr,
                                                                     length,
                                                                     0u);
        EXPECT_EQ(DML_STATUS_BATCH_TASK_INDEX_OVERFLOW, set_op_status);
    }


    /**
     * @brief Tests the function with null job_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_strip_by_index, tb_job_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);
        auto dif_config_ptr  = dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t));

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, destination_ptr);
        EXPECT_NE(nullptr, dif_config_ptr);

        const auto set_op_status = dml_batch_set_dif_strip_by_index(nullptr,
                                                                    task_index,
                                                                    source_ptr,
                                                                    length,
                                                                    reinterpret_cast<dml_dif_config_t *>(dif_config_ptr),
                                                                    destination_ptr,
                                                                    length,
                                                                    0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null source_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_strip_by_index, tb_source_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = nullptr;
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);
        auto dif_config_ptr  = dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t));

        EXPECT_NE(nullptr, destination_ptr);
        EXPECT_NE(nullptr, dif_config_ptr);

        const auto set_op_status = dml_batch_set_dif_strip_by_index(dml_job_ptr,
                                                                    task_index,
                                                                    source_ptr,
                                                                    length,
                                                                    reinterpret_cast<dml_dif_config_t *>(dif_config_ptr),
                                                                    destination_ptr,
                                                                    length,
                                                                    0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null destination_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_strip_by_index, tb_destination_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto destination_ptr = nullptr;
        auto dif_config_ptr  = dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t));

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, dif_config_ptr);

        const auto set_op_status = dml_batch_set_dif_strip_by_index(dml_job_ptr,
                                                                    task_index,
                                                                    source_ptr,
                                                                    length,
                                                                    reinterpret_cast<dml_dif_config_t *>(dif_config_ptr),
                                                                    destination_ptr,
                                                                    length,
                                                                    0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with null dif_config_ptr
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_strip_by_index, tb_dif_config_ptr_null)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, destination_ptr);

        const auto set_op_status = dml_batch_set_dif_strip_by_index(dml_job_ptr,
                                                                    task_index,
                                                                    source_ptr,
                                                                    length,
                                                                    nullptr,
                                                                    destination_ptr,
                                                                    length,
                                                                    0u);
        EXPECT_EQ(DML_STATUS_NULL_POINTER_ERROR, set_op_status);
    }


    /**
     * @brief Tests the function with index overflow
     */
    DML_UNIT_TEST_GENERATOR(unit_dml_batch_set_dif_strip_by_index, tb_index_overflow)
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

        auto length          = 10u;
        auto source_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto destination_ptr = dml::test::global_allocator::allocate_ptr(length);
        auto dif_config_ptr  = dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t));

        EXPECT_NE(nullptr, source_ptr);
        EXPECT_NE(nullptr, destination_ptr);
        EXPECT_NE(nullptr, dif_config_ptr);

        const auto set_op_status = dml_batch_set_dif_strip_by_index(dml_job_ptr,
                                                                    task_index,
                                                                    source_ptr,
                                                                    length,
                                                                    reinterpret_cast<dml_dif_config_t *>(dif_config_ptr),
                                                                    destination_ptr,
                                                                    length,
                                                                    0u);
        EXPECT_EQ(DML_STATUS_BATCH_TASK_INDEX_OVERFLOW, set_op_status);
    }


    // Test registers
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_nop_by_index, tb_job_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_nop_by_index, tb_index_overflow);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_mem_move_by_index, tb_job_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_mem_move_by_index, tb_source_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_mem_move_by_index, tb_destination_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_mem_move_by_index, tb_index_overflow);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dualcast_by_index, tb_job_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dualcast_by_index, tb_source_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dualcast_by_index, tb_destination_first_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dualcast_by_index, tb_destination_second_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dualcast_by_index, tb_index_overflow);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_compare_by_index, tb_job_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_compare_by_index, tb_source_first_ptr);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_compare_by_index, tb_source_second_ptr);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_compare_by_index, tb_index_overflow);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_compare_pattern_by_index, tb_job_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_compare_pattern_by_index, tb_source_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_compare_pattern_by_index, tb_pattern_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_compare_pattern_by_index, tb_index_overflow);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_crc_by_index, tb_job_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_crc_by_index, tb_source_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_crc_by_index, tb_crc_seed_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_crc_by_index, tb_index_overflow);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_copy_crc_by_index, tb_job_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_copy_crc_by_index, tb_source_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_copy_crc_by_index, tb_destination_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_copy_crc_by_index, tb_crc_seed_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_copy_crc_by_index, tb_index_overflow);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_fill_by_index, tb_job_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_fill_by_index, tb_pattern_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_fill_by_index, tb_destination_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_fill_by_index, tb_index_overflow);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_cache_flush_by_index, tb_job_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_cache_flush_by_index, tb_destination_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_cache_flush_by_index, tb_index_overflow);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_delta_create_by_index, tb_job_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_delta_create_by_index, tb_source_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_delta_create_by_index, tb_reference_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_delta_create_by_index, tb_delta_record_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_delta_create_by_index, tb_index_overflow);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_delta_apply_by_index, tb_job_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_delta_apply_by_index, tb_destination_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_delta_apply_by_index, tb_delta_record_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_delta_apply_by_index, tb_index_overflow);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_check_by_index, tb_job_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_check_by_index, tb_source_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_check_by_index, tb_dif_config_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_check_by_index, tb_index_overflow);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_update_by_index, tb_job_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_update_by_index, tb_source_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_update_by_index, tb_destination_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_update_by_index, tb_dif_config_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_update_by_index, tb_index_overflow);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_insert_by_index, tb_job_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_insert_by_index, tb_source_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_insert_by_index, tb_destination_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_insert_by_index, tb_dif_config_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_insert_by_index, tb_index_overflow);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_strip_by_index, tb_job_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_strip_by_index, tb_source_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_strip_by_index, tb_destination_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_strip_by_index, tb_dif_config_ptr_null);
    DML_UNIT_TEST_REGISTER(unit_dml_batch_set_dif_strip_by_index, tb_index_overflow);

}
