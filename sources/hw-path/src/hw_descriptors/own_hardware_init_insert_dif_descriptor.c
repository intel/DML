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
 * @brief Contains an implementation of the @ref DML_OP_DIF_INSERT descriptor initialization
 * @date 5/26/2020
 *
 */
#include <assert.h>
#include "own_hardware_status.h"
#include "own_hardware_definitions.h"
#include "hardware_descriptors_api.h"


#define OWN_DIF_SOURCE_FLAGS      0u /**< The index of the source DIF flags */
#define OWN_DIF_DESTINATION_FLAGS 1u /**< The index of the destination DIF flags */
#define OWN_DIF_GENERAL_FLAGS     2u /**< The index of the general DIF flags */
#define OWN_DIF_RESERVED_FLAGS    3u /**< The index of the reserved byte */

#define OWN_DIF_RESERVED_DESTINATION_FLAGS 0x07u /**> Reserved bits in the destination DIF flags */
#define OWN_DIF_RESERVED_GENERAL_FLAGS     0xF0u /**> Reserved bits in the general DIF flags */


/**
 * @brief Helps to represent 32-bit flags field as a 4-byte array
 */
typedef union
{
    uint32_t uint_32;   /**< Contains 32-bit value */
    uint8_t  uint_8[4]; /**< Contains 4-byte array */
} dif_flags_t;


/**
 * @brief Represents descriptor for the @ref DML_OP_DIF_INSERT operation
 */
DML_HW_BYTE_PACKED_STRUCTURE_BEGIN
{
    uint32_t                  privilege_control;           /**< 19:0 PASID - process address space ID; 30:20 - reserved; 31 - User/Supervisor */
    hw_operation_control_t    operation_control;           /**< Operation and it's properties */
    dsahw_completion_record_t *completion_record_ptr;      /**< Pointer to the completion record space */
    uint8_t                   *source_ptr;                 /**< Pointer to the unprotected data */
    uint8_t                   *destination_ptr;            /**< Pointer to the protected data */
    uint32_t                  transfer_size;               /**< Unprotected data size */
    uint16_t                  completion_interrupt_handle; /**< Interruption number in the interrupt table */
    uint8_t                   reserved_1[2];               /**< Not used bytes in the descriptor */
    dif_flags_t               dif_flags;                   /**< Configuration of the DIF operation */
    uint8_t                   reserved_2[12];              /**< Not used bytes in the descriptor */
    uint32_t                  reference_tag_seed;          /**< Seed for reference tag */
    uint16_t                  application_tag_mask;        /**< Mask for application tag */
    uint16_t                  application_tag_seed;        /**< Seed for application tag */
} own_insert_dif_descriptor_t;
DML_HW_BYTE_PACKED_STRUCTURE_END

/*
 * Check that descriptor has a correct size
 */
static_assert(sizeof(own_insert_dif_descriptor_t) == DSA_HW_DESCRIPTOR_SIZE,
              "Descriptor size is not correct");


dsahw_status_t DML_HW_API(init_insert_dif_descriptor)(dsahw_descriptor_t *descriptor_ptr,
                                                      uint8_t *source_ptr,
                                                      uint32_t source_length,
                                                      const dml_dif_config_t *dif_config_ptr,
                                                      uint8_t *destination_ptr,
                                                      dml_operation_flags_t flags,
                                                      dsahw_completion_record_t *result_ptr)
{
    DML_BAD_ARGUMENT_NULL_POINTER(source_ptr)
    DML_BAD_ARGUMENT_NULL_POINTER(destination_ptr)
    OWN_CHECK_UNPROTECTED_STREAM_SIZE(source_length,
                                      dif_config_ptr->block_size)

    // Clean descriptor
    for (uint32_t i = 0u; i < DSA_HW_DESCRIPTOR_SIZE; ++i)
    {
        uint8_t *ptr = (uint8_t *) descriptor_ptr;
        ptr[i] = 0u;
    }

    // Clean completion record
    for (uint32_t i = 0u; i < DSA_HW_COMPLETION_RECORD_SIZE; ++i)
    {
        uint8_t *ptr = (uint8_t *) result_ptr;
        ptr[i] = 0u;
    }


    // Redefine a descriptor to fill
    own_insert_dif_descriptor_t *dif_descriptor_ptr = (own_insert_dif_descriptor_t *) descriptor_ptr;

    // TODO Set privilege settings

    // Set operation_control
    dif_descriptor_ptr->operation_control.operation_type = DML_OP_DIF_INSERT;
    dif_descriptor_ptr->operation_control.general_flags  = (uint16_t)flags
                                                           | HW_FLAG_COMPLETION_RECORD_ADDRESS_VALID
                                                           | HW_FLAG_REQUEST_COMPLETION_RECORD
                                                           | HW_FLAG_BLOCK_ON_FAULT; // TODO workaround due software model bug

    dif_descriptor_ptr->operation_control.operation_specific_flags = 0u;

    // Set completion record address
    dif_descriptor_ptr->completion_record_ptr = result_ptr;

    // Set buffers settings
    dif_descriptor_ptr->source_ptr      = source_ptr;
    dif_descriptor_ptr->destination_ptr = destination_ptr;
    dif_descriptor_ptr->transfer_size   = source_length;

    // Set dif properties
    dif_descriptor_ptr->dif_flags.uint_32 = dif_config_ptr->flags;

    // Reset reserved bits in the DIF flag fields
    dif_descriptor_ptr->dif_flags.uint_8[OWN_DIF_SOURCE_FLAGS]       = 0u;
    dif_descriptor_ptr->dif_flags.uint_8[OWN_DIF_RESERVED_FLAGS]     = 0u;
    dif_descriptor_ptr->dif_flags.uint_8[OWN_DIF_DESTINATION_FLAGS] &= ~OWN_DIF_RESERVED_DESTINATION_FLAGS;
    dif_descriptor_ptr->dif_flags.uint_8[OWN_DIF_GENERAL_FLAGS]     &= ~OWN_DIF_RESERVED_GENERAL_FLAGS;

    // Set DIF-block size
    dif_descriptor_ptr->dif_flags.uint_8[OWN_DIF_GENERAL_FLAGS] |= dif_config_ptr->block_size;

    // Set tag seeds and application tag mask
    dif_descriptor_ptr->reference_tag_seed    = dif_config_ptr->destination_reference_tag_seed;
    dif_descriptor_ptr->application_tag_seed  = dif_config_ptr->destination_application_tag_seed;
    dif_descriptor_ptr->application_tag_mask  = dif_config_ptr->destination_application_tag_mask;

    // Set handle
    dif_descriptor_ptr->completion_interrupt_handle = 0u;

    return DML_STATUS_OK;
}
