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

#ifndef DML_ML_VALUES_HPP
#define DML_ML_VALUES_HPP

#include "types.hpp"

namespace dml::ml
{
    enum class operation : operation_t
    {
        nop             = 0x00,
        batch           = 0x01,
        drain           = 0x02,
        memory_move     = 0x03,
        fill            = 0x04,
        compare         = 0x05,
        compare_pattern = 0x06,
        create_delta    = 0x07,
        apply_delta     = 0x08,
        dualcast        = 0x09,
        crc             = 0x10,
        copy_crc        = 0x11,
        dif_check       = 0x12,
        dif_insert      = 0x13,
        dif_strip       = 0x14,
        dif_update      = 0x15,
        cache_flush     = 0x20
    };

    enum class flag : flags_t
    {
        fence                                   = 0b0000000000000001,
        block_on_fault                          = 0b0000000000000010,
        completion_record_address_valid         = 0b0000000000000100,
        request_completion_record               = 0b0000000000001000,
        request_completion_interrupt            = 0b0000000000010000,
        completion_record_steering_tag_selector = 0b0000000000100000,
        check_result                            = 0b0000000010000000,
        cache_control                           = 0b0000000100000000,
        address_1_tc_selector                   = 0b0000001000000000,
        address_2_tc_selector                   = 0b0000010000000000,
        address_3_tc_selector                   = 0b0000100000000000,
        completion_record_tc_selector           = 0b0001000000000000,
        strict_ordering                         = 0b0010000000000000,
        destination_readback                    = 0b0100000000000000,
        destination_steering_tag_selector       = 0b1000000000000000
    };

    enum class drain_flag : operation_specific_flags_t
    {
        readback_address_1_valid        = 0b0001,
        readback_address_2_valid        = 0b0010,
        suppress_tc_a_implicit_readback = 0b0100,
        suppress_tc_b_implicit_readback = 0b1000
    };

    enum class dualcast_flag : operation_specific_flags_t
    {
        destination_2_steering_tag_selector = 0b1
    };

    enum class crc_flag : operation_specific_flags_t
    {
        read_crc_seed                       = 0b001,
        bypass_crc_inversion_and_reflection = 0b010,
        bypass_data_reflection              = 0b100,
    };

    enum class dif_flag : operation_specific_flags_t
    {
        block_size_512    = 0b0000,
        block_size_520    = 0b0001,
        block_size_4096   = 0b0010,
        block_size_4104   = 0b0011,
        invert_crc_seed   = 0b0100,
        invert_crc_result = 0b1000,
    };

    enum class dif_source_flag : operation_specific_flags_t
    {
        enable_all_f_detect_error              = 0b00000001,
        all_f_detect                           = 0b00000010,
        application_tag_f_detect               = 0b00000100,
        application_and_reference_tag_f_detect = 0b00001000,
        incrementing_application_tag_type      = 0b00010000,
        guard_check_disable                    = 0b00100000,
        reference_tag_check_disable            = 0b01000000,
        fixed_reference_tag_type               = 0b10000000
    };

    enum class dif_destination_flag : operation_specific_flags_t
    {
        application_tag_pass_through      = 0b00001000,
        incrementing_application_tag_type = 0b00010000,
        guard_field_pass_through          = 0b00100000,
        reference_tag_pass_through        = 0b01000000,
        fixed_reference_tag_type          = 0b10000000
    };

    enum dif_status : dif_status_t
    {
        guard_mismatch            = 0x01,
        applicaation_tag_mismatch = 0x02,
        reference_tag_mismatch    = 0x04,
        all_f_detect_error        = 0x08
    };
}  // namespace dml::ml

#endif  //DML_ML_VALUES_HPP
