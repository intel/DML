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
 * @brief Contains an implementation of the @ref DML_OP_DELTA_CREATE descriptor initialization
 * @date 6/22/2020
 *
 */
#include <assert.h>
#include "own_hardware_status.h"
#include "own_hardware_definitions.h"
#include "hardware_descriptors_api.h"


#define OWN_MAX_AVAILABLE_INPUT_SIZE 0x80000u /**< Input vector size limit*/
#define OWN_DELTA_NOTE_SIZE          0xAu     /**< Delta note size*/


/**
 * @brief Represents descriptor for @ref DML_OP_DELTA_CREATE operation
 */
DML_HW_BYTE_PACKED_STRUCTURE_BEGIN
{
    uint32_t                  privilege_control;           /**< 19:0 PASID - process address space ID; 30:20 - reserved; 31 - User/Supervisor */
    hw_operation_control_t    operation_control;           /**< Operation and it's properties */
    dsahw_completion_record_t *completion_record_ptr;      /**< Pointer to the completion record space */
    uint8_t                   *source_first_ptr;           /**< Pointer to the memory region */
    uint8_t                   *source_second_ptr;          /**< Pointer to the reference memory region */
    uint32_t                  transfer_size;               /**< Count of bytes to copy */
    uint16_t                  completion_interrupt_handle; /**< Interruption number in the interrupt table */
    const uint8_t             reserved_1[2];               /**< Not used bytes in the descriptor */
    uint8_t                   *delta_record_ptr;           /**< Pointer to the output vector */
    uint32_t                  max_delta_record_size;       /**< Maximum number of bytes that can be stored in the output vector */
    const uint8_t             reserved_2[4];               /**< Not used bytes in the descriptor */
    uint8_t                   expected_result;             /**< Expected result of the operation */
    const uint8_t             reserved_3[7];               /**< Not used bytes in the descriptor */
} own_delta_create_descriptor_t;
DML_HW_BYTE_PACKED_STRUCTURE_END


/*
 * Check that descriptor has a correct size
 */
static_assert(sizeof(own_delta_create_descriptor_t) == DSA_HW_DESCRIPTOR_SIZE,
              "Descriptor size is not correct");


dsahw_status_t DML_HW_API(init_delta_create_descriptor)(dsahw_descriptor_t *descriptor_ptr,
                                                        uint8_t *source_first_ptr,
                                                        uint8_t *source_second_ptr,
                                                        uint32_t source_length,
                                                        uint8_t *delta_record_ptr,
                                                        uint32_t delta_record_length,
                                                        dml_meta_result_t expected_result,
                                                        dml_operation_flags_t flags,
                                                        dsahw_completion_record_t *result_ptr)
{
    DML_BAD_ARGUMENT_NULL_POINTER(source_first_ptr);
    DML_BAD_ARGUMENT_NULL_POINTER(source_second_ptr);
    DML_BAD_ARGUMENT_NULL_POINTER(delta_record_ptr);
    DML_BAD_ARGUMENT_RETURN(delta_record_length % OWN_DELTA_NOTE_SIZE, DML_STATUS_DELTA_INPUT_SIZE_ERROR)
    DML_BAD_ARGUMENT_RETURN(source_length > OWN_MAX_AVAILABLE_INPUT_SIZE, DML_STATUS_DELTA_OFFSET_ERROR)

    // Redefine a descriptor to fill
    own_delta_create_descriptor_t *op_descriptor_ptr = (own_delta_create_descriptor_t *)descriptor_ptr;

    // TODO Set privilege settings

    // Clean descriptor
    for (uint32_t i = 0u; i < DSA_HW_DESCRIPTOR_SIZE; ++i)
    {
        uint8_t *ptr = (uint8_t *)op_descriptor_ptr;
        ptr[i] = 0u;
    }

    // Clean completion record
    for (uint32_t i = 0u; i < DSA_HW_COMPLETION_RECORD_SIZE; ++i)
    {
        uint8_t *ptr = (uint8_t *) result_ptr;
        ptr[i] = 0u;
    }


    // Operation fields
    op_descriptor_ptr->source_first_ptr      = source_first_ptr;
    op_descriptor_ptr->source_second_ptr     = source_second_ptr;
    op_descriptor_ptr->transfer_size         = source_length;
    op_descriptor_ptr->delta_record_ptr      = delta_record_ptr;
    op_descriptor_ptr->max_delta_record_size = delta_record_length;

    op_descriptor_ptr->operation_control.operation_type = DML_OP_DELTA_CREATE;

    // Common fields
    op_descriptor_ptr->completion_interrupt_handle     = 0u;
    op_descriptor_ptr->completion_record_ptr           = result_ptr;
    op_descriptor_ptr->operation_control.general_flags = (uint16_t)flags
                                                         | HW_FLAG_COMPLETION_RECORD_ADDRESS_VALID
                                                         | HW_FLAG_REQUEST_COMPLETION_RECORD
                                                         | HW_FLAG_BLOCK_ON_FAULT; // TODO workaround due software model bug

    if (DML_FLAG_CHECK_RESULT & flags)
    {
        op_descriptor_ptr->expected_result = expected_result;
    }


    return DML_STATUS_OK;
}
