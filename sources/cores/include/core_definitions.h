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
 * @defgroup core_src Kernel Layer
 * @brief Intel(R) Data Mover Library (Intel® DML) Core functions
 *
 * @details DML Kernels Layer is a software path run on the user CPU, which must implement of the base DML features.
 * The kernel is an atomic function without any dependencies on the upper layers of abstraction
 *
 * @defgroup core_public Public API
 * @ingroup core_src
 *
 * @defgroup core_public_definitions Public Definitions
 * @ingroup core_public
 * @{
 *
 * @brief Contains general definitions for public use in Intel® Data Mover Library (Intel® DML) Cores.
 *
 */

#include <stdint.h>
#include "dml/dmldefs.h"

#ifndef KERNEL_DEFINITIONS_H__
#define KERNEL_DEFINITIONS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ------ Definitions ------ */

#if defined( _WIN32 ) || defined ( _WIN64 )
#define DML_CORE_STDCALL  __stdcall
#define DML_CORE_CDECL    __cdecl
#else
#define DML_CORE_STDCALL
#define DML_CORE_CDECL
#endif

/* ------ Macros ------ */
/**
 * @brief Defines an internal function declared in the file scope
 */
#define DML_CORE_OWN_INLINE(type, name, arg) type static inline dmlc_own_##name arg

#if !defined( DML_CORE_API )
#define DML_CORE_API(type, name, arg) type DML_CORE_STDCALL dmlc_##name arg /**< Declaration macros to manipulate function name */
#endif

/* ------ Statuses ------ */

typedef dml_status_t dmlc_status_t; /**< Redefinition of @ref dml_status_t for core functions */

#ifdef __cplusplus
}
#endif

#endif //KERNEL_DEFINITIONS_H__

/** @} */
