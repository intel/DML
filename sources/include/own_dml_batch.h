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
 * @brief Contains batch task structure
 * @date 9/8/2020
 *
 */
#include "dmldefs.h"


#define OWN_BATCH_TASK_SIZE (sizeof(own_batch_task_t))

#define OWN_BATCH_GET_TASK_BY_INDEX(job_ptr, task_position) ((own_batch_task_t *)(&(job_ptr)->destination_first_ptr[(task_position)]))

/**
 * @brief Represents internal descriptor for the @ref DML_OP_BATCH operation.
 */
typedef struct
{
    dml_job_t    dml_job; /**< Parameters of the operation */
    dml_status_t status;  /**< Status of the operation     */
} own_batch_task_t;
