/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 *
 * @brief Contains public hardware definitions
 *
 */

#include "dml/dml.h"
#include "dml/dmldefs.h"

#if defined(linux)
#include "libaccel_config.h"
#endif

#ifndef DSA_HW_DSA_DEFINES_H__
#define DSA_HW_DSA_DEFINES_H__

#ifdef LOG_HW_INIT
#include <stdio.h>
#define DIAGA(...) printf(__VA_ARGS__); fflush(stdout)                  /**< Diagnostic printer for appending to line */
#define DIAG(...) printf("dml-diag: " __VA_ARGS__); fflush(stdout)      /**< Diagnostic printer */
#else
#define DIAGA(...)                                                      /**< Diagnostic printer for appending to line */
#define DIAG(...)                                                       /**< Diagnostic printer */
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ------ Definitions ------ */

#if defined(_WIN32) || defined(_WIN64)
#define DML_HW_STDCALL __stdcall
#define DML_HW_CDECL   __cdecl
#else
#define DML_HW_STDCALL
#define DML_HW_CDECL
#endif

/* ------ Macros ------ */

#if !defined(DML_HW_API)
#define DML_HW_API(name) DML_HW_STDCALL dsa_##name /**< Declaration macros to manipulate function name */
#endif

/* ------ Statuses ------ */

typedef dml_status_t dsahw_status_t; /**< Redefinition of @ref dml_status_t for core functions */

/* ------ State ------ */

#if defined(__GNUC__)
/**
     * @brief Packs a structure byte by byte
     */
#define DML_HW_BYTE_PACKED_STRUCTURE_BEGIN typedef struct __attribute__((__packed__))

/**
     * @brief Pops a previous structure pack property
     */
#define DML_HW_BYTE_PACKED_STRUCTURE_END
#elif (_MSC_VER)
/**
     * @brief Packs a structure byte by byte
     */
#define DML_HW_BYTE_PACKED_STRUCTURE_BEGIN __pragma(pack(push, 1)) typedef struct

/**
    * @brief Pops a previous structure pack property
    */
#define DML_HW_BYTE_PACKED_STRUCTURE_END   __pragma(pack(pop))
#else
#error Compiler not supported
#endif

/* ################# DSA DESCRIPTOR  ################# */

#define DSA_HW_DESCRIPTOR_SIZE (64u) /**< DSA hardware descriptor byte size */

/**
 * @brief Defines a common type of the DSA hardware descriptor
 */
DML_HW_BYTE_PACKED_STRUCTURE_BEGIN
{
    uint8_t bytes[DSA_HW_DESCRIPTOR_SIZE]; /**< Allocation memory for an abstract DSA descriptor*/
}
dsahw_descriptor_t;
DML_HW_BYTE_PACKED_STRUCTURE_END

/* ################# DSA COMPLETION RECORD  ################# */

#define DSA_HW_COMPLETION_RECORD_SIZE (32u) /**< DSA hardware completion record byte size */

/**
 * @brief Defines an abstract type of the DSA hardware completion record
 */
DML_HW_BYTE_PACKED_STRUCTURE_BEGIN
{
    uint8_t status;                                    /**< DSA completion status field */
    uint8_t bytes[DSA_HW_COMPLETION_RECORD_SIZE - 1u]; /**< Allocation memory for others fields*/
}
dsahw_completion_record_t;
DML_HW_BYTE_PACKED_STRUCTURE_END

/**
 * @brief Enumerate a structure guards for internal dsa_structures
 */
typedef enum
{
    OWN_HW_STATE_GUARD     = 0x42726961,
    OWN_SW_STATE_GUARD     = 0x46726564,
    OWN_BATCH_CONFIG_GUARD = 0x4A6F686E,
} own_dml_structure_id_t;

typedef uint8_t portal_t; /**< Define portal type */

/**
 * @brief Contain information about descriptor's portals
 */
typedef struct
{
    portal_t *portals_ptr;    /**< Pointer to memory, which is mapped as DSA Portals */
    uint32_t  current_portal; /**< Current available portal to enqueue a descriptor   */
    uint32_t  portal_count;   /**< Maximal count of portals in the portal table       */
} own_hw_portal_information_t;

/**
 * @brief Contain information about TC-A and TC-B portals
 */
typedef struct
{
    own_hw_portal_information_t tc_a_portals; /**< WQs working with TC-A */
    own_hw_portal_information_t tc_b_portals; /**< WQs working with TC-B */
} own_hw_portal_table_t;

/**
 * @brief Contains information from General Capabilities Register (GENCAP)
 */
typedef struct
{
    uint32_t max_transfer_size;            /**< The maximum transfer size that can be specified in a descriptor                 */
    uint32_t max_batch_size;               /**< The maximum number of descriptors that can be referenced by a Batch descriptor  */
    uint16_t message_size;                 /**< The number of entries in the Interrupt Message Storage                          */
    uint8_t  descriptor_readback_support;  /**< Specification of Readback Address in Drain descriptors is supported/unsupported */
    uint8_t  memory_cache_control_support; /**< Cache control for write operation is supported/unsupported                      */
    uint8_t  destination_readback_support; /**< The Destination Readback flag in descriptors is supported/unsupported           */
    uint8_t  flush_cache_control_support;  /**< Cache control for cache flush operations is supported/unsupported               */
    uint8_t  overlapping_copy_support;     /**< Memory overlapping supported/unsupported for the memory move operation          */
    uint8_t  block_on_fault_support;       /**< Block of fault supported/unsupported                                            */
    uint8_t  configuration_support;        /**< Group configuration and WQ configuration are read-write/read-only               */
} own_hw_gen_cap_t;

/**
 * @brief Contains specific information about Hardware Path
 */
typedef struct
{
    own_dml_structure_id_t guard;        /**< Structure guard                         */
    own_hw_portal_table_t  portal_table; /**< Contains information about open portals */
    own_hw_gen_cap_t       gen_cap;      /**< General Capabilities Register fields    */
#if defined(linux)
    struct accfg_ctx *dsa_context_ptr; /**< @todo */
#endif
} dsahw_context_t;

#ifdef __cplusplus
}
#endif

#endif  // DSA_HW_DSA_DEFINES_H__
