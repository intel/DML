/*
 * Copyright 2020-2021 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they
 * were provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit
 * this software or the related documents without Intel's prior written
 * permission.
 *
 * This software and the related documents are provided as is, with no
 * express or implied warranties, other than those that are expressly
 * stated in the License.
 *
 */

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
