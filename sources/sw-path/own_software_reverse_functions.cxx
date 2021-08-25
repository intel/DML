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
 * @brief Contains an implementation features for value reversing
 * @date 3/12/2020
 *
 */

OWN_FUN_INLINE(uint32_t, sw_bit_reverse_32u, (uint32_t value))
{
    value = (value & 0x55555555u) << 1u  | (value & 0xAAAAAAAAu) >> 1u;
    value = (value & 0x33333333u) << 2u  | (value & 0xCCCCCCCCu) >> 2u;
    value = (value & 0x0F0F0F0Fu) << 4u  | (value & 0xF0F0F0F0u) >> 4u;
    value = (value & 0x00FF00FFu) << 8u  | (value & 0xFF00FF00u) >> 8u;
    value = (value & 0x0000FFFFu) << 16u | (value & 0xFFFF0000u) >> 16u;

    return value;
}


OWN_FUN_INLINE(uint16_t, sw_reverse_bytes_16u, (uint16_t value))
{
    union
    {
        uint16_t value;
        uint8_t  bytes[2];
    } received_value, reversed_value;

    received_value.value = value;

    reversed_value.bytes[0] = received_value.bytes[1];
    reversed_value.bytes[1] = received_value.bytes[0];

    return reversed_value.value;
}


OWN_FUN_INLINE(uint32_t, sw_reverse_bytes_32u, (uint32_t value))
{
    union
    {
        uint32_t value;
        uint8_t  bytes[4];
    } received_value, reverced_value;

    received_value.value = value;

    reverced_value.bytes[0] = received_value.bytes[3];
    reverced_value.bytes[1] = received_value.bytes[2];
    reverced_value.bytes[2] = received_value.bytes[1];
    reverced_value.bytes[3] = received_value.bytes[0];

    return reverced_value.value;
}
