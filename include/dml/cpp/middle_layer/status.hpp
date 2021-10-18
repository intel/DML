/*
 * Copyright 2021 Intel Corporation.
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

#ifndef DML_ML_STATUS_HPP
#define DML_ML_STATUS_HPP

#include <cstdint>
#include <limits>

namespace dml::ml
{
    enum class submission_status
    {
        success,
        failure
    };

    enum class execution_status: std::uint8_t
    {
        success                       = 0x01u,  /**< Success. */
        false_predicate_success       = 0x02u,  /**< Success with false predicate. */
        page_fault_during_processing  = 0x03u,  /**< Partial completion due to page fault.  */
        page_response_error           = 0x04u,  /**< Partial completion due to an Invalid Request response to a Page Request. */
        batch_error                   = 0x05u,  /**< One or more operations in the batch completed with Status not equal to Success. */
        batch_page_fault_error        = 0x06u,  /**< Partial completion due to page fault.  */
        offset_order_error            = 0x07u,  /**< Offsets in the delta record were not in increasing order. */
        offset_overflow               = 0x08u,  /**< An offset in the delta record was greater than or equal to the Transfer Size of the descriptor. */
        dif_control_error             = 0x09u,  /**< DIF error. This value is used for the DIF Check, DIF Strip, and DIF Update operations. */
        operation_error               = 0x10u,  /**< Unsupported operation code. */
        flag_error                    = 0x11u,  /**< Invalid flags. Any flag contains an unsupported or reserved value. */
        non_zero_reserved_field_error = 0x12u,  /**< Non-zero reserved field (other than a flag). */
        invalid_transfer_size_error   = 0x13u,  /**< Invalid Transfer Size. */
        descriptor_count_error        = 0x14u,  /**< Descriptor Count out of range. */
        delta_size_error              = 0x15u,  /**< Maximum Delta Record Size or Delta Record Size out of range. */
        buffers_overlap               = 0x16u,  /**< Overlapping buffers. */
        dualcast_misalign_error       = 0x17u,  /**< Bits 11:0 of the two destination buffers differ in Memory Copy with Dualcast. */
        descriptor_list_align_error   = 0x18u,  /**< Misaligned Descriptor List Address. */
        invalid_interrupt_handle      = 0x19u,  /**< Invalid Completion Interrupt Handle. */
        page_fault_on_translation     = 0x1au,  /**< A page fault occurred while translating a Completion Record Address and either */
        completion_record_align_error = 0x1bu,  /**< Completion Record Address is not 32-byte aligned. */
        misalign_address_error        = 0x1cu,  /**< Misaligned address */
        privilege_error               = 0x1du,  /**< Priv is 1 and the Privileged Mode Enable field of the PCI Express PASID capability is 0. */
        traffic_class_error           = 0x1eu,  /**< Incorrect Traffic Class configuration */
        readback_translation_error    = 0x1fu,  /**< A page fault occurred while translating a Readback Address */
        operation_readback_timeout    = 0x20u,  /**< The operation failed due to a hardware error other than a completion timeout or unsuccessful */
        hardware_timeout              = 0x21u,  /**< Hardware error (completion timeout or unsuccessful completion status) */
        address_translation_error     = 0x22u,  /**< An error occurred during address translation */

        unexpected = std::numeric_limits<std::uint8_t>::max() /**< Unexpected error code */
    };
}  // namespace dml::ml

#endif  //DML_ML_STATUS_HPP
