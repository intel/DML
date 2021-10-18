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
 * @date 3/12/2020
 *
 * @defgroup SW_PATH_COMMON Common Functions API
 * @ingroup SW_PATH
 * @{
 * @brief Contains Common DML features for some operations
 *
 */

#include "own_dml_api.h"

#ifndef DML_OWN_DML_SOFTWARE_COMMON_API_HPP__
#define DML_OWN_DML_SOFTWARE_COMMON_API_HPP__

/**
 * @brief Reverses a specified 32-bit value
 *
 * @param[in] value  value to reverse
 *
 * @return reversed 32-bit value
 */
OWN_API_INLINE(uint32_t, sw_bit_reverse_32u, (uint32_t value))


/**
 * @brief Changes byte order of 16-bit value between little-endian and big-endian forms
 *
 * @param[in] value  value to reverse
 *
 * @return reversed 16-bit value
 */
OWN_API_INLINE(uint16_t, sw_reverse_bytes_16u, (uint16_t value))


/**
 * @brief Changes byte order of 32-bit value between little-endian and big-endian forms
 *
 * @param[in] value  value to reverse
 *
 * @return reversed 32-bit value
 */
OWN_API_INLINE(uint32_t, sw_reverse_bytes_32u, (uint32_t value))

#endif //DML_OWN_DML_SOFTWARE_COMMON_API_HPP__

/** @} */
