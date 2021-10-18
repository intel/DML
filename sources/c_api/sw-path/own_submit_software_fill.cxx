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
 * @brief Contains an implementation of @ref DML_OP_FILL operation
 *
 * @date 3/6/2020
 *
 */

#ifndef DML_OWN_SUBMIT_SOFTWARE_FILL_H__
#define DML_OWN_SUBMIT_SOFTWARE_FILL_H__

OWN_FUN_INLINE(dml_status_t, sw_fill, (dml_job_t *const dml_job_ptr))
{
    // Bad Argument check
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr->destination_first_ptr)

    // Variables
    uint8_t *const destination_ptr = dml_job_ptr->destination_first_ptr;
    const uint64_t filler          = *((uint64_t *)dml_job_ptr->pattern);
    const uint32_t bytes_to_fill   = dml_job_ptr->destination_length;

    dmlc_fill_with_pattern_8u(filler,destination_ptr, bytes_to_fill);

    return DML_STATUS_OK;
}

#endif //DML_OWN_SUBMIT_SOFTWARE_FILL_H__
