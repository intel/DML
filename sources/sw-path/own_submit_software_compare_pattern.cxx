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
 * @brief Contains an implementation of @ref DML_OP_COMPARE_PATTERN operation
 * @date 3/10/2020
 *
 */

OWN_FUN_INLINE(dml_status_t, sw_compare_with_pattern, (dml_job_t *const dml_job_ptr))
{
    // Bad Argument check
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr->source_first_ptr)

    // Variables
    uint8_t *const vector_ptr = dml_job_ptr->source_first_ptr;
    pattern_t pattern = *((pattern_t*) dml_job_ptr->pattern);
    uint32_t *const offset_ptr = &dml_job_ptr->offset;
    const uint32_t bytes_to_compare = dml_job_ptr->source_length;

    dmlc_status_t compare_result = dmlc_compare_with_pattern_8u(vector_ptr,
                                                                pattern,
                                                                bytes_to_compare,
                                                                offset_ptr);

    return idml_sw_check_result(dml_job_ptr, compare_result);
}