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

#include <core/descriptor_views.hpp>
#include <core/operations.hpp>
#include <core/types.hpp>
#include <cstring>
#include <dml/detail/ml/operation.hpp>

#include "ml_utils.hpp"

namespace dml::detail::ml
{
    operation make_nop_operation(const nop_options options) noexcept
    {
        auto descriptor = core::descriptor{};
        auto view       = core::nop_descriptor(descriptor);

        view.operation() = to_underlying(core::operation::nop);
        view.flags()     = static_cast<flags_t>(options);

        return as_ml(descriptor);
    }

    operation make_drain_operation(address_t                    readback_address_1,
                                   address_t                    readback_address_2,
                                   const drain_options          options,
                                   const drain_specific_options specific_options) noexcept
    {
        auto descriptor = core::descriptor{};
        auto view       = core::drain_descriptor(descriptor);

        view.operation()                = to_underlying(core::operation::drain);
        view.readback_address_1()       = readback_address_1;
        view.readback_address_2()       = readback_address_2;
        view.flags()                    = static_cast<flags_t>(options);
        view.operation_specific_flags() = static_cast<operation_specific_flags_t>(specific_options);

        return as_ml(descriptor);
    }

    operation make_mem_move_operation(const byte_t *const    src,
                                      byte_t *const          dst,
                                      const transfer_size_t  size,
                                      const mem_move_options options) noexcept
    {
        auto descriptor = core::descriptor{};
        auto view       = core::mem_move_descriptor(descriptor);

        view.operation()           = to_underlying(core::operation::memory_move);
        view.source_address()      = reinterpret_cast<address_t>(src);
        view.destination_address() = reinterpret_cast<address_t>(dst);
        view.transfer_size()       = size;
        view.flags()               = static_cast<flags_t>(options);

        return as_ml(descriptor);
    }

    operation make_fill_operation(const uint64_t        pattern,
                                  byte_t *const         dst,
                                  const transfer_size_t size,
                                  const fill_options    options) noexcept
    {
        auto descriptor = core::descriptor{};
        auto view       = core::fill_descriptor(descriptor);

        view.operation()           = to_underlying(core::operation::fill);
        view.pattern()             = pattern;
        view.destination_address() = reinterpret_cast<address_t>(dst);
        view.transfer_size()       = size;
        view.flags()               = static_cast<flags_t>(options);

        return as_ml(descriptor);
    }

    operation make_dualcast_operation(const byte_t *const             src,
                                      byte_t *const                   dst1,
                                      byte_t *const                   dst2,
                                      const transfer_size_t           size,
                                      const dualcast_options          options,
                                      const dualcast_specific_options specific_options) noexcept
    {
        auto descriptor = core::descriptor{};
        auto view       = core::dualcast_descriptor(descriptor);

        view.operation()                = to_underlying(core::operation::dualcast);
        view.source_address()           = reinterpret_cast<address_t>(src);
        view.destination_1_address()    = reinterpret_cast<address_t>(dst1);
        view.destination_2_address()    = reinterpret_cast<address_t>(dst2);
        view.transfer_size()            = size;
        view.flags()                    = static_cast<flags_t>(options);
        view.operation_specific_flags() = static_cast<operation_specific_flags_t>(specific_options);

        return as_ml(descriptor);
    }

    operation make_compare_operation(const byte_t *const   src1,
                                     const byte_t *const   src2,
                                     const transfer_size_t size,
                                     const compare_options options,
                                     const compare_result  expected_result) noexcept
    {
        auto descriptor = core::descriptor{};
        auto view       = core::compare_descriptor(descriptor);

        view.operation()        = to_underlying(core::operation::compare);
        view.source_1_address() = reinterpret_cast<address_t>(src1);
        view.source_2_address() = reinterpret_cast<address_t>(src2);
        view.transfer_size()    = size;
        view.flags()            = static_cast<flags_t>(options);
        view.expected_result()  = static_cast<result_t>(expected_result);

        return as_ml(descriptor);
    }

    operation make_compare_pattern_operation(const uint64_t                pattern,
                                             const byte_t                 *src,
                                             const transfer_size_t         size,
                                             const compare_pattern_options options,
                                             const compare_result          expected_result) noexcept
    {
        auto descriptor = core::descriptor{};
        auto view       = core::compare_pattern_descriptor(descriptor);

        view.operation()       = to_underlying(core::operation::compare_pattern);
        view.pattern()         = pattern;
        view.source_address()  = reinterpret_cast<address_t>(src);
        view.transfer_size()   = size;
        view.flags()           = static_cast<flags_t>(options);
        view.expected_result() = static_cast<result_t>(expected_result);

        return as_ml(descriptor);
    }

