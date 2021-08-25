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
 * @brief Contain default implementation of the follow functions:
 *      - @ref dmlc_fill_with_pattern_8u()
 *
 * @date 10/29/2020
 *
 */

#include "core_memory.h"
#include "own_dmlc_definitions.h"

DML_CORE_OWN_INLINE(dmlc_status_t, opt_fill_with_pattern_8u, ( uint64_t        pattern,
                                                               uint8_t  *const memory_region_ptr,
                                                               uint32_t        bytes_to_process ) )
{
    DML_CORE_CHECK_NULL_POINTER(memory_region_ptr)

    // Current position in memory region 64u
    uint64_t *memory_region_current_64u_ptr = (uint64_t *)memory_region_ptr;

    // Current pattern 64u to fill with
    const uint64_t *pattern_current_64u_ptr = (const uint64_t *)(&pattern);

    // Current pattern 8u to fill with
    const uint8_t *pattern_current_8u_ptr   = (const uint8_t *)(&pattern);

    // Current position in memory region 8u
    uint8_t *memory_region_current_8u_ptr;

    // Fill body
    while(sizeof(uint64_t) <= bytes_to_process)
    {
        // Put 8 bytes to current memory region
        (*memory_region_current_64u_ptr) =
        (*pattern_current_64u_ptr);

        // Shift position in memory region
        memory_region_current_64u_ptr++;

        // Decrease bytes counter
        bytes_to_process -= sizeof(uint64_t);
    }

    // Get position in memory region
    memory_region_current_8u_ptr = (uint8_t *)memory_region_current_64u_ptr;

    // Fill tail
    while(0 < bytes_to_process)
    {
        // Put 1 byte to current memory region
        (*memory_region_current_8u_ptr) =
        (*pattern_current_8u_ptr);

        // Shift position in memory region
        memory_region_current_8u_ptr++;

        // Shift position in current pattern
        pattern_current_8u_ptr++;

        // Decrease bytes counter
        bytes_to_process -= sizeof(uint8_t);
    }

    // Success
    return DML_STATUS_OK;
}
