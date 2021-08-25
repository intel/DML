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
 * @brief Contains an implementation of the @ref DML_OP_DRAIN descriptor initialization
 * @date 6/4/2020
 *
 */
#include <assert.h>
#include "own_hardware_status.h"
#include "own_hardware_definitions.h"
#include "hardware_descriptors_api.h"


/**
 * @brief Represents descriptor for the @ref DML_OP_BATCH operation
 */
DML_HW_BYTE_PACKED_STRUCTURE_BEGIN
{
    uint32_t                  privilege_control;           /**< 19:0 PASID - process address space ID; 30:20 - reserved; 31 - User/Supervisor */
    hw_operation_control_t    operation_control;           /**< Operation and it's properties */
    dsahw_completion_record_t *completion_record_ptr;      /**< Pointer to the completion record space */
    uint64_t                  *readback_address1_ptr;      /**< The first pointer where HW will perform implicit zero-length read */
    uint64_t                  *readback_address2_ptr;      /**< The second pointer where HW will perform implicit zero-length read */
    const uint8_t             reserved_1[4];               /**< Not used bytes in the descriptor */
    uint16_t                  completion_interrupt_handle; /**< Interruption number in the interrupt table */
    const uint8_t             reserved_2[26];              /**< Not used bytes in the descriptor */
} own_hw_drain_descriptor_t;
DML_HW_BYTE_PACKED_STRUCTURE_END


/*
 * Check that descriptor has a correct size
 */
static_assert(sizeof(own_hw_drain_descriptor_t) == DSA_HW_DESCRIPTOR_SIZE,
              "Descriptor size is not correct");


dsahw_status_t DML_HW_API(init_drain_descriptor)(dsahw_descriptor_t *descriptor_ptr,
                                                 uint64_t *readback_address1_ptr,
                                                 uint64_t *readback_address2_ptr,
                                                 dml_operation_flags_t flags,
                                                 dsahw_completion_record_t *result_ptr)
{
    // Clean descriptor
    for (uint32_t i = 0u; i < DSA_HW_DESCRIPTOR_SIZE; ++i)
    {
        uint8_t *ptr = (uint8_t *) descriptor_ptr;
        ptr[i] = 0u;
    }


    // Redefine a descriptor to fill
    own_hw_drain_descriptor_t *drain_descriptor_ptr = (own_hw_drain_descriptor_t *) descriptor_ptr;

    drain_descriptor_ptr->operation_control.operation_type = DML_OP_DRAIN;
    drain_descriptor_ptr->operation_control.general_flags  = (uint16_t)flags
                                                             | HW_FLAG_COMPLETION_RECORD_ADDRESS_VALID
                                                             | HW_FLAG_REQUEST_COMPLETION_RECORD; // TODO workaround due software model bug
    drain_descriptor_ptr->completion_record_ptr = result_ptr;
    drain_descriptor_ptr->completion_interrupt_handle = 0u;

    return DML_STATUS_OK;
}
