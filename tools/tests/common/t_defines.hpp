/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @file
 * @brief Contain necessary constants and macros which can be used for test development
 * @date 2/13/2020
 *
 */

#ifndef DML_T_DEFINES_HPP__
#define DML_T_DEFINES_HPP__

/* ------ World Constants Definitions ------ */
constexpr uint8_t BYTE_BIT_LENGTH = 8u;           /**< Byte bit-length*/

constexpr uint32_t KILOBYTE  = 1024u;             /**< One Kilobyte size in bytes */
constexpr uint32_t PAGE_SIZE = 4u * KILOBYTE;     /**< Typical virtual page size used by OS */


/* ------ Tests Common Constants ------ */

constexpr uint32_t TEST_MIN_MEMORY_BLOCK_SIZE = KILOBYTE / 2u;                          /**< Minimal size of memory region to allocation in test*/
constexpr uint32_t TEST_MAX_MEMORY_BLOCK_SIZE = PAGE_SIZE + TEST_MIN_MEMORY_BLOCK_SIZE; /**< MaximaL size of memory region to allocation in test*/


/* ------ Test Macros ------ */

#define TEST_LAST_FROM(count) ((count) - 1u)  /**< Simple macros to calculate last vector index */

#endif //DML_T_DEFINES_HPP__
