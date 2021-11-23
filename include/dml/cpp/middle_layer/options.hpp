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
    struct nop_option
    {
        using type = nop_options;

        static constexpr type none  = type();
        static constexpr type fence = flag::fence;
    };

    /**
     * @todo
     */
    struct drain_option
    {
        using type = drain_options;

        static constexpr type none                  = type();
        static constexpr type address_1_tc_selector = flag::address_1_tc_selector;
        static constexpr type address_2_tc_selector = flag::address_2_tc_selector;
    };

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
    struct dif_check_option
    {
        using type = dif_check_options;

        static constexpr type none                  = type(0);                     /**< @todo */
        static constexpr type fence                 = flag::fence;                 /**< @todo */
        static constexpr type block_on_fault        = flag::block_on_fault;        /**< @todo */
        static constexpr type address_1_tc_selector = flag::address_1_tc_selector; /**< @todo */
    };

    /**
     * @todo
     */
    struct dif_insert_option
    {
        using type = dif_insert_options;

        static constexpr type none                              = type(0);                                 /**< @todo */
        static constexpr type fence                             = flag::fence;                             /**< @todo */
        static constexpr type block_on_fault                    = flag::block_on_fault;                    /**< @todo */
        static constexpr type cache_control                     = flag::cache_control;                     /**< @todo */
        static constexpr type address_1_tc_selector             = flag::address_1_tc_selector;             /**< @todo */
        static constexpr type address_2_tc_selector             = flag::address_2_tc_selector;             /**< @todo */
        static constexpr type strict_ordering                   = flag::strict_ordering;                   /**< @todo */
        static constexpr type destination_readback              = flag::destination_readback;              /**< @todo */
        static constexpr type destination_steering_tag_selector = flag::destination_steering_tag_selector; /**< @todo */
    };

    /**
     * @todo
     */
    struct dif_strip_option
    {
        using type = dif_strip_options;

        static constexpr type none                              = type(0);                                 /**< @todo */
        static constexpr type fence                             = flag::fence;                             /**< @todo */
        static constexpr type block_on_fault                    = flag::block_on_fault;                    /**< @todo */
        static constexpr type cache_control                     = flag::cache_control;                     /**< @todo */
        static constexpr type address_1_tc_selector             = flag::address_1_tc_selector;             /**< @todo */
        static constexpr type address_2_tc_selector             = flag::address_2_tc_selector;             /**< @todo */
        static constexpr type strict_ordering                   = flag::strict_ordering;                   /**< @todo */
        static constexpr type destination_readback              = flag::destination_readback;              /**< @todo */
        static constexpr type destination_steering_tag_selector = flag::destination_steering_tag_selector; /**< @todo */
    };

    /**
     * @todo
     */
    struct dif_update_option
    {
        using type = dif_update_options;

        static constexpr type none                              = type(0);                                 /**< @todo */
        static constexpr type fence                             = flag::fence;                             /**< @todo */
        static constexpr type block_on_fault                    = flag::block_on_fault;                    /**< @todo */
        static constexpr type cache_control                     = flag::cache_control;                     /**< @todo */
        static constexpr type address_1_tc_selector             = flag::address_1_tc_selector;             /**< @todo */
        static constexpr type address_2_tc_selector             = flag::address_2_tc_selector;             /**< @todo */
        static constexpr type strict_ordering                   = flag::strict_ordering;                   /**< @todo */
        static constexpr type destination_readback              = flag::destination_readback;              /**< @todo */
        static constexpr type destination_steering_tag_selector = flag::destination_steering_tag_selector; /**< @todo */
    };

    /**
     * @todo
     */
    struct compare_expected_result_option
    {
        static constexpr auto expect_equal     = compare_expected_result_options(0);
        static constexpr auto expect_not_equal = compare_expected_result_options(1);
    };

    /**
     * @todo
     */
    struct delta_expected_result_option
    {
        static constexpr auto expect_equal     = delta_expected_result_options(1);
        static constexpr auto expect_not_equal = delta_expected_result_options(2);
        static constexpr auto expect_overflow  = delta_expected_result_options(4);
    };

    /**
     * @todo
     */
    struct drain_additional_option
    {
        using type = drain_additional_options;

        static constexpr type none                            = type();
        static constexpr type readback_address_1_valid        = drain_flag::readback_address_1_valid;
        static constexpr type readback_address_2_valid        = drain_flag::readback_address_2_valid;
        static constexpr type suppress_tc_a_implicit_readback = drain_flag::suppress_tc_a_implicit_readback;
        static constexpr type suppress_tc_b_implicit_readback = drain_flag::suppress_tc_b_implicit_readback;
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

    /**
     * @todo
     */
    struct dif_additional_option
    {
        using type = dif_additional_options;

        static constexpr type block_size_512    = dif_flag::block_size_512;    /**< @todo */
        static constexpr type block_size_520    = dif_flag::block_size_520;    /**< @todo */
        static constexpr type block_size_4096   = dif_flag::block_size_4096;   /**< @todo */
        static constexpr type block_size_4104   = dif_flag::block_size_4104;   /**< @todo */
        static constexpr type invert_crc_seed   = dif_flag::invert_crc_seed;   /**< @todo */
        static constexpr type invert_crc_result = dif_flag::invert_crc_result; /**< @todo */
    };

    /**
     * @todo
     */
    struct dif_additional_src_option
    {
        using type = dif_additional_src_options;

        static constexpr type none                      = type(0);                                 /**< @todo */
        static constexpr type enable_all_f_detect_error = dif_src_flag::enable_all_f_detect_error; /**< @todo */
        static constexpr type all_f_detect              = dif_src_flag::all_f_detect;              /**< @todo */
        static constexpr type app_tag_f_detect          = dif_src_flag::app_tag_f_detect;          /**< @todo */
        static constexpr type app_and_ref_tag_f_detect  = dif_src_flag::app_and_ref_tag_f_detect;  /**< @todo */
        static constexpr type incrementing_app_tag_type = dif_src_flag::incrementing_app_tag_type; /**< @todo */
        static constexpr type guard_check_disable       = dif_src_flag::guard_check_disable;       /**< @todo */
        static constexpr type ref_tag_check_disable     = dif_src_flag::ref_tag_check_disable;     /**< @todo */
        static constexpr type fixed_ref_tag_type        = dif_src_flag::fixed_ref_tag_type;        /**< @todo */
    };

    /**
     * @todo
     */
    struct dif_additional_dst_option
    {
        using type = dif_additional_dst_options;

        static constexpr type none                      = type(0);                                 /**< @todo */
        static constexpr type app_tag_pass_through      = dif_dst_flag::app_tag_pass_through;      /**< @todo */
        static constexpr type incrementing_app_tag_type = dif_dst_flag::incrementing_app_tag_type; /**< @todo */
        static constexpr type guard_field_pass_through  = dif_dst_flag::guard_field_pass_through;  /**< @todo */
        static constexpr type ref_tag_pass_through      = dif_dst_flag::ref_tag_pass_through;      /**< @todo */
        static constexpr type fixed_ref_tag_type        = dif_dst_flag::fixed_ref_tag_type;        /**< @todo */
    };
}  // namespace dml::ml

#endif  //DML_COMMON_OPTIONS_HPP
