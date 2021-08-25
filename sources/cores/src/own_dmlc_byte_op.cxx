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
 *      - @ref dmlc_own_reverse_8u()
 *
 * @date 3/5/2020
 *
 */


DML_CORE_OWN_INLINE(uint8_t, reverse_8u, ( uint8_t byte ) )
{
    // Value to return
    uint8_t reversed_value = byte;

    reversed_value = ((reversed_value & 0x55u) << 1u) | ((reversed_value & 0xAAu) >> 1u);
    reversed_value = ((reversed_value & 0x33u) << 2u) | ((reversed_value & 0xCCu) >> 2u);
    reversed_value = ((reversed_value & 0x0Fu) << 4u) | ((reversed_value & 0xF0u) >> 4u);

    return reversed_value;
}
