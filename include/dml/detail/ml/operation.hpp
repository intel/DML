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

#ifndef DML_ML_OPERATION_HPP
#define DML_ML_OPERATION_HPP

#include <dml/detail/common/types.hpp>
#include <dml/detail/ml/options.hpp>

namespace dml::detail::ml
{
    struct alignas(64u) operation
    {
        byte_t bytes[64u];
    };

    [[nodiscard]] operation make_nop_operation(nop_options options) noexcept;

    [[nodiscard]] operation make_drain_operation(address_t              readback_address_1,
                                                 address_t              readback_address_2,
                                                 drain_options          options,
                                                 drain_specific_options specific_options) noexcept;

    [[nodiscard]] operation make_mem_move_operation(const byte_t    *src,
                                                    byte_t          *dst,
                                                    transfer_size_t  size,
                                                    mem_move_options options) noexcept;

    [[nodiscard]] operation make_fill_operation(uint64_t pattern, byte_t *dst, transfer_size_t size, fill_options options) noexcept;

    [[nodiscard]] operation make_dualcast_operation(const byte_t             *src,
                                                    byte_t                   *dst1,
                                                    byte_t                   *dst2,
                                                    transfer_size_t           size,
                                                    dualcast_options          options,
                                                    dualcast_specific_options specific_options) noexcept;

    [[nodiscard]] operation make_compare_operation(const byte_t   *src1,
                                                   const byte_t   *src2,
                                                   transfer_size_t size,
                                                   compare_options options,
                                                   compare_result  expected_result) noexcept;

    [[nodiscard]] operation make_compare_pattern_operation(uint64_t                pattern,
                                                           const byte_t           *src,
                                                           transfer_size_t         size,
                                                           compare_pattern_options options,
                                                           compare_result          expected_result) noexcept;

    [[nodiscard]] operation make_crc_operation(const byte_t        *src,
                                               transfer_size_t      size,
                                               crc_value_t          crc_seed,
                                               crc_options          options,
                                               crc_specific_options specific_options) noexcept;

    [[nodiscard]] operation make_copy_crc_operation(const byte_t             *src,
                                                    byte_t                   *dst,
                                                    transfer_size_t           size,
                                                    crc_value_t               crc_seed,
                                                    copy_crc_options          options,
                                                    copy_crc_specific_options specific_options) noexcept;

    [[nodiscard]] operation make_create_delta_operation(const byte_t        *src1,
                                                        const byte_t        *src2,
                                                        transfer_size_t      size,
                                                        byte_t              *delta_record,
                                                        transfer_size_t      delta_max_size,
                                                        create_delta_options options,
                                                        create_delta_result  expected_result) noexcept;

    [[nodiscard]] operation make_apply_delta_operation(const byte_t       *delta_record,
                                                       transfer_size_t     delta_size,
                                                       byte_t             *dst,
                                                       transfer_size_t     size,
                                                       apply_delta_options options) noexcept;

    [[nodiscard]] operation make_cache_flush_operation(byte_t *dst, transfer_size_t size, cache_flush_options options) noexcept;

    [[nodiscard]] operation make_dif_check_operation(const byte_t        *src,
                                                     transfer_size_t      transfer_size,
                                                     dif_parameters       src_parameters,
                                                     dif_check_options    options,
                                                     dif_specific_options specific_options,
                                                     dif_source_options   source_options) noexcept;

    [[nodiscard]] operation make_dif_insert_operation(const byte_t           *src,
                                                      byte_t                 *dst,
                                                      transfer_size_t         transfer_size,
                                                      dif_parameters          dst_parameters,
                                                      dif_insert_options      options,
                                                      dif_specific_options    specific_options,
                                                      dif_destination_options destination_options) noexcept;

    [[nodiscard]] operation make_dif_strip_operation(const byte_t        *src,
                                                     byte_t              *dst,
                                                     transfer_size_t      transfer_size,
                                                     dif_parameters       src_parameters,
                                                     dif_strip_options    options,
                                                     dif_specific_options specific_options,
                                                     dif_source_options   source_options) noexcept;

    [[nodiscard]] operation make_dif_update_operation(const byte_t           *src,
                                                      byte_t                 *dst,
                                                      transfer_size_t         transfer_size,
                                                      dif_parameters          src_parameters,
                                                      dif_parameters          dst_parameters,
                                                      dif_update_options      options,
                                                      dif_specific_options    specific_options,
                                                      dif_source_options      source_options,
                                                      dif_destination_options destination_options) noexcept;

    [[nodiscard]] operation make_batch_operation(const operation *src, transfer_size_t length, batch_options options) noexcept;
}  // namespace dml::detail::ml

#endif  //DML_ML_OPERATION_HPP
