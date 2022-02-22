/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <core/operations.hpp>
#include <core/types.hpp>
#include <core/view.hpp>
#include <dml/detail/ml/impl/make_descriptor.hpp>

namespace dml::detail::ml::impl
{
    descriptor make_nop_descriptor(const nop_options options) noexcept
    {
        auto dsc  = descriptor();
        auto view = core::make_view<core::operation::nop>(dsc);

        view.operation() = to_underlying(core::operation::nop);
        view.flags()     = static_cast<flags_t>(options);

        return dsc;
    }

    descriptor make_drain_descriptor(address_t                    readback_address_1,
                                     address_t                    readback_address_2,
                                     const drain_options          options,
                                     const drain_specific_options specific_options) noexcept
    {
        auto dsc  = descriptor();
        auto view = core::make_view<core::operation::drain>(dsc);

        view.operation()                = to_underlying(core::operation::drain);
        view.readback_address_1()       = readback_address_1;
        view.readback_address_2()       = readback_address_2;
        view.flags()                    = static_cast<flags_t>(options);
        view.operation_specific_flags() = static_cast<operation_specific_flags_t>(specific_options);

        return dsc;
    }

    descriptor make_mem_move_descriptor(const byte_t *const    src,
                                        byte_t *const          dst,
                                        const transfer_size_t  size,
                                        const mem_move_options options) noexcept
    {
        auto dsc  = descriptor();
        auto view = core::make_view<core::operation::mem_move>(dsc);

        view.operation()           = to_underlying(core::operation::mem_move);
        view.source_address()      = reinterpret_cast<address_t>(src);
        view.destination_address() = reinterpret_cast<address_t>(dst);
        view.transfer_size()       = size;
        view.flags()               = static_cast<flags_t>(options);

        return dsc;
    }

    descriptor make_fill_descriptor(const uint64_t        pattern,
                                    byte_t *const         dst,
                                    const transfer_size_t size,
                                    const fill_options    options) noexcept
    {
        auto dsc  = descriptor();
        auto view = core::make_view<core::operation::fill>(dsc);

        view.operation()           = to_underlying(core::operation::fill);
        view.pattern()             = pattern;
        view.destination_address() = reinterpret_cast<address_t>(dst);
        view.transfer_size()       = size;
        view.flags()               = static_cast<flags_t>(options);

        return dsc;
    }

    descriptor make_dualcast_descriptor(const byte_t *const             src,
                                        byte_t *const                   dst1,
                                        byte_t *const                   dst2,
                                        const transfer_size_t           size,
                                        const dualcast_options          options,
                                        const dualcast_specific_options specific_options) noexcept
    {
        auto dsc  = descriptor();
        auto view = core::make_view<core::operation::dualcast>(dsc);

        view.operation()                = to_underlying(core::operation::dualcast);
        view.source_address()           = reinterpret_cast<address_t>(src);
        view.destination_1_address()    = reinterpret_cast<address_t>(dst1);
        view.destination_2_address()    = reinterpret_cast<address_t>(dst2);
        view.transfer_size()            = size;
        view.flags()                    = static_cast<flags_t>(options);
        view.operation_specific_flags() = static_cast<operation_specific_flags_t>(specific_options);

        return dsc;
    }

    descriptor make_compare_descriptor(const byte_t *const   src1,
                                       const byte_t *const   src2,
                                       const transfer_size_t size,
                                       const compare_options options,
                                       const compare_result  expected_result) noexcept
    {
        auto dsc  = descriptor();
        auto view = core::make_view<core::operation::compare>(dsc);

        view.operation()        = to_underlying(core::operation::compare);
        view.source_1_address() = reinterpret_cast<address_t>(src1);
        view.source_2_address() = reinterpret_cast<address_t>(src2);
        view.transfer_size()    = size;
        view.flags()            = static_cast<flags_t>(options);
        view.expected_result()  = static_cast<result_t>(expected_result);

        return dsc;
    }

    descriptor make_compare_pattern_descriptor(const uint64_t                pattern,
                                               const byte_t                 *src,
                                               const transfer_size_t         size,
                                               const compare_pattern_options options,
                                               const compare_result          expected_result) noexcept
    {
        auto dsc  = descriptor();
        auto view = core::make_view<core::operation::compare_pattern>(dsc);

        view.operation()       = to_underlying(core::operation::compare_pattern);
        view.pattern()         = pattern;
        view.source_address()  = reinterpret_cast<address_t>(src);
        view.transfer_size()   = size;
        view.flags()           = static_cast<flags_t>(options);
        view.expected_result() = static_cast<result_t>(expected_result);

        return dsc;
    }

    descriptor make_crc_descriptor(const byte_t *const        src,
                                   const transfer_size_t      size,
                                   const crc_value_t          crc_seed,
                                   const crc_options          options,
                                   const crc_specific_options specific_options) noexcept
    {
        auto dsc  = descriptor();
        auto view = core::make_view<core::operation::crc>(dsc);

        view.operation()                = to_underlying(core::operation::crc);
        view.source_address()           = reinterpret_cast<address_t>(src);
        view.transfer_size()            = size;
        view.flags()                    = static_cast<flags_t>(options);
        view.operation_specific_flags() = static_cast<operation_specific_flags_t>(specific_options);
        view.crc_seed()                 = crc_seed;

        return dsc;
    }

