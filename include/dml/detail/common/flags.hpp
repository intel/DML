/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_COMMON_FLAGS_HPP
#define DML_COMMON_FLAGS_HPP

#include <dml/detail/common/types.hpp>
#include <dml/detail/common/utils/enum.hpp>
#include <type_traits>

namespace dml::detail
{
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

    enum class nop_flag : std::underlying_type_t<flag>
    {
        fence = to_underlying(flag::fence)
    };

    enum class batch_flag : std::underlying_type_t<flag>
    {
        address_1_tc_selector = to_underlying(flag::address_1_tc_selector)
    };

    enum class drain_flag : std::underlying_type_t<flag>
    {
        address_1_tc_selector = to_underlying(flag::address_1_tc_selector),
        address_2_tc_selector = to_underlying(flag::address_2_tc_selector)
    };

    enum class mem_move_flag : std::underlying_type_t<flag>
    {
        fence                             = to_underlying(flag::fence),
        block_on_fault                    = to_underlying(flag::block_on_fault),
        cache_control                     = to_underlying(flag::cache_control),
        address_1_tc_selector             = to_underlying(flag::address_1_tc_selector),
        address_2_tc_selector             = to_underlying(flag::address_2_tc_selector),
        strict_ordering                   = to_underlying(flag::strict_ordering),
        destination_readback              = to_underlying(flag::destination_readback),
        destination_steering_tag_selector = to_underlying(flag::destination_steering_tag_selector)
    };

    enum class fill_flag : std::underlying_type_t<flag>
    {
        fence                             = to_underlying(flag::fence),
        block_on_fault                    = to_underlying(flag::block_on_fault),
        cache_control                     = to_underlying(flag::cache_control),
        address_2_tc_selector             = to_underlying(flag::address_2_tc_selector),
        strict_ordering                   = to_underlying(flag::strict_ordering),
        destination_readback              = to_underlying(flag::destination_readback),
        destination_steering_tag_selector = to_underlying(flag::destination_steering_tag_selector)
    };

    enum class compare_flag : std::underlying_type_t<flag>
    {
        fence                 = to_underlying(flag::fence),
        block_on_fault        = to_underlying(flag::block_on_fault),
        check_result          = to_underlying(flag::check_result),
        address_1_tc_selector = to_underlying(flag::address_1_tc_selector),
        address_2_tc_selector = to_underlying(flag::address_2_tc_selector)
    };

    enum class compare_pattern_flag : std::underlying_type_t<flag>
    {
        fence                 = to_underlying(flag::fence),
        block_on_fault        = to_underlying(flag::block_on_fault),
        check_result          = to_underlying(flag::check_result),
        address_1_tc_selector = to_underlying(flag::address_1_tc_selector)
    };

    enum class create_delta_flag : std::underlying_type_t<flag>
    {
        fence                             = to_underlying(flag::fence),
        block_on_fault                    = to_underlying(flag::block_on_fault),
        check_result                      = to_underlying(flag::check_result),
        cache_control                     = to_underlying(flag::cache_control),
        address_1_tc_selector             = to_underlying(flag::address_1_tc_selector),
        address_2_tc_selector             = to_underlying(flag::address_2_tc_selector),
        address_3_tc_selector             = to_underlying(flag::address_3_tc_selector),
        strict_ordering                   = to_underlying(flag::strict_ordering),
        destination_readback              = to_underlying(flag::destination_readback),
        destination_steering_tag_selector = to_underlying(flag::destination_steering_tag_selector)
    };

    enum class apply_delta_flag : std::underlying_type_t<flag>
    {
        fence                             = to_underlying(flag::fence),
        block_on_fault                    = to_underlying(flag::block_on_fault),
        cache_control                     = to_underlying(flag::cache_control),
        address_1_tc_selector             = to_underlying(flag::address_1_tc_selector),
        address_2_tc_selector             = to_underlying(flag::address_2_tc_selector),
        strict_ordering                   = to_underlying(flag::strict_ordering),
        destination_readback              = to_underlying(flag::destination_readback),
        destination_steering_tag_selector = to_underlying(flag::destination_steering_tag_selector)
    };

