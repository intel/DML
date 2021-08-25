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
 * @date 3/18/2020
 *
 * @defgroup HW_DEFINITIONS General Hardware Definitions
 * @ingroup HW_PATH
 * @{
 * @brief Contains an general definition to use a hardware path
 */
#include "hardware_definitions.h"
#include "own_hardware_checkers.h"

#ifndef DML_OWN_HARDWARE_DEFINITIONS_H__
#define DML_OWN_HARDWARE_DEFINITIONS_H__

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


/* ################# DML Macros ################# */

#define OWN_FUN(name) dsahw_##name      /**< Internal function declaration */

#define OWN_FUN_CALL(name) dsahw_##name /**< Internal function call */

/* ################# DML Macros ################# */

/**
 * @name Driver Statuses List
 * @anchor DRIVER_STATUSES
 * @{
 */
#define DSA_HARDWARE_STATUS_BASE                        10000u                          /**< @todo */
#define DML_HW_STATUS_OK                                DML_STATUS_OK                   /**< @todo */
#define DML_HW_STATUS_BUFFER_OVERFLOW                   (DSA_HARDWARE_STATUS_BASE + 1u) /**< @todo */
#define DML_HW_STATUS_DEVICE_NOT_AVAILABLE              (DSA_HARDWARE_STATUS_BASE + 2u) /**< @todo */
#define DML_HW_STATUS_CHAR_DEVICE_NOT_AVAILABLE         (DSA_HARDWARE_STATUS_BASE + 3u) /**< @todo */
#define DML_HW_STATUS_DEVICE_PATH_OVERFLOW              (DSA_HARDWARE_STATUS_BASE + 4u) /**< @todo */
#define DML_HW_STATUS_DEVICE_NOT_OPENED                 (DSA_HARDWARE_STATUS_BASE + 5u) /**< @todo */
#define DML_HW_STATUS_PORTAL_MAP_FAILED                 (DSA_HARDWARE_STATUS_BASE + 6u) /**< @todo */
#define DML_HW_STATUS_LIB_ACCEL_ERROR                   (DSA_HARDWARE_STATUS_BASE + 7u) /**< @todo */
#define DML_HW_STATUS_DEVICE_WORK_QUEUE_NOT_AVAILABLE   (DSA_HARDWARE_STATUS_BASE + 8u) /**< @todo */
/** @} */


/* ################# DSA HARDWARE FLAGS ################# */

/**
 * @name General Hardware Flags
 * @anchor GeneralHardwareFlags
 * @brief This enumeration is used to indicate various options for descriptor.
 * @{
 * @todo Make clean flag usage
 */
#define HW_FLAG_FENCE                                      0x0001u  /**< @todo add description */
#define HW_FLAG_BLOCK_ON_FAULT                             0x0002u  /**< @todo add description */
#define HW_FLAG_COMPLETION_RECORD_ADDRESS_VALID            0x0004u  /**< @todo add description */
#define HW_FLAG_REQUEST_COMPLETION_RECORD                  0x0008u  /**< @todo add description */
#define HW_FLAG_REQUEST_COMPLETION_INTERRUPT               0x0010u  /**< @todo add description */
#define HW_FLAG_COMPLETION_RECORD_STEERING_TAG_SELECTOR    0x0020u  /**< @todo add description */
#define HW_FLAG_RESERVED                                   0x0040u  /**< @todo add description */
#define HW_FLAG_CHECK_RESULT                               0x0080u  /**< @todo add description */
#define HW_FLAG_CACHE_CONTROL                              0x0100u  /**< @todo add description */
#define HW_FLAG_ADDRESS_1_TC_SELECTOR                      0x0200u  /**< @todo add description */
#define HW_FLAG_ADDRESS_2_TC_SELECTOR                      0x0400u  /**< @todo add description */
#define HW_FLAG_ADDRESS_3_TC_SELECTOR                      0x0800u  /**< @todo add description */
#define HW_FLAG_COMPLETION_RECORD_TC_SELECTOR              0x1000u  /**< @todo add description */
#define HW_FLAG_STRICT_ORDERING                            0x2000u  /**< @todo add description */
#define HW_FLAG_DESTINATION_READBACK                       0x4000u  /**< @todo add description */
#define HW_FLAG_DESTINATION_STEERING_TAG_SELECTOR          0x8000u  /**< @todo add description */
/** @} */


/* ################# DSA HARDWARE STATE ################# */

/**< Typical page size for most operating systems */
#define OWN_OS_PAGE_SIZE (0x1000u)

#define OWN_MAXIMAL_PORTALS_COUNT (OWN_OS_PAGE_SIZE / HW_DESCRIPTOR_SIZE) /**< @todo */

/** Extracts operation's specific flags */
#define OWN_HW_GET_OPERATION_FLAGS(flags) ((uint8_t)(flags >> 16u))

typedef struct accfg_ctx    own_accfg_ctx;    /**< HW Context typedef */
typedef struct accfg_device own_accfg_device; /**< HW Device typedef */
typedef struct accfg_group  own_accfg_group;  /**< HW Group typedef */
typedef struct accfg_wq     own_accfg_wq;     /**< HW Work queue typedef */
typedef struct accfg_engine own_accfg_engine; /**< HW Engine typedef */

/**
 * @brief Contain information about the operation and it's properties
 */
DML_HW_BYTE_PACKED_STRUCTURE_BEGIN
{
    uint16_t general_flags;             /**< Contains a common flags for different operations */
    uint8_t  operation_specific_flags;  /**< Contains a specific flags for operation           */
    uint8_t  operation_type;            /**< Contains an operation type @ref dml_operation_t    */
} hw_operation_control_t;
DML_HW_BYTE_PACKED_STRUCTURE_END

#ifdef __cplusplus
}
#endif

#endif //DML_OWN_HARDWARE_DEFINITIONS_H__

/** @} */
