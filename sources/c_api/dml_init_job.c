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
 * @brief Contains an implementation of @ref dml_init_job function
 * @date 3/13/2020
 *
 */
#if defined(DML_HW)
    #include "hardware_api.h"
#endif
#include "own_dml_api.h"
#include "own_dml_align.h"
#include "own_dml_internal_state.h"
#include "core_memory.h"


OWN_API_INLINE(dml_status_t, init_batch, (own_dml_batch_info_t *const batch_info_ptr,
                                          const uint32_t batch_max_size,
                                          uint8_t *const buffer_ptr))

/**
 * @brief Inits internal structure for software path
 */
OWN_API_INLINE(dml_status_t, sw_init, (uint8_t *const hw_state_ptr))

DML_FUN(dml_status_t, dml_init_job, (const dml_path_t path, dml_job_t *const dml_job_ptr))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr)
    DML_BAD_ARGUMENT_INCORRECT_PATH(path)

    // Constants
    const uint32_t dml_state_size      = DML_ALIGNED_SIZE(sizeof(own_dml_state_t), DML_DEFAULT_ALIGNMENT);
    const uint32_t software_state_size = DML_ALIGNED_SIZE(idml_sw_get_state_size(), DML_DEFAULT_ALIGNMENT);
    const uint32_t hardware_state_size = DML_ALIGNED_SIZE(0u, DML_DEFAULT_ALIGNMENT);

    // Need to consider internal_data_ptr offset
    const uint32_t dml_unused_space = DML_BYTES_TO_ALIGN(((uint8_t *) dml_job_ptr) + sizeof(dml_job_t),
                                                         DML_DEFAULT_ALIGNMENT);

    const uint32_t dml_job_size_with_aligned_end = sizeof(dml_job_t) + dml_unused_space;

    const uint32_t total_job_size = dml_job_size_with_aligned_end
                                    + dml_state_size
                                    + ((path != DML_PATH_HW) ? software_state_size : 0u)
                                    + ((path != DML_PATH_SW) ? hardware_state_size : 0u)
#if defined (DML_HW)
                                    + (DML_PATH_HW == path) ? (OWN_HW_INTERNAL_BUFFERS_SIZE) : (0u)
#endif
;

    // Variables
    dml_status_t status;
    own_dml_state_t *state_ptr;

    // Fill Job by zero
    dmlc_fill_with_pattern_8u(0u, (uint8_t *) dml_job_ptr, total_job_size);

    // Set internal pointers
    dml_job_ptr->internal_data_ptr = ((uint8_t *) dml_job_ptr) + dml_job_size_with_aligned_end;

    state_ptr = (own_dml_state_t *) dml_job_ptr->internal_data_ptr;

    state_ptr->sw_state_ptr                     = dml_job_ptr->internal_data_ptr + dml_state_size;
    state_ptr->hw_state_ptr                     = state_ptr->sw_state_ptr + ((path != DML_PATH_HW) ? software_state_size : 0u);
#if defined(DML_HW)
    state_ptr->hw_operation.descriptor_ptr      = DML_ALIGNED_PTR(state_ptr->hw_state_ptr + hardware_state_size, DSA_HW_MAX_DESCRIPTOR_ALIGNMENT);
    state_ptr->hw_operation.result_ptr          = DML_ALIGNED_PTR(((uint8_t *)state_ptr->hw_operation.descriptor_ptr) + DSA_HW_DESCRIPTOR_SIZE, DSA_HW_MAX_RESULT_ALIGNMENT);
    state_ptr->hw_batch_buffers.descriptors_ptr = DML_ALIGNED_PTR(((uint8_t *)state_ptr->hw_operation.result_ptr) + DSA_HW_COMPLETION_RECORD_SIZE, DSA_HW_MAX_DESCRIPTOR_ALIGNMENT);
    state_ptr->hw_batch_buffers.results_ptr     = DML_ALIGNED_PTR(((uint8_t *)state_ptr->hw_batch_buffers.descriptors_ptr) + DML_HW_BATCH_DESCRIPTORS_SIZE, DSA_HW_MAX_RESULT_ALIGNMENT);
#endif

    // Save active path
    state_ptr->active_path  = path;

    // Init for internal path states only
    switch (path)
    {
#if defined(DML_HW)
        case DML_PATH_HW:
            status = dsa_get_context((dsahw_context_t **)&(state_ptr->hw_state_ptr));
            break;

        case DML_PATH_SW:
        case DML_PATH_AUTO:
#endif
        default:
            status = idml_sw_init(state_ptr->sw_state_ptr);
    }

    DML_RETURN_IN_CASE_OF_ERROR(status)

    status = idml_init_batch(&state_ptr->batch_info, OWN_SOFTWARE_BATCH_SIZE, NULL);

    return status;
}

OWN_FUN_INLINE(dml_status_t, init_batch, (own_dml_batch_info_t *const batch_info_ptr,
                                          const uint32_t batch_max_size,
                                          uint8_t *const buffer_ptr))
{
    // Internal Buffers
    own_dml_buffer_t *descriptors = &batch_info_ptr->descriptors;
    own_dml_buffer_t *completion_records = &batch_info_ptr->completion_records;

    // Set Batch Info
    batch_info_ptr->guard    = OWN_BATCH_CONFIG_GUARD;
    batch_info_ptr->size     = 0u;
    batch_info_ptr->max_size = batch_max_size;

    // Init Batch buffers
    // descriptors->size              = batch_max_size; // Count of hardware descriptors
    // descriptors->buffer_ptr        = buffer_ptr;
    // completion_records->size       = batch_max_size; // Count of hardware descriptors
    // completion_records->buffer_ptr = buffer_ptr + (descriptors->size * HW_DESCRIPTOR_SIZE);

    return DML_STATUS_OK;
}


OWN_FUN_INLINE(dml_status_t, sw_init, (uint8_t *const sw_state_ptr))
{
    // Variables
    own_sw_state_t *sw_state = (own_sw_state_t *) sw_state_ptr;

    // Initialization
    sw_state->guard = OWN_SW_STATE_GUARD;

    return DML_STATUS_OK;
}
