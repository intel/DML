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
 * @brief Contain DML job parser
 * @date 3/2/2020
 *
 */
#include "own_dml_api.h"
#include "own_dml_batch.h"
#include "own_dml_internal_state.h"
#if defined(DML_HW)
    #include "hardware_api.h"
    #include "dml_hw_init_descriptor.cxx"
#endif


DML_FUN(dml_status_t, dml_submit_job, (dml_job_t *const dml_job_ptr))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)

    own_dml_state_t *state_ptr = (own_dml_state_t *) dml_job_ptr->internal_data_ptr;

    switch (state_ptr->active_path)
    {
    #if defined(DML_HW)
        case DML_PATH_HW:
        {
            dml_status_t status                         = DML_STATUS_OK;
            dsahw_completion_record_t *result_ptr       = state_ptr->hw_operation.result_ptr;
            dsahw_descriptor_t *descriptor_ptr          = state_ptr->hw_operation.descriptor_ptr;
            own_dml_hw_batch_buffer_t *batch_buffer_ptr = &state_ptr->hw_batch_buffers;

            status = idml_hw_init_descriptor(dml_job_ptr,
                                             result_ptr,
                                             batch_buffer_ptr,
                                             descriptor_ptr);

            if (DML_STATUS_OK != status)
            {
                return status;
            }

            return dsa_submit((dsahw_context_t *)state_ptr->hw_state_ptr, descriptor_ptr, dml_job_ptr->flags);
        }

        case DML_PATH_SW:
        case DML_PATH_AUTO:
    #endif
        default:
            return idml_sw_submit_job(dml_job_ptr);
    }
}