    operation make_crc_operation(const byte_t *const        src,
                                 const transfer_size_t      size,
                                 const crc_value_t          crc_seed,
                                 const crc_options          options,
                                 const crc_specific_options specific_options) noexcept
    {
        auto descriptor = core::descriptor{};
        auto view       = core::crc_descriptor(descriptor);

        view.operation()                = to_underlying(core::operation::crc);
        view.source_address()           = reinterpret_cast<address_t>(src);
        view.transfer_size()            = size;
        view.flags()                    = static_cast<flags_t>(options);
        view.operation_specific_flags() = static_cast<operation_specific_flags_t>(specific_options);
        view.crc_seed()                 = crc_seed;

        return as_ml(descriptor);
    }

    operation make_copy_crc_operation(const byte_t *const             src,
                                      byte_t *const                   dst,
                                      const transfer_size_t           size,
                                      const crc_value_t               crc_seed,
                                      const copy_crc_options          options,
                                      const copy_crc_specific_options specific_options) noexcept
    {
        auto descriptor = core::descriptor{};
        auto view       = core::copy_crc_descriptor(descriptor);

        view.operation()                = to_underlying(core::operation::copy_crc);
        view.source_address()           = reinterpret_cast<address_t>(src);
        view.destination_address()      = reinterpret_cast<address_t>(dst);
        view.transfer_size()            = size;
        view.flags()                    = static_cast<flags_t>(options);
        view.operation_specific_flags() = static_cast<operation_specific_flags_t>(specific_options);
        view.crc_seed()                 = crc_seed;

        return as_ml(descriptor);
    }

    operation make_create_delta_operation(const byte_t *const        src1,
                                          const byte_t *const        src2,
                                          const transfer_size_t      size,
                                          byte_t *const              delta_record,
                                          const transfer_size_t      delta_max_size,
                                          const create_delta_options options,
                                          const create_delta_result  expected_result) noexcept
    {
        auto descriptor = core::descriptor{};
        auto view       = core::create_delta_descriptor(descriptor);

        view.operation()                 = to_underlying(core::operation::create_delta);
        view.source_1_address()          = reinterpret_cast<address_t>(src1);
        view.source_2_address()          = reinterpret_cast<address_t>(src2);
        view.delta_record_address()      = reinterpret_cast<address_t>(delta_record);
        view.transfer_size()             = size;
        view.maximum_delta_record_size() = delta_max_size;
        view.flags()                     = static_cast<flags_t>(options);
        view.expected_result_mask()      = static_cast<result_t>(expected_result);

        return as_ml(descriptor);
    }

    operation make_apply_delta_operation(const byte_t *const       delta_record,
                                         const transfer_size_t     delta_size,
                                         byte_t *const             dst,
                                         const transfer_size_t     size,
                                         const apply_delta_options options) noexcept
    {
        auto descriptor = core::descriptor{};
        auto view       = core::apply_delta_descriptor(descriptor);

        view.operation()            = to_underlying(core::operation::apply_delta);
        view.delta_record_address() = reinterpret_cast<address_t>(delta_record);
        view.destination_address()  = reinterpret_cast<address_t>(dst);
        view.transfer_size()        = size;
        view.delta_record_size()    = delta_size;
        view.flags()                = static_cast<flags_t>(options);

        return as_ml(descriptor);
    }

    operation make_cache_flush_operation(byte_t *const dst, const transfer_size_t size, const cache_flush_options options) noexcept
    {
        auto descriptor = core::descriptor{};
        auto view       = core::cache_flush_descriptor(descriptor);

        view.operation()           = to_underlying(core::operation::cache_flush);
        view.destination_address() = reinterpret_cast<address_t>(dst);
        view.transfer_size()       = size;
        view.flags()               = static_cast<flags_t>(options);

        return as_ml(descriptor);
    }

    operation make_dif_check_operation(const byte_t        *src,
                                       transfer_size_t      transfer_size,
                                       dif_parameters       src_parameters,
                                       dif_check_options    options,
                                       dif_specific_options specific_options,
                                       dif_source_options   source_options) noexcept
    {
        auto descriptor = core::descriptor{};
        auto view       = core::dif_check_descriptor(descriptor);

        view.operation()           = to_underlying(core::operation::dif_check);
        view.source_address()      = reinterpret_cast<address_t>(src);
        view.transfer_size()       = transfer_size;
        view.flags()               = static_cast<flags_t>(options);
        view.dif_flags()           = static_cast<dif_flags_t>(specific_options);
        view.source_dif_flags()    = static_cast<dif_flags_t>(source_options);
        view.source_ref_tag()      = src_parameters.ref_tag_seed;
        view.source_app_tag()      = src_parameters.app_tag_seed;
        view.source_app_tag_mask() = src_parameters.app_tag_mask;

        return as_ml(descriptor);
    }

