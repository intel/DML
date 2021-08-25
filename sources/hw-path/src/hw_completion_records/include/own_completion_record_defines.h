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
 * @brief Contains an common defines for completion records
 * @date 3/27/2020
 *
 */

#include "own_hardware_definitions.h"

#ifndef DML_OWN_COMPLETION_RECORD_DEFINES_H__
#define DML_OWN_COMPLETION_RECORD_DEFINES_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Contain main completion record fields
 */
DML_HW_BYTE_PACKED_STRUCTURE_BEGIN
{
    uint8_t  operation_status;     /**< Status of the executed task: Success or some Error */
    uint8_t  operation_result;     /**< Result of completed operation                      */
    uint8_t  reserved[2];          /**< Reserved bytes                                     */
    uint32_t elements_processed;   /**< Count of processed elements (bytes, words and etc.)*/
} own_general_result_field_t;
DML_HW_BYTE_PACKED_STRUCTURE_END

#ifdef __cplusplus
}
#endif

#endif //DML_OWN_COMPLETION_RECORD_DEFINES_H__
