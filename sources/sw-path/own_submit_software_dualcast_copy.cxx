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
 * @brief Contains a software implementation of @ref DML_OP_DUALCAST operation
 * @date 3/12/2020
 *
 */

OWN_FUN_INLINE(dml_status_t, sw_dualcast_copy, (dml_job_t *const dml_job_ptr))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr->source_first_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr->destination_first_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr->destination_second_ptr)
    DML_BAD_ARGUMENT_RETURN(((((uint64_t)dml_job_ptr->destination_first_ptr) & 0xFFFu) !=
                            (((uint64_t)dml_job_ptr->destination_second_ptr) & 0xFFFu)),
                            DML_STATUS_DUALCAST_ALIGN_ERROR)
    DML_BAD_ARGUMENT_BUFFERS_OVERLAPPING(dml_job_ptr->source_first_ptr,
                                         dml_job_ptr->source_length,
                                         dml_job_ptr->destination_first_ptr,
                                         dml_job_ptr->source_length)
    DML_BAD_ARGUMENT_BUFFERS_OVERLAPPING(dml_job_ptr->source_first_ptr,
                                         dml_job_ptr->source_length,
                                         dml_job_ptr->destination_second_ptr,
                                         dml_job_ptr->source_length)
    DML_BAD_ARGUMENT_BUFFERS_OVERLAPPING(dml_job_ptr->destination_first_ptr,
                                         dml_job_ptr->source_length,
                                         dml_job_ptr->destination_second_ptr,
                                         dml_job_ptr->source_length)
    
    // Variables
    uint8_t *const source_ptr             = dml_job_ptr->source_first_ptr;
    uint8_t *const destination_first_ptr  = dml_job_ptr->destination_first_ptr;
    uint8_t *const destination_second_ptr = dml_job_ptr->destination_second_ptr;
    const uint32_t bytes_to_copy          = dml_job_ptr->source_length;

    return dmlc_dualcast_copy_8u(source_ptr,
                                 destination_first_ptr,
                                 destination_second_ptr,
                                 bytes_to_copy);
}