    enum class dualcast_flag : std::underlying_type_t<flag>
    {
        fence                             = to_underlying(flag::fence),
        block_on_fault                    = to_underlying(flag::block_on_fault),
        cache_control                     = to_underlying(flag::cache_control),
        address_1_tc_selector             = to_underlying(flag::address_1_tc_selector),
        address_2_tc_selector             = to_underlying(flag::address_2_tc_selector),
        address_3_tc_selector             = to_underlying(flag::address_3_tc_selector),
        strict_ordering                   = to_underlying(flag::strict_ordering),
        destination_readback              = to_underlying(flag::destination_readback),
        destination_steering_tag_selector = to_underlying(flag::destination_steering_tag_selector)
    };

    enum class crc_flag : std::underlying_type_t<flag>
    {
        fence                 = to_underlying(flag::fence),
        block_on_fault        = to_underlying(flag::block_on_fault),
        address_1_tc_selector = to_underlying(flag::address_1_tc_selector),
        address_3_tc_selector = to_underlying(flag::address_3_tc_selector)
    };

    enum class copy_crc_flag : std::underlying_type_t<flag>
    {
        fence                             = to_underlying(flag::fence),
        block_on_fault                    = to_underlying(flag::block_on_fault),
        cache_control                     = to_underlying(flag::cache_control),
        address_1_tc_selector             = to_underlying(flag::address_1_tc_selector),
        address_2_tc_selector             = to_underlying(flag::address_2_tc_selector),
        address_3_tc_selector             = to_underlying(flag::address_3_tc_selector),
        strict_ordering                   = to_underlying(flag::strict_ordering),
        destination_readback              = to_underlying(flag::destination_readback),
        destination_steering_tag_selector = to_underlying(flag::destination_steering_tag_selector)
    };

    enum class dif_check_flag : std::underlying_type_t<flag>
    {
        fence                 = to_underlying(flag::fence),
        block_on_fault        = to_underlying(flag::block_on_fault),
        address_1_tc_selector = to_underlying(flag::address_1_tc_selector)
    };

    enum class dif_insert_flag : std::underlying_type_t<flag>
    {
        fence                             = to_underlying(flag::fence),
        block_on_fault                    = to_underlying(flag::block_on_fault),
        cache_control                     = to_underlying(flag::cache_control),
        address_1_tc_selector             = to_underlying(flag::address_1_tc_selector),
        address_2_tc_selector             = to_underlying(flag::address_2_tc_selector),
        strict_ordering                   = to_underlying(flag::strict_ordering),
        destination_readback              = to_underlying(flag::destination_readback),
        destination_steering_tag_selector = to_underlying(flag::destination_steering_tag_selector)
    };

    enum class dif_strip_flag : std::underlying_type_t<flag>
    {
        fence                             = to_underlying(flag::fence),
        block_on_fault                    = to_underlying(flag::block_on_fault),
        cache_control                     = to_underlying(flag::cache_control),
        address_1_tc_selector             = to_underlying(flag::address_1_tc_selector),
        address_2_tc_selector             = to_underlying(flag::address_2_tc_selector),
        strict_ordering                   = to_underlying(flag::strict_ordering),
        destination_readback              = to_underlying(flag::destination_readback),
        destination_steering_tag_selector = to_underlying(flag::destination_steering_tag_selector)
    };

    enum class dif_update_flag : std::underlying_type_t<flag>
    {
        fence                             = to_underlying(flag::fence),
        block_on_fault                    = to_underlying(flag::block_on_fault),
        cache_control                     = to_underlying(flag::cache_control),
        address_1_tc_selector             = to_underlying(flag::address_1_tc_selector),
        address_2_tc_selector             = to_underlying(flag::address_2_tc_selector),
        strict_ordering                   = to_underlying(flag::strict_ordering),
        destination_readback              = to_underlying(flag::destination_readback),
        destination_steering_tag_selector = to_underlying(flag::destination_steering_tag_selector)
    };

    enum class cache_flush_flag : std::underlying_type_t<flag>
    {
        fence                             = to_underlying(flag::fence),
        block_on_fault                    = to_underlying(flag::block_on_fault),
        cache_control                     = to_underlying(flag::cache_control),
        address_2_tc_selector             = to_underlying(flag::address_2_tc_selector),
        strict_ordering                   = to_underlying(flag::strict_ordering),
        destination_readback              = to_underlying(flag::destination_readback),
        destination_steering_tag_selector = to_underlying(flag::destination_steering_tag_selector)
    };
}  // namespace dml::detail

#endif  //DML_COMMON_FLAGS_HPP
