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
 *
 * @defgroup dml_job_private Own API
 * @ingroup dml_job_api
 * @{
 *
 * @defgroup dml_job_private_definitions Private Definitions
 * @ingroup dml_job_private
 * @{
 * @brief Common Types and Macro Definitions for Intel® Data Mover Library (Intel® DML)
 *
 */
#if defined (DML_HW)
    #include "hardware_definitions.h"
#else

#endif
#include "dml/dml.h"
#include "own_dml_checkers.h"

#ifndef DML_OWN_DML_DEFINES_H__
#define DML_OWN_DML_DEFINES_H__

#define OWN_OS_PAGE_SIZE 0x1000u     /**< Typical page size for most operating systems */

/* Define NULL pointer value */
#ifndef NULL
#ifdef  __cplusplus
        #define NULL    0
    #else
        #define NULL    ((void *)0)
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ------ Macros ------ */

/**
 * @brief Macros for internal errors handling
 */
#define DML_RETURN_IN_CASE_OF_ERROR(status) \
if ((status) && ((status) != DML_STATUS_FALSE_PREDICATE_OK))\
{\
    return (status);\
}


/**
 * @brief Macros for internal success status handling
 */
#define DML_RETURN_IN_CASE_OF_SUCCESS(status) \
if (((status) == DML_STATUS_OK) || ((status) == DML_STATUS_FALSE_PREDICATE_OK))\
{\
    return (status);\
}


/**
 * @brief Function prototype handlers
 */

#define DML_FUN(type, name, arg) extern type DML_STDCALL name arg

#define OWN_API(type, name, arg) type DML_STDCALL idml_##name arg;

#define OWN_FUN(type, name, arg) extern type DML_STDCALL idml_##name arg

#define OWN_API_INLINE(type, name, arg) type static inline idml_##name arg;

#define OWN_FUN_INLINE(type, name, arg) type static inline idml_##name arg

/* ------ Enumerations ------ */

#ifndef DML_HW

/**
 * @brief Enumerate a structure guards for internal dml_structures
 */
typedef enum
{
    OWN_HW_STATE_GUARD     = 0x42726961,
    OWN_SW_STATE_GUARD     = 0x46726564,
    OWN_BATCH_CONFIG_GUARD = 0x4A6F686E,
} own_dml_structure_id_t;

#endif

/**
 * @brief DML boolean type
 */
typedef enum
{
    OWN_FALSE = 0u,    /**< Bool True  */
    OWN_TRUE  = 1u     /**< Bool False */
} dml_bool_t;


/* ------ Structures ------ */

/**
 * @brief Contains information about some buffer
 */
typedef struct
{
    uint8_t *buffer_ptr; /**< Buffer start */
    uint32_t size;       /**< Buffer size */
} own_dml_buffer_t;

#ifdef __cplusplus
}
#endif

#endif //DML_OWN_DML_DEFINES_H__

/**
 * @} Close DML Job API Private Definitions
 * @} Close DML Job API Private API group
 *
 */