    descriptor make_copy_crc_descriptor(const byte_t *const             src,
                                        byte_t *const                   dst,
                                        const transfer_size_t           size,
                                        const crc_value_t               crc_seed,
                                        const copy_crc_options          options,
                                        const copy_crc_specific_options specific_options) noexcept
    {
        auto dsc  = descriptor();
        auto view = core::make_view<core::operation::copy_crc>(dsc);

        view.operation()                = to_underlying(core::operation::copy_crc);
        view.source_address()           = reinterpret_cast<address_t>(src);
        view.destination_address()      = reinterpret_cast<address_t>(dst);
        view.transfer_size()            = size;
        view.flags()                    = static_cast<flags_t>(options);
        view.operation_specific_flags() = static_cast<operation_specific_flags_t>(specific_options);
        view.crc_seed()                 = crc_seed;

        return dsc;
    }

    descriptor make_create_delta_descriptor(const byte_t *const        src1,
                                            const byte_t *const        src2,
                                            const transfer_size_t      size,
                                            byte_t *const              delta_record,
                                            const transfer_size_t      delta_max_size,
                                            const create_delta_options options,
                                            const create_delta_result  expected_result) noexcept
    {
        auto dsc  = descriptor();
        auto view = core::make_view<core::operation::create_delta>(dsc);

        view.operation()                 = to_underlying(core::operation::create_delta);
        view.source_1_address()          = reinterpret_cast<address_t>(src1);
        view.source_2_address()          = reinterpret_cast<address_t>(src2);
        view.delta_record_address()      = reinterpret_cast<address_t>(delta_record);
        view.transfer_size()             = size;
        view.maximum_delta_record_size() = delta_max_size;
        view.flags()                     = static_cast<flags_t>(options);
        view.expected_result_mask()      = static_cast<result_t>(expected_result);

        return dsc;
    }

    descriptor make_apply_delta_descriptor(const byte_t *const       delta_record,
                                           const transfer_size_t     delta_size,
                                           byte_t *const             dst,
                                           const transfer_size_t     size,
                                           const apply_delta_options options) noexcept
    {
        auto dsc  = descriptor();
        auto view = core::make_view<core::operation::apply_delta>(dsc);

        view.operation()            = to_underlying(core::operation::apply_delta);
        view.delta_record_address() = reinterpret_cast<address_t>(delta_record);
        view.destination_address()  = reinterpret_cast<address_t>(dst);
        view.transfer_size()        = size;
        view.delta_record_size()    = delta_size;
        view.flags()                = static_cast<flags_t>(options);

        return dsc;
    }

    descriptor make_cache_flush_descriptor(byte_t *const dst, const transfer_size_t size, const cache_flush_options options) noexcept
    {
        auto dsc  = descriptor();
        auto view = core::make_view<core::operation::cache_flush>(dsc);

        view.operation()           = to_underlying(core::operation::cache_flush);
        view.destination_address() = reinterpret_cast<address_t>(dst);
        view.transfer_size()       = size;
        view.flags()               = static_cast<flags_t>(options);

        return dsc;
    }

    descriptor make_dif_check_descriptor(const byte_t        *src,
                                         transfer_size_t      transfer_size,
                                         dif_parameters       src_parameters,
                                         dif_check_options    options,
                                         dif_specific_options specific_options,
                                         dif_source_options   source_options) noexcept
    {
        auto dsc  = descriptor();
        auto view = core::make_view<core::operation::dif_check>(dsc);

        view.operation()           = to_underlying(core::operation::dif_check);
        view.source_address()      = reinterpret_cast<address_t>(src);
        view.transfer_size()       = transfer_size;
        view.flags()               = static_cast<flags_t>(options);
        view.dif_flags()           = static_cast<dif_flags_t>(specific_options);
        view.source_dif_flags()    = static_cast<dif_flags_t>(source_options);
        view.source_ref_tag()      = src_parameters.ref_tag_seed;
        view.source_app_tag()      = src_parameters.app_tag_seed;
        view.source_app_tag_mask() = src_parameters.app_tag_mask;

        return dsc;
    }

    descriptor make_dif_insert_descriptor(const byte_t           *src,
                                          byte_t                 *dst,
                                          transfer_size_t         transfer_size,
                                          dif_parameters          dst_parameters,
                                          dif_insert_options      options,
                                          dif_specific_options    specific_options,
                                          dif_destination_options destination_options) noexcept
    {
        auto dsc  = descriptor();
        auto view = core::make_view<core::operation::dif_insert>(dsc);

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

        return dsc;
    }

    descriptor make_dif_strip_descriptor(const byte_t        *src,
                                         byte_t              *dst,
                                         transfer_size_t      transfer_size,
                                         dif_parameters       src_parameters,
                                         dif_strip_options    options,
                                         dif_specific_options specific_options,
                                         dif_source_options   source_options) noexcept
    {
        auto dsc  = descriptor();
        auto view = core::make_view<core::operation::dif_strip>(dsc);

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

        return dsc;
    }

    descriptor make_dif_update_descriptor(const byte_t           *src,
                                          byte_t                 *dst,
                                          transfer_size_t         transfer_size,
                                          dif_parameters          src_parameters,
                                          dif_parameters          dst_parameters,
                                          dif_update_options      options,
                                          dif_specific_options    specific_options,
                                          dif_source_options      source_options,
                                          dif_destination_options destination_options) noexcept
    {
        auto dsc  = descriptor();
        auto view = core::make_view<core::operation::dif_update>(dsc);

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

        return dsc;
    }

    descriptor make_batch_descriptor(const descriptor *const src, const transfer_size_t length, const batch_options options) noexcept
    {
        auto dsc  = descriptor();
        auto view = core::make_view<core::operation::batch>(dsc);

        view.operation()               = to_underlying(core::operation::batch);
        view.descriptor_list_address() = reinterpret_cast<address_t>(src);
        view.descriptors_count()       = length;
        view.flags()                   = static_cast<flags_t>(options);

        return dsc;
    }
}  // namespace dml::detail::ml::impl
