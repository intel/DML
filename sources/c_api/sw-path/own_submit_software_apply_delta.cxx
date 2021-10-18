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
 * @brief Contains a software implementation of @ref DML_OP_DELTA_APPLY operation
 * @date 3/11/2020
 *
 */

OWN_FUN_INLINE(dml_status_t, sw_apply_delta, (dml_job_t *const dml_job_ptr))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr->source_first_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr->destination_first_ptr)

    // Variables
    uint8_t *const vector_to_update_ptr  = dml_job_ptr->destination_first_ptr;
    uint8_t *const delta_record_ptr      = dml_job_ptr->source_first_ptr;
    const uint32_t vector_to_update_size = dml_job_ptr->destination_length;
    const uint32_t delta_record_size     = dml_job_ptr->source_length;

    // Call to DML Core function
    return dmlc_apply_delta_record_8u(vector_to_update_ptr,
                                      delta_record_ptr,
                                      vector_to_update_size,
                                      delta_record_size);
}
