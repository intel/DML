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
 * @brief Contains a software implementation of @ref DML_OP_DELTA_CREATE operation
 * @date 3/10/2020
 *
 */

OWN_FUN_INLINE(dml_status_t, sw_create_delta, (dml_job_t *const dml_job_ptr))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr->source_first_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr->source_second_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr->destination_first_ptr)

    // Variables
    uint8_t  *const source_vector_ptr     = dml_job_ptr->source_first_ptr;
    uint8_t  *const reference_vector_ptr  = dml_job_ptr->source_second_ptr;
    uint8_t  *const delta_record_ptr      = dml_job_ptr->destination_first_ptr;
    uint32_t *const delta_record_size_ptr = &(dml_job_ptr->destination_length);
    const uint32_t delta_record_max_size  = dml_job_ptr->destination_length;
    const uint32_t compared_bytes_count   = dml_job_ptr->source_length;
    const uint64_t check_result_flag      = (DML_FLAG_CHECK_RESULT & dml_job_ptr->flags);
    const uint32_t check_result_type      = dml_job_ptr->expected_result;

    dml_status_t status = dmlc_create_delta_record_8u(reference_vector_ptr,
                                                      source_vector_ptr,
                                                      compared_bytes_count,
                                                      delta_record_max_size,
                                                      delta_record_ptr,
                                                      delta_record_size_ptr);
    
    // Check for error
    if ((DML_STATUS_NULL_POINTER_ERROR     == status) ||
        (DML_STATUS_DELTA_ALIGN_ERROR      == status) ||
        (DML_STATUS_DELTA_OFFSET_ERROR     == status) ||
        (DML_STATUS_DELTA_INPUT_SIZE_ERROR == status))
    {
        return status;
    }

    if (0u < (*delta_record_size_ptr))
    {
        dml_job_ptr->offset = (uint32_t)(*((uint16_t *)(&delta_record_ptr[0])));
        dml_job_ptr->result = (DML_STATUS_OK == status) ? (1u) : (2u);
    }
    else
    {
        dml_job_ptr->result = (DML_STATUS_OK == status) ? (0u) : (2u);
    }

    if (check_result_flag)
    {
        return (check_result_type & (1u << dml_job_ptr->result)) ?
               (DML_STATUS_OK) :
               (DML_STATUS_FALSE_PREDICATE_OK);
    }

    return status;
}
 
