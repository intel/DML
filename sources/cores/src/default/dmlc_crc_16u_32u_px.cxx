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
  *      - @ref dmlc_calculate_crc_32u()
  *
  * @date 7/20/2021
  *
  */

DML_CORE_OWN_INLINE(dmlc_status_t, calculate_crc_32u, (const uint8_t* const memory_region_ptr,
    uint32_t bytes_to_hash,
    uint32_t* const crc_ptr,
    uint32_t polynomial))
{
     // Current crc value
    uint32_t current_crc = (*crc_ptr);

    // Through all bytes
    for (uint32_t i = 0u; i < bytes_to_hash; ++i)
    {
        // Calculate crc for current byte
        current_crc = dmlc_own_crc_byte_32u(current_crc, memory_region_ptr[i], polynomial);
    }

    // Store result
    (*crc_ptr) = current_crc;
    return DML_STATUS_OK;
}
