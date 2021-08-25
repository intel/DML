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
 * @brief Contains a software implementation of @ref DML_OP_CACHE_FLUSH operation
 * @date 3/12/2020
 *
 */

OWN_FUN_INLINE(dml_status_t, sw_cache_flush, (dml_job_t *const dml_job_ptr))
{
    DML_BAD_ARGUMENT_NULL_POINTER(dml_job_ptr->destination_first_ptr);

    uint8_t *const memory_region_ptr  = dml_job_ptr->destination_first_ptr;
    const uint32_t memory_region_size = dml_job_ptr->destination_length;
    dml_operation_flags_t flags       = dml_job_ptr->flags;

    return (flags & DML_FLAG_DONT_INVALIDATE_CACHE) ?
                            dmlc_copy_cache_to_memory_8u(memory_region_ptr, memory_region_size) :
                            dmlc_move_cache_to_memory_8u(memory_region_ptr, memory_region_size);
}
