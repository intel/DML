/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_COMMON_SPECIFIC_FLAGS_HPP
#define DML_COMMON_SPECIFIC_FLAGS_HPP

#include <dml/detail/common/types.hpp>
#include <type_traits>

namespace dml::detail
{
    enum class compare_result : result_t
    {
        equal     = 0,
        not_equal = 1
    };

    enum class create_delta_result : result_t
    {
        equal     = 1,
        not_equal = 2,
        overflow  = 4
    };

    enum class drain_specific_flag : operation_specific_flags_t
    {
        readback_address_1_valid        = 0b0001,
        readback_address_2_valid        = 0b0010,
        suppress_tc_a_implicit_readback = 0b0100,
        suppress_tc_b_implicit_readback = 0b1000
    };

    enum class dualcast_specific_flag : operation_specific_flags_t
    {
        destination_2_steering_tag_selector = 0b1
    };

    enum class crc_specific_flag : operation_specific_flags_t
    {
        read_crc_seed                       = 0b001,
        bypass_crc_inversion_and_reflection = 0b010,
        bypass_data_reflection              = 0b100
    };

    enum class dif_specific_flag : operation_specific_flags_t
    {
        block_size_512    = 0b0000,
        block_size_520    = 0b0001,
        block_size_4096   = 0b0010,
        block_size_4104   = 0b0011,
        invert_crc_seed   = 0b0100,
        invert_crc_result = 0b1000
    };

    enum class dif_source_flag : operation_specific_flags_t
    {
        enable_all_f_detect_error = 0b00000001,
        all_f_detect              = 0b00000010,
        app_tag_f_detect          = 0b00000100,
        app_and_ref_tag_f_detect  = 0b00001000,
        incrementing_app_tag_type = 0b00010000,
        guard_check_disable       = 0b00100000,
        ref_tag_check_disable     = 0b01000000,
        fixed_ref_tag_type        = 0b10000000
    };

    enum class dif_destination_flag : operation_specific_flags_t
    {
        app_tag_pass_through      = 0b00001000,
        incrementing_app_tag_type = 0b00010000,
        guard_field_pass_through  = 0b00100000,
        ref_tag_pass_through      = 0b01000000,
        fixed_ref_tag_type        = 0b10000000
    };
}  // namespace dml::detail

#endif  //DML_COMMON_SPECIFIC_FLAGS_HPP
