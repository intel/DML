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
 * @brief Contains an implementation of @ref DML_OP_BATCH operation
 *
 * @date 1/14/2021
 *
 */

OWN_FUN_INLINE(dml_status_t, sw_batch, (dml_job_t *const dml_job_ptr))
{
    uint32_t size = dml_job_ptr->destination_length / OWN_BATCH_TASK_SIZE;


    for (uint32_t index = 0; index < size; ++index)
    {
        dml_job_t * job = &OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, OWN_BATCH_TASK_SIZE * index)->dml_job;
        dml_status_t * status = &OWN_BATCH_GET_TASK_BY_INDEX(dml_job_ptr, OWN_BATCH_TASK_SIZE * index)->status;

        *(status) = idml_sw_submit_job(job);

        // todo: Need to add conditional batch processing termination (false predicate)
        if (DML_STATUS_OK != *status)
        {
            return DML_STATUS_BATCH_ERROR;
        }
    }

    return DML_STATUS_OK;
}
