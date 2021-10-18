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
 * @brief Contains an implementation of @ref dml_check_job
 * @date 3/20/2020
 *
 */
#include "dml/dml.h"
#include "own_dml_api.h"
#include "own_dml_batch.h"
#include "own_dml_internal_state.h"
#if defined(DML_HW)
    #include "hardware_api.h"
    #include "dml_hw_get_operation_result.cxx"
#endif


DML_FUN(dml_status_t, dml_check_job, (dml_job_t *const dml_job_ptr))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)

    own_dml_state_t *state_ptr = OWN_GET_JOB_STATE_PTR(dml_job_ptr);
    dml_status_t status        = DML_STATUS_OK;

#if defined(DML_HW)
    if (DML_PATH_HW == state_ptr->active_path)
    {
        if (0u == state_ptr->hw_operation.result_ptr->status)
        {
            return DML_STATUS_BEING_PROCESSED;
        }

        return idml_hw_get_operation_result(state_ptr->hw_operation.result_ptr, state_ptr->hw_batch_buffers.results_ptr, dml_job_ptr);
    }
#endif

    return status;
}
