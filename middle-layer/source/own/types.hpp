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

#ifndef DML_ML_SOURCE_OWN_TYPES_HPP
#define DML_ML_SOURCE_OWN_TYPES_HPP

#include <cstdint>
#include <type_traits>

namespace dml::ml
{
    /**
     * @brief Statuses reported by hardware
     */
    enum class hw_status : uint8_t
    {
        success                       = 0x01u, /**< Success. */
        false_predicate_success       = 0x02u, /**< Success with false predicate. */
        page_fault_during_processing  = 0x03u, /**< Partial completion due to page fault.  */
        page_response_error           = 0x04u, /**< Partial completion due to an Invalid Request response to a Page Request. */
        batch_processing_error        = 0x05u, /**< One or more operations in the batch completed with Status not equal to Success. */
        batch_page_fault_error        = 0x06u, /**< Partial completion due to page fault.  */
        offset_order_error            = 0x07u, /**< Offsets in the delta record were not in increasing order. */
        offset_overflow               = 0x08u, /**< An offset in the delta record was greater than or equal to the Transfer Size of the descriptor. */
        dif_control_error             = 0x09u, /**< DIF error. This value is used for the DIF Check, DIF Strip, and DIF Update operations. */
        operation_error               = 0x10u, /**< Unsupported operation code. */
        flag_error                    = 0x11u, /**< Invalid flags. Any flag contains an unsupported or reserved value. */
        non_zero_reserved_field_error = 0x12u, /**< Non-zero reserved field (other than a flag). */
        invalid_transfer_size_error   = 0x13u, /**< Invalid Transfer Size. */
        operation_count_error         = 0x14u, /**< Descriptor Count out of range. */
        delta_size_error              = 0x15u, /**< Maximum Delta Record Size or Delta Record Size out of range. */
        buffers_overlap               = 0x16u, /**< Overlapping buffers. */
        dualcast_misalign_error       = 0x17u, /**< Bits 11:0 of the two destination buffers differ in Memory Copy with Dualcast. */
        operation_list_align_error    = 0x18u, /**< Misaligned Descriptor List Address. */
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
        internal_error                = 0xFFu, /**< Unexpected error occurred (SW only) */
    };

    /**
     * @brief Operation supported by hardware
     */
    enum class hw_operation : uint8_t
    {
        nop             = 0x00u, /**< No operation                          */
        batch           = 0x01u, /**< Batch operation                       */
        drain           = 0x02u, /**< Drain operation                       */
        mem_move        = 0x03u, /**< Memory move operation                 */
        fill            = 0x04u, /**< Memory fill operation                 */
        compare         = 0x05u, /**< Memory compare operation              */
        compare_pattern = 0x06u, /**< Memory compare with pattern operation */
        create_delta    = 0x07u, /**< Create delta record operation         */
        apply_delta     = 0x08u, /**< Apply delta record operation          */
        dualcast        = 0x09u, /**< Memory copy with dualcast operation   */
        crc             = 0x10u, /**< Generate CRC operation                */
        copy_crc        = 0x11u, /**< Copy with CRC operation               */
        dif_check       = 0x12u, /**< Dif check operation                   */
        dif_insert      = 0x13u, /**< Dif insert operation                  */
        dif_strip       = 0x14u, /**< Dif strip operation                   */
        dif_update      = 0x15u, /**< Dif update operation                  */
        cache_flush     = 0x20u  /**< Cache flush operation                 */
    };

    /**
     * @brief Options supported by hardware
     */
    enum class hw_option : uint16_t
    {
        none                                    = 0x0000u, /**< @todo */
        fence                                   = 0x0001u, /**< @todo */
        block_on_fault                          = 0x0002u, /**< @todo */
        completion_record_address_valid         = 0x0004u, /**< @todo */
        request_completion_record               = 0x0008u, /**< @todo */
        request_completion_interrupt            = 0x0010u, /**< @todo */
        completion_record_steering_tag_selector = 0x0020u, /**< @todo */
        reserved                                = 0x0040u, /**< @todo */
        check_result                            = 0x0080u, /**< @todo */
        cache_control                           = 0x0100u, /**< @todo */
        address_1_tc_selector                   = 0x0200u, /**< @todo */
        address_2_tc_selector                   = 0x0400u, /**< @todo */
        address_3_tc_selector                   = 0x0800u, /**< @todo */
        completion_record_tc_selector           = 0x1000u, /**< @todo */
        strict_ordering                         = 0x2000u, /**< @todo */
        destination_readback                    = 0x4000u, /**< @todo */
        destination_steering_tag_selector       = 0x8000u  /**< @todo */
    };

    /**
     * @brief Options supported by hardware for CRC operation
     */
    enum class crc_option : uint8_t
    {
        read_seed              = 0b001,
        bypass_reflection      = 0b010,
        bypass_data_reflection = 0b100
    };

    /**
     * @brief Helper for casting enumeration to its underlying type
     *
     * Only C++23 has this feature in the standard
     *
     * @tparam T     Enumeration type
     * @param value  Enumeration value
     *
     * @return Enumeration value in its underlying type
     */
    template <typename T>
    constexpr auto to_underlying(T value) noexcept
    {
        return static_cast<std::underlying_type_t<T>>(value);
    }

    /**
     * @brief Unite two @ref hw_option sets
     *
     * @param lhs Left-hand side operand
     * @param rhs Right-hand side operand
     *
     * @return Union of two option sets
     */
    constexpr auto operator|(hw_option lhs, hw_option rhs) noexcept
    {
        return static_cast<hw_option>(to_underlying(lhs) | to_underlying(rhs));
    }

    /**
     * @brief Intersects two @ref hw_option sets
     *
     * @param lhs Left-hand side operand
     * @param rhs Right-hand side operand
     *
     * @return Intersection of two option sets
     */
    constexpr auto operator&(hw_option lhs, hw_option rhs) noexcept
    {
        return static_cast<hw_option>(to_underlying(lhs) & to_underlying(rhs));
    }

    /**
     * @brief Checks if right hand side set is a subset of left hand size set
     *
     * @param lhs Left-hand side operand
     * @param rhs Right-hand side operand
     *
     * @return Boolean result of the check
     */
    constexpr auto any(hw_option lhs, hw_option rhs) noexcept { return (lhs & rhs) == rhs; }

    /**
     * @brief Unite two @ref crc_option sets
     *
     * @param lhs Left-hand side operand
     * @param rhs Right-hand side operand
     *
     * @return Union of two option sets
     */
    constexpr auto operator|(crc_option lhs, crc_option rhs) noexcept
    {
        return static_cast<crc_option>(to_underlying(lhs) | to_underlying(rhs));
    }

    /**
     * @brief Intersects two @ref crc_option sets
     *
     * @param lhs Left-hand side operand
     * @param rhs Right-hand side operand
     *
     * @return Intersection of two option sets
     */
    constexpr auto operator&(crc_option lhs, crc_option rhs) noexcept
    {
        return static_cast<crc_option>(to_underlying(lhs) & to_underlying(rhs));
    }

    /**
     * @brief Checks if right hand side set is a subset of left hand size set
     *
     * @param lhs Left-hand side operand
     * @param rhs Right-hand side operand
     *
     * @return Boolean result of the check
     */
    constexpr auto any(crc_option lhs, crc_option rhs) noexcept { return (lhs & rhs) == rhs; }

    /**
     * @brief Common flags for all operations
     */
    constexpr auto hw_use_completion_record =
        hw_option::completion_record_address_valid | hw_option::request_completion_record;
}  // namespace dml::ml

#endif  //DML_ML_SOURCE_OWN_TYPES_HPP
