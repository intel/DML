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
 *      - @ref dmlc_own_crc_byte_16u()
 *      - @ref dmlc_own_crc_byte_32u()
 *
 * @date 2/24/2020
 *
 */

DML_CORE_OWN_INLINE(uint16_t, crc_byte_16u, ( uint16_t init_crc,
                                             uint8_t  next_byte,
                                             uint16_t polynomial ) )
{
    // Current crc value
    uint16_t current_crc = init_crc ^ (next_byte << OWN_CRC16_BYTE_SHIFT);

    // Modulo-2 division bit by bit
    for(uint8_t bit = 0u; bit < OWN_BYTE_BIT_LENGTH; ++bit)
    {
        // Check high bit
        current_crc = (current_crc & OWN_HIGH_BIT_MASK_16U) ?
                      // If high bit is set - shift it to the left and XOR with polynomial
                      ((current_crc << 1u) ^ polynomial) :
                      // Else just shift it to the left
                      (current_crc << 1u);
    }

    return current_crc;
}


DML_CORE_OWN_INLINE(uint32_t, crc_byte_32u, ( uint32_t init_crc,
                                             uint8_t  next_byte,
                                             uint32_t polynomial ) )
{
    // Current crc value
    uint32_t current_crc = init_crc ^ (next_byte << OWN_CRC32_BYTE_SHIFT);

    // Modulo-2 division bit by bit
    for(uint8_t bit = 0u; bit < OWN_BYTE_BIT_LENGTH; ++bit)
    {
        // Check high bit
        current_crc = (current_crc & OWN_HIGH_BIT_MASK_32U) ?
                      // If high bit is set - shift it to the left and XOR with polynomial
                      ((current_crc << 1u) ^ polynomial) :
                      // Else just shift it to the left
                      (current_crc << 1u);
    }

    return current_crc;
}
