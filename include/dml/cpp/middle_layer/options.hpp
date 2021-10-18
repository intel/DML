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

#ifndef DML_COMMON_OPTIONS_HPP
#define DML_COMMON_OPTIONS_HPP

#include "option_types.hpp"
#include "values.hpp"

namespace dml::ml
{
    /**
     * @todo
     */
    struct mem_move_option
    {
        using type = mem_move_options;

        static constexpr type none                              = type();
        static constexpr type fence                             = flag::fence;
        static constexpr type block_on_fault                    = flag::block_on_fault;
        static constexpr type cache_control                     = flag::cache_control;
        static constexpr type address_1_tc_selector             = flag::address_1_tc_selector;
        static constexpr type address_2_tc_selector             = flag::address_2_tc_selector;
        static constexpr type strict_ordering                   = flag::strict_ordering;
        static constexpr type destination_readback              = flag::destination_readback;
        static constexpr type destination_steering_tag_selector = flag::destination_steering_tag_selector;
    };

    /**
     * @todo
     */
    struct fill_option
    {
        using type = fill_options;

        static constexpr type none                              = type();
        static constexpr type fence                             = flag::fence;
        static constexpr type block_on_fault                    = flag::block_on_fault;
        static constexpr type cache_control                     = flag::cache_control;
        static constexpr type address_2_tc_selector             = flag::address_2_tc_selector;
        static constexpr type strict_ordering                   = flag::strict_ordering;
        static constexpr type destination_readback              = flag::destination_readback;
        static constexpr type destination_steering_tag_selector = flag::destination_steering_tag_selector;
    };

    /**
     * @todo
     */
    struct dualcast_option
    {
        using type = dualcast_options;

        static constexpr type none                              = type();
        static constexpr type fence                             = flag::fence;
        static constexpr type block_on_fault                    = flag::block_on_fault;
        static constexpr type cache_control                     = flag::cache_control;
        static constexpr type address_1_tc_selector             = flag::address_1_tc_selector;
        static constexpr type address_2_tc_selector             = flag::address_2_tc_selector;
        static constexpr type address_3_tc_selector             = flag::address_3_tc_selector;
        static constexpr type strict_ordering                   = flag::strict_ordering;
        static constexpr type destination_readback              = flag::destination_readback;
        static constexpr type destination_steering_tag_selector = flag::destination_steering_tag_selector;
    };

    /**
     * @todo
     */
    struct compare_option
    {
        using type = compare_options;

        static constexpr type none                  = type();
        static constexpr type fence                 = flag::fence;
        static constexpr type block_on_fault        = flag::block_on_fault;
        static constexpr type check_result          = flag::check_result;
        static constexpr type address_1_tc_selector = flag::address_1_tc_selector;
        static constexpr type address_2_tc_selector = flag::address_2_tc_selector;
    };

    /**
     * @todo
     */
    struct compare_pattern_option
    {
        using type = compare_pattern_options;

        static constexpr type none                  = type();
        static constexpr type fence                 = flag::fence;
        static constexpr type block_on_fault        = flag::block_on_fault;
        static constexpr type check_result          = flag::check_result;
        static constexpr type address_1_tc_selector = flag::address_1_tc_selector;
    };

    /**
     * @todo
     */
    struct crc_option
    {
        using type = crc_options;

        static constexpr type none                  = type();
        static constexpr type fence                 = flag::fence;
        static constexpr type block_on_fault        = flag::block_on_fault;
        static constexpr type address_1_tc_selector = flag::address_1_tc_selector;
        static constexpr type address_3_tc_selector = flag::address_3_tc_selector;
    };

    /**
     * @todo
     */
    struct copy_crc_option
    {
        using type = copy_crc_options;

        static constexpr type none                              = type();
        static constexpr type fence                             = flag::fence;
        static constexpr type block_on_fault                    = flag::block_on_fault;
        static constexpr type cache_control                     = flag::cache_control;
        static constexpr type address_1_tc_selector             = flag::address_1_tc_selector;
        static constexpr type address_2_tc_selector             = flag::address_2_tc_selector;
        static constexpr type address_3_tc_selector             = flag::address_3_tc_selector;
        static constexpr type strict_ordering                   = flag::strict_ordering;
        static constexpr type destination_readback              = flag::destination_readback;
        static constexpr type destination_steering_tag_selector = flag::destination_steering_tag_selector;
    };

