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
 * @brief Implement the dml feature is provided by @ref DML_FLAG_CHECK_RESULT flag
 * @date 3/11/2020
 *
 */

OWN_FUN_INLINE(dml_status_t, sw_check_result, (dml_job_t *const dml_job_ptr, const dmlc_status_t result))
{
    // Variables
    dml_status_t status;
    const uint32_t expected_result = dml_job_ptr->expected_result;
    const uint32_t checked_result  = (DML_COMPARE_STATUS_NE == result) ? 1u: 0u;

    if (dml_job_ptr->flags & DML_FLAG_CHECK_RESULT)
    {
        if (expected_result == checked_result)
        {
            status = DML_STATUS_OK;
        }
        else
        {
            status = DML_STATUS_FALSE_PREDICATE_OK;
        }
    }
    else
    {
        status = DML_STATUS_OK;
    }

    dml_job_ptr->result = checked_result;

    return status;
}
