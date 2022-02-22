/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_DETAIL_COMMON_STATUS_HPP
#define DML_DETAIL_COMMON_STATUS_HPP

#include <dml/detail/common/types.hpp>

namespace dml::detail
{
    enum class validation_status
    {
        success,
        null_address,
        null_size,
        large_size,
        overlapping,
        misalignment,
        dif_strip_adjacent,
        wrong_size,
        wrong_delta_size,
        wrong_dif_size,
        wrong_dualcast_address,
        wrong_batch_size,
        unsupported_operation,
        error
    };

    enum class submission_status
    {
        success,
        queue_busy,
        failure
    };

    enum class execution_status : status_t
    {
        processing                    = 0x00u, /** Descriptor is still being processed */
        success                       = 0x01u, /**< Success. */
        false_predicate_success       = 0x02u, /**< Success with false predicate. */
        page_fault_during_processing  = 0x03u, /**< Partial completion due to page fault.  */
        page_response_error           = 0x04u, /**< Partial completion due to an Invalid Request response to a Page Request. */
        batch_error                   = 0x05u, /**< One or more operations in the batch completed with Status not equal to Success. */
        batch_page_fault_error        = 0x06u, /**< Partial completion due to page fault.  */
        offset_order_error            = 0x07u, /**< Offsets in the delta record were not in increasing order. */
        offset_overflow               = 0x08u, /**< An offset in the delta record was greater than or equal to the Transfer Size of the descriptor. */
        dif_control_error             = 0x09u, /**< DIF error. This value is used for the DIF Check, DIF Strip, and DIF Update operations. */
        operation_error               = 0x10u, /**< Unsupported operation code. */
        flag_error                    = 0x11u, /**< Invalid flags. Any flag contains an unsupported or reserved value. */
        non_zero_reserved_field_error = 0x12u, /**< Non-zero reserved field (other than a flag). */
        invalid_transfer_size_error   = 0x13u, /**< Invalid Transfer Size. */
        descriptor_count_error        = 0x14u, /**< Descriptor Count out of range. */
        delta_size_error              = 0x15u, /**< Maximum Delta Record Size or Delta Record Size out of range. */
        buffers_overlap               = 0x16u, /**< Overlapping buffers. */
        dualcast_misalign_error       = 0x17u, /**< Bits 11:0 of the two destination buffers differ in Memory Copy with Dualcast. */
        descriptor_list_align_error   = 0x18u, /**< Misaligned Descriptor List Address. */
        invalid_interrupt_handle      = 0x19u, /**< Invalid Completion Interrupt Handle. */
        page_fault_on_translation     = 0x1au, /**< A page fault occurred while translating a Completion Record Address and either */
        completion_record_align_error = 0x1bu, /**< Completion Record Address is not 32-byte aligned. */
        misalign_address_error        = 0x1cu, /**< Misaligned address */
        privilege_error               = 0x1du, /**< Priv is 1 and the Privileged Mode Enable field of the PCI Express PASID capability is 0. */
        traffic_class_error           = 0x1eu, /**< Incorrect Traffic Class configuration */
        readback_translation_error    = 0x1fu, /**< A page fault occurred while translating a Readback Address */
        operation_readback_timeout    = 0x20u, /**< The operation failed due to a hardware error other than a completion timeout or unsuccessful */
        hardware_timeout              = 0x21u, /**< Hardware error (completion timeout or unsuccessful completion status) */
        address_translation_error     = 0x22u, /**< An error occurred during address translation */
    };
}  // namespace dml::detail

#endif  //DML_DETAIL_COMMON_STATUS_HPP
