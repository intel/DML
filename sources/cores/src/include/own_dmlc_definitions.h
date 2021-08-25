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
 * @file
 * @date 2/10/2020
 *
 * @defgroup core_own Own API
 * @ingroup core_src
 *
 * @defgroup core_own_definition Internal Definitions
 * @ingroup core_own
 * @{
 *
 * @brief Contains general definitions for internal use in Intel® Data Mover Library (Intel® DML) Cores.
 *
 */

#if defined(_MSC_BUILD)
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#else
#error "Unsupported compiler"
#endif

#include "core_definitions.h"
#include "own_dmlc_checkers.h"

#ifndef OWN_KERNEL_DEFINITIONS_H__
#define OWN_KERNEL_DEFINITIONS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Define NULL pointer value */
#ifndef NULL
    #ifdef  __cplusplus
        #define NULL    0
    #else
        #define NULL    ((void *)0)
    #endif
#endif

/* ------ Defines ------ */
#define OWN_BYTE_BIT_LENGTH 8u                /**< Byte bit-length*/
#define OWN_HIGH_BIT_MASK_16U ( 0x8000u )     /**< Mask for checking high bit in uint16 value */
#define OWN_HIGH_BIT_MASK_32U ( 0x80000000u ) /**< Mask for checking high bit in uint16 value */

/* ------ Enumerations ------ */

/**
 * @brief Kernel boolean type
 */
typedef enum
{
    OWN_BOOL_FALSE = 0u,    /**< Bool True  */
    OWN_BOOL_TRUE  = 1u     /**< Bool False */
} kernel_bool_t;

#ifdef __cplusplus
}
#endif

#endif //OWN_KERNEL_DEFINITIONS_H__

/** @} */
