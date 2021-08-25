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
 * @date 3/30/2020
 *
 * @defgroup SW_PATH_DIF_FEATURE Data Integrity Field Definitions
 * @ingroup SW_PATH
 * @{
 * @brief Contains Common Definitions for operations in DIF group
 *
 */

#include "own_dml_definitions.h"
#include "core_memory.h"
#include "core_hash_functions.h"

#ifndef DML_OWN_DML_SOFTWARE_DIF_FEATURE_H__
#define DML_OWN_DML_SOFTWARE_DIF_FEATURE_H__


/* ------ DEFINITIONS ------ */

#define OWN_DIF_CRC_POLYNOMIAL 0x8BB7u /**< CRC16 T10 polynomial */

/* ------ MACRO ------ */

#if defined(__GNUC__)
/**
 * @brief Packs a structure with byte by byte
 */
#define OWN_BYTE_PACKED_STRUCTURE_BEGIN \
        typedef struct __attribute__ ((__packed__))
#elif(_MSC_VER)
/**
     * @brief Packs a structure with byte by byte
     */
    #define OWN_BYTE_PACKED_STRUCTURE_BEGIN \
        __pragma(pack(push, 1)) \
        typedef struct
#else
    #error Compiler not supported
#endif

#if defined(__GNUC__)
/**
 * @brief Pops a previous structure pack property
 */
#define OWN_BYTE_PACKED_STRUCTURE_END
#elif(_MSC_VER)
/**
    * @brief Pops a previous structure pack property
    */
    #define OWN_BYTE_PACKED_STRUCTURE_END \
        __pragma(pack(pop))
#else
    #error Compiler not supported
#endif

/* ------ STRUCTURES ------ */

/**
 * @brief Description of Data Integrity Field (DIF)
 */
OWN_BYTE_PACKED_STRUCTURE_BEGIN
{
    uint16_t guard_tag;         /**< CRC16 checksum                              */
    uint16_t application_tag;   /**< Teg used to set compliance with application */
    uint32_t reference_tag;     /**< Teg used to set compliance with data group  */
} own_dif_t;
OWN_BYTE_PACKED_STRUCTURE_END

extern uint32_t own_dif_block_sizes[]; /**< Contain associative data block size for protection with DIF */

#endif //DML_OWN_DML_SOFTWARE_DIF_FEATURE_H__

/** @} */
