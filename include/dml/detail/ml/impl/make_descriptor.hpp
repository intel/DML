/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_ML_MAKE_DESCRIPTOR_HPP
#define DML_ML_MAKE_DESCRIPTOR_HPP

#include <dml/detail/common/types.hpp>
#include <dml/detail/ml/options.hpp>

namespace dml::detail::ml::impl
{
    [[nodiscard]] descriptor make_nop_descriptor(nop_options options) noexcept;

    [[nodiscard]] descriptor make_drain_descriptor(address_t              readback_address_1,
                                                   address_t              readback_address_2,
                                                   drain_options          options,
                                                   drain_specific_options specific_options) noexcept;

    [[nodiscard]] descriptor make_mem_move_descriptor(const byte_t    *src,
                                                      byte_t          *dst,
                                                      transfer_size_t  size,
                                                      mem_move_options options) noexcept;

    [[nodiscard]] descriptor make_fill_descriptor(uint64_t pattern, byte_t *dst, transfer_size_t size, fill_options options) noexcept;

    [[nodiscard]] descriptor make_dualcast_descriptor(const byte_t             *src,
                                                      byte_t                   *dst1,
                                                      byte_t                   *dst2,
                                                      transfer_size_t           size,
                                                      dualcast_options          options,
                                                      dualcast_specific_options specific_options) noexcept;

    [[nodiscard]] descriptor make_compare_descriptor(const byte_t   *src1,
                                                     const byte_t   *src2,
                                                     transfer_size_t size,
                                                     compare_options options,
                                                     compare_result  expected_result) noexcept;

    [[nodiscard]] descriptor make_compare_pattern_descriptor(uint64_t                pattern,
                                                             const byte_t           *src,
                                                             transfer_size_t         size,
                                                             compare_pattern_options options,
                                                             compare_result          expected_result) noexcept;

    [[nodiscard]] descriptor make_crc_descriptor(const byte_t        *src,
                                                 transfer_size_t      size,
                                                 crc_value_t          crc_seed,
                                                 crc_options          options,
                                                 crc_specific_options specific_options) noexcept;

    [[nodiscard]] descriptor make_copy_crc_descriptor(const byte_t             *src,
                                                      byte_t                   *dst,
                                                      transfer_size_t           size,
                                                      crc_value_t               crc_seed,
                                                      copy_crc_options          options,
                                                      copy_crc_specific_options specific_options) noexcept;

    [[nodiscard]] descriptor make_create_delta_descriptor(const byte_t        *src1,
                                                          const byte_t        *src2,
                                                          transfer_size_t      size,
                                                          byte_t              *delta_record,
                                                          transfer_size_t      delta_max_size,
                                                          create_delta_options options,
                                                          create_delta_result  expected_result) noexcept;

    [[nodiscard]] descriptor make_apply_delta_descriptor(const byte_t       *delta_record,
                                                         transfer_size_t     delta_size,
                                                         byte_t             *dst,
                                                         transfer_size_t     size,
                                                         apply_delta_options options) noexcept;

    [[nodiscard]] descriptor make_cache_flush_descriptor(byte_t *dst, transfer_size_t size, cache_flush_options options) noexcept;

    [[nodiscard]] descriptor make_dif_check_descriptor(const byte_t        *src,
                                                       transfer_size_t      transfer_size,
                                                       dif_parameters       src_parameters,
                                                       dif_check_options    options,
                                                       dif_specific_options specific_options,
                                                       dif_source_options   source_options) noexcept;

    [[nodiscard]] descriptor make_dif_insert_descriptor(const byte_t           *src,
                                                        byte_t                 *dst,
                                                        transfer_size_t         transfer_size,
                                                        dif_parameters          dst_parameters,
                                                        dif_insert_options      options,
                                                        dif_specific_options    specific_options,
                                                        dif_destination_options destination_options) noexcept;

    [[nodiscard]] descriptor make_dif_strip_descriptor(const byte_t        *src,
                                                       byte_t              *dst,
                                                       transfer_size_t      transfer_size,
                                                       dif_parameters       src_parameters,
                                                       dif_strip_options    options,
                                                       dif_specific_options specific_options,
                                                       dif_source_options   source_options) noexcept;

    [[nodiscard]] descriptor make_dif_update_descriptor(const byte_t           *src,
                                                        byte_t                 *dst,
                                                        transfer_size_t         transfer_size,
                                                        dif_parameters          src_parameters,
                                                        dif_parameters          dst_parameters,
                                                        dif_update_options      options,
                                                        dif_specific_options    specific_options,
                                                        dif_source_options      source_options,
                                                        dif_destination_options destination_options) noexcept;

    [[nodiscard]] descriptor make_batch_descriptor(const descriptor *src, transfer_size_t length, batch_options options) noexcept;
}  // namespace dml::detail::ml::impl

#endif  //DML_ML_MAKE_DESCRIPTOR_HPP
