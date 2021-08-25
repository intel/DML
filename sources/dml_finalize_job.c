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
 * @brief Contains an implementation of @ref dml_finalize_job function
 * @date 3/24/2020
 *
 */

#include "own_dml_internal_state.h"

#if defined(DML_HW)
    #include "hardware_api.h"
#endif

DML_FUN(dml_status_t, dml_finalize_job, (dml_job_t *const dml_job_ptr))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)

    // Variables
    dml_status_t status = DML_STATUS_OK;
    own_dml_state_t *const state = OWN_GET_JOB_STATE_PTR(dml_job_ptr);

    // Free resources in case if hardware path used
#if defined(DML_HW)
    if (DML_PATH_SW != state->active_path)
    {
        status = dsa_finalize((dsahw_context_t *)&(state->hw_state_ptr));
    }
#endif

    return status;
}