    /**
     * @todo
     */
    struct create_delta_option
    {
        using type = create_delta_options;

        static constexpr type none                              = type();
        static constexpr type fence                             = flag::fence;
        static constexpr type block_on_fault                    = flag::block_on_fault;
        static constexpr type check_result                      = flag::check_result;
        static constexpr type cache_control                     = flag::cache_control;
        static constexpr type address_1_tc_selector             = flag::address_1_tc_selector;
        static constexpr type address_2_tc_selector             = flag::address_2_tc_selector;
        static constexpr type address_3_tc_selector             = flag::address_3_tc_selector;
        static constexpr type strict_ordering                   = flag::strict_ordering;
        static constexpr type destination_readback              = flag::destination_readback;
        static constexpr type destination_steering_tag_selector = flag::destination_steering_tag_selector;
    };

    /**
     * @todo
     */
    struct apply_delta_option
    {
        using type = apply_delta_options;

        static constexpr type none                              = type();
        static constexpr type fence                             = flag::fence;
        static constexpr type block_on_fault                    = flag::block_on_fault;
        static constexpr type cache_control                     = flag::cache_control;
        static constexpr type address_1_tc_selector             = flag::address_1_tc_selector;
        static constexpr type address_2_tc_selector             = flag::address_2_tc_selector;
        static constexpr type strict_ordering                   = flag::strict_ordering;
        static constexpr type destination_readback              = flag::destination_readback;
        static constexpr type destination_steering_tag_selector = flag::destination_steering_tag_selector;
    };

    /**
     * @todo
     */
    struct cache_flush_option
    {
        using type = cache_flush_options;

        static constexpr type none                              = type();
        static constexpr type fence                             = flag::fence;
        static constexpr type block_on_fault                    = flag::block_on_fault;
        static constexpr type cache_control                     = flag::cache_control;
        static constexpr type address_2_tc_selector             = flag::address_2_tc_selector;
        static constexpr type strict_ordering                   = flag::strict_ordering;
        static constexpr type destination_readback              = flag::destination_readback;
        static constexpr type destination_steering_tag_selector = flag::destination_steering_tag_selector;
    };

    /**
     * @todo
     */
    struct batch_option
    {
        using type = batch_options;

        static constexpr type none                  = type();
        static constexpr type address_1_tc_selector = flag::address_1_tc_selector;
    };

    /**
     * @todo
     */
    struct expected_result_option
    {
        static constexpr auto expect_equal     = expected_result_options(0);
        static constexpr auto expect_not_equal = expected_result_options(1);
    };

    /**
     * @todo
     */
    struct crc_additional_option
    {
        using type = crc_additional_options;

        static constexpr type none                   = type();
        static constexpr type read_crc_seed          = crc_flag::read_crc_seed;
        static constexpr type bypass_reflection      = crc_flag::bypass_crc_inversion_and_reflection;
        static constexpr type bypass_data_reflection = crc_flag::bypass_data_reflection;
    };

    /**
     * @todo
     */
    struct copy_crc_additional_option
    {
        using type = copy_crc_additional_options;

        static constexpr type none                   = type();
        static constexpr type read_crc_seed          = crc_flag::read_crc_seed;
        static constexpr type bypass_reflection      = crc_flag::bypass_crc_inversion_and_reflection;
        static constexpr type bypass_data_reflection = crc_flag::bypass_data_reflection;
    };

    /**
     * @todo
     */
    struct dualcast_additional_option
    {
        using type = dualcast_additional_options;

        static constexpr type none                                = type();
        static constexpr type destination_2_steering_tag_selector = dualcast_flag::destination_2_steering_tag_selector;
    };
}  // namespace dml::ml

#endif  //DML_COMMON_OPTIONS_HPP
