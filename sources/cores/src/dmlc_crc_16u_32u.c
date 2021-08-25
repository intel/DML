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
 * @brief Contain implementation of the follow functions:
 *      - @ref dmlc_calculate_crc_16u()
 *      - @ref dmlc_calculate_crc_32u()
 *      - @ref dmlc_calculate_crc_reflected_32u()
 *
 * @date 2/5/2020
 *
 */


#include "core_hash_functions.h"
#include "own_dmlc_definitions.h"
#include "own_dmlc_crc_16u_32u.cxx"
#include "own_dmlc_byte_op.cxx"

#if defined(AVX512)
#include "avx512/dmlc_crc_16u_32u_k0.cxx"
#else
#include "default/dmlc_crc_16u_32u_px.cxx"
#endif



DML_CORE_API(dmlc_status_t, calculate_crc_16u, (const uint8_t  *const memory_region_ptr,
                                                      uint32_t bytes_to_hash,
                                                      uint16_t *const crc_ptr,
                                                      uint16_t polynomial))
{
    // Check input arguments
    DML_CORE_CHECK_NULL_POINTER(memory_region_ptr)
    DML_CORE_CHECK_NULL_POINTER(crc_ptr)

    // Current crc value
    uint16_t current_crc = (*crc_ptr);

    // Through all bytes
    for(uint32_t i = 0u; i < bytes_to_hash; ++i)
    {
        // Calculate crc for current byte
        current_crc  = dmlc_own_crc_byte_16u(current_crc, memory_region_ptr[i], polynomial);
    }

    // Store result
    (*crc_ptr) = current_crc;

    return DML_STATUS_OK;
}

#if defined(_MSC_VER)
#define BORDER_OPT  256
#else
#define BORDER_OPT  256
#endif


DML_CORE_API(dmlc_status_t, calculate_crc_32u, (const uint8_t  *const memory_region_ptr,
                                                      uint32_t bytes_to_hash,
                                                      uint32_t *const crc_ptr,
                                                      uint32_t polynomial))
{
#if defined(AVX512)
    if (bytes_to_hash < BORDER_OPT)
        return dmlc_own_calculate_crc_32u_noopt(memory_region_ptr, bytes_to_hash, crc_ptr, polynomial);
#endif 
    return dmlc_own_calculate_crc_32u(memory_region_ptr, bytes_to_hash, crc_ptr, polynomial);
}


DML_CORE_API(dmlc_status_t, calculate_crc_reflected_32u, (const uint8_t  *const memory_region_ptr,
                                                                uint32_t bytes_to_hash,
                                                                uint32_t *const crc_ptr,
                                                                uint32_t polynomial))
{
    // Check input arguments
    DML_CORE_CHECK_NULL_POINTER(memory_region_ptr)
    DML_CORE_CHECK_NULL_POINTER(crc_ptr)

    // Current crc value
    uint32_t current_crc = (*crc_ptr);

    // Temporary storage for reversed value
    uint8_t  reversed_value;

    // Through all bytes
    for(uint32_t i = 0u; i < bytes_to_hash; ++i)
    {
        // Get current byte
        reversed_value = memory_region_ptr[i];

        // Reverse bits
        reversed_value = dmlc_own_reverse_8u(reversed_value);

        // Calculate crc for current reversed byte
        current_crc = dmlc_own_crc_byte_32u(current_crc, reversed_value, polynomial);
    }

    // Store result
    (*crc_ptr) = current_crc;

    return DML_STATUS_OK;
}
