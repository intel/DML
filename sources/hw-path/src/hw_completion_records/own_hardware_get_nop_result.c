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
 * @brief Contains an implementation of the @ref dsa_get_nop_result function
 * @date 3/26/2020
 *
 */
#include <assert.h>
#include "own_hardware_status.h"
#include "own_completion_record_defines.h"
#include "hardware_completion_records_api.h"


/**
 * @brief Represents a Completion Record of the @ref DML_OP_NOP operation
 */
DML_HW_BYTE_PACKED_STRUCTURE_BEGIN
{
    uint8_t  status;              /**< Operation status  */
    const uint8_t reserved_1[31]; /**< Not used bytes    */
} own_nop_completion_record_t;
DML_HW_BYTE_PACKED_STRUCTURE_END


/*
 * Check that completion record has a correct size
 */
static_assert(sizeof(own_nop_completion_record_t) == DSA_HW_COMPLETION_RECORD_SIZE,
              "Descriptor size is not correct");


dsahw_status_t DML_HW_API(get_nop_result)(const dsahw_completion_record_t *completion_record_ptr)
{
    const own_nop_completion_record_t *hw_result_ptr = (const own_nop_completion_record_t *)completion_record_ptr;

    hw_status_t hw_status = (hw_status_t)hw_result_ptr->status;

    switch (hw_status)
    {
        case HW_STATUS_SUCCESS:

            return DML_STATUS_OK;

        default:

            return DML_STATUS_INTERNAL_ERROR;
    }
}
