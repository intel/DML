/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 *
 * @brief Contains public hardware limits
 *
 */
#include "hardware_definitions.h"

#ifndef DML_HW_DSA_LIMITS_H__
#define DML_HW_DSA_LIMITS_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Maximum number of the descriptors/completion records that can be launched in the single batch
 */
#define DML_HW_MAXIMAL_BATCH_TASKS_COUNT (1024u)

/**
 * @brief Number of bytes used for the descriptors buffer allocation
 */
#define DML_HW_BATCH_DESCRIPTORS_SIZE (DML_HW_MAXIMAL_BATCH_TASKS_COUNT * DSA_HW_DESCRIPTOR_SIZE)

/**
 * @brief Number of bytes used for the completion records allocation
 */
#define DML_HW_BATCH_RESULTS_SIZE (DML_HW_MAXIMAL_BATCH_TASKS_COUNT * DSA_HW_COMPLETION_RECORD_SIZE)

/**
 * @brief Maximum number of bytes needed for descriptor alignment
 */
#define DSA_HW_MAX_DESCRIPTOR_ALIGNMENT (64u)

/**
 * @brief Maximum number of bytes needed for completion record alignment
 */
#define DSA_HW_MAX_RESULT_ALIGNMENT (32u)

#ifdef __cplusplus
}
#endif

#endif  // DML_HW_DSA_LIMITS_H__