    operation make_dif_insert_operation(const byte_t           *src,
                                        byte_t                 *dst,
                                        transfer_size_t         transfer_size,
                                        dif_parameters          dst_parameters,
                                        dif_insert_options      options,
                                        dif_specific_options    specific_options,
                                        dif_destination_options destination_options) noexcept
    {
        auto descriptor = core::descriptor{};
        auto view       = core::dif_insert_descriptor(descriptor);

        view.operation()                = to_underlying(core::operation::dif_insert);
        view.source_address()           = reinterpret_cast<address_t>(src);
        view.destination_address()      = reinterpret_cast<address_t>(dst);
        view.transfer_size()            = transfer_size;
        view.flags()                    = static_cast<flags_t>(options);
        view.dif_flags()                = static_cast<dif_flags_t>(specific_options);
        view.destination_dif_flags()    = static_cast<dif_flags_t>(destination_options);
        view.destination_ref_tag()      = dst_parameters.ref_tag_seed;
        view.destination_app_tag()      = dst_parameters.app_tag_seed;
        view.destination_app_tag_mask() = dst_parameters.app_tag_mask;

        return as_ml(descriptor);
    }

    operation make_dif_strip_operation(const byte_t        *src,
                                       byte_t              *dst,
                                       transfer_size_t      transfer_size,
                                       dif_parameters       src_parameters,
                                       dif_strip_options    options,
                                       dif_specific_options specific_options,
                                       dif_source_options   source_options) noexcept
    {
        auto descriptor = core::descriptor{};
        auto view       = core::dif_strip_descriptor(descriptor);

        view.operation()           = to_underlying(core::operation::dif_strip);
        view.source_address()      = reinterpret_cast<address_t>(src);
        view.destination_address() = reinterpret_cast<address_t>(dst);
        view.transfer_size()       = transfer_size;
        view.flags()               = static_cast<flags_t>(options);
        view.dif_flags()           = static_cast<dif_flags_t>(specific_options);
        view.source_dif_flags()    = static_cast<dif_flags_t>(source_options);
        view.source_ref_tag()      = src_parameters.ref_tag_seed;
        view.source_app_tag()      = src_parameters.app_tag_seed;
        view.source_app_tag_mask() = src_parameters.app_tag_mask;

        return as_ml(descriptor);
    }

    operation make_dif_update_operation(const byte_t           *src,
                                        byte_t                 *dst,
                                        transfer_size_t         transfer_size,
                                        dif_parameters          src_parameters,
                                        dif_parameters          dst_parameters,
                                        dif_update_options      options,
                                        dif_specific_options    specific_options,
                                        dif_source_options      source_options,
                                        dif_destination_options destination_options) noexcept
    {
        auto descriptor = core::descriptor{};
        auto view       = core::dif_update_descriptor(descriptor);

        view.operation()           = to_underlying(core::operation::dif_update);
        view.source_address()      = reinterpret_cast<address_t>(src);
        view.destination_address() = reinterpret_cast<address_t>(dst);
        view.transfer_size()       = transfer_size;
        view.flags()               = static_cast<flags_t>(options);
        view.dif_flags()           = static_cast<dif_flags_t>(specific_options);

        view.source_dif_flags()    = static_cast<dif_flags_t>(source_options);
        view.source_ref_tag()      = src_parameters.ref_tag_seed;
        view.source_app_tag()      = src_parameters.app_tag_seed;
        view.source_app_tag_mask() = src_parameters.app_tag_mask;

        view.destination_dif_flags()    = static_cast<dif_flags_t>(destination_options);
        view.destination_ref_tag()      = dst_parameters.ref_tag_seed;
        view.destination_app_tag()      = dst_parameters.app_tag_seed;
        view.destination_app_tag_mask() = dst_parameters.app_tag_mask;

        return as_ml(descriptor);
    }

    operation make_batch_operation(const operation *const src, const transfer_size_t length, const batch_options options) noexcept
    {
        auto descriptor = core::descriptor{};
        auto view       = core::batch_descriptor(descriptor);

        view.operation()               = to_underlying(core::operation::batch);
        view.descriptor_list_address() = reinterpret_cast<address_t>(src);
        view.descriptors_count()       = length;
        view.flags()                   = static_cast<flags_t>(options);

        return as_ml(descriptor);
    }
}  // namespace dml::detail::ml
