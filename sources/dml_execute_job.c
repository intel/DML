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
 * @brief Contains an implementation of @ref dml_execute_job function
 * @date 3/13/2020
 *
 */

#include "dml.h"
#include "own_dml_api.h"
#include "own_dml_internal_state.h"

DML_FUN(dml_status_t, dml_execute_job, (dml_job_t *const dml_job_ptr))
{
    dml_status_t status = dml_submit_job(dml_job_ptr);

    DML_RETURN_IN_CASE_OF_ERROR(status)

#if defined(DML_HW)
    if (DML_PATH_HW == OWN_GET_JOB_STATE_PTR(dml_job_ptr)->active_path)
    {
        status = dml_wait_job(dml_job_ptr);
    }
#endif

    return status;
}