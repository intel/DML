/*******************************************************************************
 * Copyright (C) 2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TOOLS_UTILS_HW_DISPATCHER_TEST_HW_STATUS_H_
#define DML_TOOLS_UTILS_HW_DISPATCHER_TEST_HW_STATUS_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumerates the hardware accelerator statuses
 */
typedef enum {
    DML_TEST_HW_ACCELERATOR_STATUS_OK          = 0U, /**< Accelerator returned success */
    DML_TEST_HW_ACCELERATOR_SUPPORT_ERR        = 1U, /**< System doesn't support accelerator */
    DML_TEST_HW_ACCELERATOR_LIBACCEL_NOT_FOUND = 2U, /**< Required version of libaccel-config is not found */
    DML_TEST_HW_ACCELERATOR_LIBACCEL_ERROR     = 3U, /**< Accelerator instance can not be found */
    DML_TEST_HW_ACCELERATOR_WORK_QUEUES_NOT_AVAILABLE =
            4U,                                /**< Enabled work queues are not found or no enabled devices */
    DML_TEST_HW_ACCELERATOR_NULL_PTR_ERR = 5U, /**< Null pointer error */
    DML_TEST_HW_ACCELERATOR_WQ_IS_BUSY   = 6U, /**< Work queue is busy with task processing */
} dml_test_hw_accelerator_status;

#ifdef __cplusplus
}
#endif

#endif //DML_TOOLS_UTILS_HW_DISPATCHER_TEST_HW_STATUS_H_
