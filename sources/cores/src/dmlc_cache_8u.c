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
 * @brief Contain implementation of functions for cache manipulation
 * @date 2/25/2020
 *
 * @details Function list:
 *          - @ref dmlc_move_cache_to_memory
 *          - @ref dmlc_copy_cache_to_memory
 *
 */


#include "core_cpu_features.h"
#include "own_dmlc_definitions.h"

#define OWN_CACHE_LINE_BYTE_SIZE 64u

DML_CORE_API(dmlc_status_t, move_cache_to_memory_8u, (const uint8_t  *memory_region_ptr,
                                                      const uint32_t bytes_to_flush))
{
    DML_CORE_CHECK_NULL_POINTER(memory_region_ptr)

    // Constants
    const uint64_t start_address    = (uint64_t) memory_region_ptr;
    const uint32_t cache_align      = OWN_CACHE_LINE_BYTE_SIZE - (start_address % OWN_CACHE_LINE_BYTE_SIZE);
    const uint32_t cache_line_count = (bytes_to_flush + OWN_CACHE_LINE_BYTE_SIZE) / OWN_CACHE_LINE_BYTE_SIZE - 1u;

    // Variables
    void *memory_address_ptr = (void*) memory_region_ptr;

    #if !defined (PX)
    _mm_clflushopt(memory_address_ptr);
    #else
    _mm_clflush(memory_address_ptr);
    #endif

    // Align pointer
    memory_region_ptr += cache_align;

    for (uint32_t i = 0u; i < cache_line_count; i++)
    {
        #if !defined (PX)
        _mm_clflushopt(memory_address_ptr);
        #else
        _mm_clflush(memory_address_ptr);
        #endif
        memory_region_ptr += OWN_CACHE_LINE_BYTE_SIZE;
    }

    return DML_STATUS_OK;
}


DML_CORE_API(dmlc_status_t, copy_cache_to_memory_8u, (const uint8_t  *memory_region_ptr,
                                                           const uint32_t bytes_to_flush))
{
    DML_CORE_CHECK_NULL_POINTER(memory_region_ptr)

    #if !defined (PX)
    // Constants
    const uint64_t start_address    = (uint64_t) memory_region_ptr;
    const uint32_t cache_align      = OWN_CACHE_LINE_BYTE_SIZE - (start_address % OWN_CACHE_LINE_BYTE_SIZE);
    const uint32_t cache_line_count = (bytes_to_flush + OWN_CACHE_LINE_BYTE_SIZE) / OWN_CACHE_LINE_BYTE_SIZE - 1u;

    // Variable
    void *memory_address_ptr = (void*) memory_region_ptr;

    _mm_clwb(memory_address_ptr);

    // Align pointer
    memory_region_ptr += cache_align;

    for (uint32_t i = 0u; i < cache_line_count; i++)
    {
        _mm_clwb(memory_address_ptr);
        memory_region_ptr += OWN_CACHE_LINE_BYTE_SIZE;
    }
    #endif

    return DML_STATUS_OK;
}
