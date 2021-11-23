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
  *      - @ref dmlc_own_copy_8u()
  *      - @ref dmlc_own_move_8u()
  *      - @ref dmlc_own_dualcast_copy_8u()
  *
  * @date 5/26/2021
  *
  */

DML_CORE_OWN_INLINE(void, copy_8u, (const uint8_t *const source_ptr,
    uint8_t *const destination_ptr,
    uint32_t       bytes_to_process))
{
    // Current position in source vector
    const uint8_t *source_current_ptr = (const uint8_t *)source_ptr;

    // Current position in destination vector
    uint8_t *destination_current_ptr = (uint8_t *)destination_ptr;

    while (0u < bytes_to_process)
    {
        // Copy 1 byte
        *destination_current_ptr = *source_current_ptr;

        // Shift position in destination vector
        destination_current_ptr++;

        // Shift position in source vector
        source_current_ptr++;

        // Decrease bytes counter
        bytes_to_process -= sizeof(uint8_t);
    }
}


DML_CORE_OWN_INLINE(void, move_8u, (const uint8_t *const source_ptr,
    uint8_t *const destination_ptr,
    uint32_t       bytes_to_process))
{
    // Current position in source vector
    const uint8_t *source_current_ptr = (const uint8_t *)(source_ptr + bytes_to_process);

    // Current position in destination vector
    uint8_t *destination_current_ptr = (uint8_t *)(destination_ptr + bytes_to_process);

    while (0u < bytes_to_process)
    {
        // Shift position in destination vector
        destination_current_ptr--;

        // Shift position in source vector
        source_current_ptr--;

        // Copy 1 byte
        (*destination_current_ptr) = (*source_current_ptr);

        // Decrease bytes counter
        bytes_to_process -= sizeof(uint8_t);
    }
}


DML_CORE_OWN_INLINE(void, dualcast_copy_8u, (const uint8_t *const source_ptr,
    uint8_t *const first_destination_ptr,
    uint8_t *const second_destination_ptr,
    uint32_t       bytes_to_process))
{
    // Current position in source vector 64u
    const uint8_t *source_current_ptr = (const uint8_t *)source_ptr;

    // Current position in first destination vector 64u
    uint8_t *first_destination_current_ptr = (uint8_t *)first_destination_ptr;

    // Current position in second destination vector 64u
    uint8_t *second_destination_current_ptr = (uint8_t *)second_destination_ptr;

    while (0 < bytes_to_process)
    {
        // Copy 1 byte to first destination vector
        (*first_destination_current_ptr) = (*source_current_ptr);

        // Copy 1 byte to second destination vector
        (*second_destination_current_ptr) = (*source_current_ptr);

        // Shift position in first destination vector
        first_destination_current_ptr++;

        // Shift position in second destination vector
        second_destination_current_ptr++;

        // Shift position in source vector
        source_current_ptr++;

        // Decrease bytes counter
        bytes_to_process -= sizeof(uint8_t);
    }
}
