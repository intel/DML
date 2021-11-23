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

#include "descriptor.hpp"
#include "descriptor_views.hpp"
#include "options.hpp"
#include "types.hpp"
#include "values.hpp"

namespace dml::ml
{
    /**
     * @todo
     */
    [[nodiscard]] inline descriptor make_nop_descriptor(const nop_options options) noexcept
    {
        auto dsc  = descriptor();
        auto view = views::nop_descriptor(dsc);

        view.operation() = static_cast<operation_t>(operation::nop);
        view.flags()     = static_cast<flags_t>(options);

        return dsc;
    }

    /**
     * @todo
     */
    [[nodiscard]] inline descriptor make_drain_descriptor(address_t                      readback_address_1,
                                                          address_t                      readback_address_2,
                                                          const drain_options            options,
                                                          const drain_additional_options additional_options) noexcept
    {
        auto dsc  = descriptor();
        auto view = views::drain_descriptor(dsc);

        view.operation()                = static_cast<operation_t>(operation::drain);
        view.readback_address_1()       = readback_address_1;
        view.readback_address_2()       = readback_address_2;
        view.flags()                    = static_cast<flags_t>(options);
        view.operation_specific_flags() = static_cast<operation_specific_flags_t>(additional_options);

        return dsc;
    }

    /**
     * @todo
     */
    [[nodiscard]] inline descriptor make_mem_move_descriptor(const byte_t *const    src,
                                                             byte_t *const          dst,
                                                             const transfer_size_t  size,
                                                             const mem_move_options options) noexcept
    {
        auto dsc  = descriptor();
        auto view = views::mem_move_descriptor(dsc);

        view.operation()           = static_cast<operation_t>(operation::memory_move);
        view.source_address()      = reinterpret_cast<address_t>(src);
        view.destination_address() = reinterpret_cast<address_t>(dst);
        view.transfer_size()       = size;
        view.flags()               = static_cast<flags_t>(options);

        return dsc;
    }

    /**
     * @todo
     */
    [[nodiscard]] inline descriptor make_fill_descriptor(const uint64_t        pattern,
                                                         byte_t *const         dst,
                                                         const transfer_size_t size,
                                                         const fill_options    options) noexcept
    {
        auto dsc  = descriptor();
        auto view = views::fill_descriptor(dsc);

        view.operation()           = static_cast<operation_t>(operation::fill);
        view.pattern()             = pattern;
        view.destination_address() = reinterpret_cast<address_t>(dst);
        view.transfer_size()       = size;
        view.flags()               = static_cast<flags_t>(options);

        return dsc;
    }

    /**
     * @todo
     */
    [[nodiscard]] inline descriptor make_dualcast_descriptor(const byte_t *const               src,
                                                             byte_t *const                     dst1,
                                                             byte_t *const                     dst2,
                                                             const transfer_size_t             size,
                                                             const dualcast_options            options,
                                                             const dualcast_additional_options additional_options) noexcept
    {
        auto dsc  = descriptor();
        auto view = views::dualcast_descriptor(dsc);

        view.operation()                = static_cast<operation_t>(operation::dualcast);
        view.source_address()           = reinterpret_cast<address_t>(src);
        view.destination_1_address()    = reinterpret_cast<address_t>(dst1);
        view.destination_2_address()    = reinterpret_cast<address_t>(dst2);
        view.transfer_size()            = size;
        view.flags()                    = static_cast<flags_t>(options);
        view.operation_specific_flags() = static_cast<operation_specific_flags_t>(additional_options);

        return dsc;
    }

    /**
     * @todo
     */
    [[nodiscard]] inline descriptor make_compare_descriptor(const byte_t *const                   src1,
                                                            const byte_t *const                   src2,
                                                            const transfer_size_t                 size,
                                                            const compare_options                 options,
                                                            const compare_expected_result_options expected_result) noexcept
    {
        auto dsc  = descriptor();
        auto view = views::compare_descriptor(dsc);

        view.operation()        = static_cast<operation_t>(operation::compare);
        view.source_1_address() = reinterpret_cast<address_t>(src1);
        view.source_2_address() = reinterpret_cast<address_t>(src2);
        view.transfer_size()    = size;
        view.flags()            = static_cast<flags_t>(options);
        view.expected_result()  = static_cast<result_t>(expected_result);

        return dsc;
    }

    /**
     * @todo
     */
    [[nodiscard]] inline descriptor make_compare_pattern_descriptor(const uint64_t                        pattern,
                                                                    const byte_t                         *src,
                                                                    const transfer_size_t                 size,
                                                                    const compare_pattern_options         options,
                                                                    const compare_expected_result_options expected_result) noexcept
    {
        auto dsc  = descriptor();
        auto view = views::compare_pattern_descriptor(dsc);

        view.operation()       = static_cast<operation_t>(operation::compare_pattern);
        view.pattern()         = pattern;
        view.source_address()  = reinterpret_cast<address_t>(src);
        view.transfer_size()   = size;
        view.flags()           = static_cast<flags_t>(options);
        view.expected_result() = static_cast<result_t>(expected_result);

        return dsc;
    }

    /**
     * @todo
     */
    [[nodiscard]] inline descriptor make_crc_descriptor(const byte_t *const          src,
                                                        const transfer_size_t        size,
                                                        const crc_value_t            crc_seed,
                                                        const crc_options            options,
                                                        const crc_additional_options additional_options) noexcept
    {
        auto dsc  = descriptor();
        auto view = views::crc_descriptor(dsc);

        view.operation()                = static_cast<operation_t>(operation::crc);
        view.source_address()           = reinterpret_cast<address_t>(src);
        view.transfer_size()            = size;
        view.flags()                    = static_cast<flags_t>(options);
        view.operation_specific_flags() = static_cast<operation_specific_flags_t>(additional_options);
        view.crc_seed()                 = crc_seed;

        return dsc;
    }

    /**
     * @todo
     */
    [[nodiscard]] inline descriptor make_copy_crc_descriptor(const byte_t *const               src,
                                                             byte_t *const                     dst,
                                                             const transfer_size_t             size,
                                                             const crc_value_t                 crc_seed,
                                                             const copy_crc_options            options,
                                                             const copy_crc_additional_options additional_options) noexcept
    {
        auto dsc  = descriptor();
        auto view = views::copy_crc_descriptor(dsc);

        view.operation()                = static_cast<operation_t>(operation::copy_crc);
        view.source_address()           = reinterpret_cast<address_t>(src);
        view.destination_address()      = reinterpret_cast<address_t>(dst);
        view.transfer_size()            = size;
        view.flags()                    = static_cast<flags_t>(options);
        view.operation_specific_flags() = static_cast<operation_specific_flags_t>(additional_options);
        view.crc_seed()                 = crc_seed;

        return dsc;
    }

    /**
     * @todo
     */
    [[nodiscard]] inline descriptor make_create_delta_descriptor(const byte_t *const                 src1,
                                                                 const byte_t *const                 src2,
                                                                 const transfer_size_t               size,
                                                                 byte_t *const                       delta_record,
                                                                 const transfer_size_t               delta_max_size,
                                                                 const create_delta_options          options,
                                                                 const delta_expected_result_options expected_result) noexcept
    {
        auto dsc  = descriptor();
        auto view = views::create_delta_descriptor(dsc);

        view.operation()                 = static_cast<operation_t>(operation::create_delta);
        view.source_1_address()          = reinterpret_cast<address_t>(src1);
        view.source_2_address()          = reinterpret_cast<address_t>(src2);
        view.delta_record_address()      = reinterpret_cast<address_t>(delta_record);
        view.transfer_size()             = size;
        view.maximum_delta_record_size() = delta_max_size;
        view.flags()                     = static_cast<flags_t>(options);
        view.expected_result_mask()      = static_cast<result_t>(expected_result);

        return dsc;
    }

    /**
     * @todo
     */
    [[nodiscard]] inline descriptor make_apply_delta_descriptor(const byte_t *const       delta_record,
                                                                const transfer_size_t     delta_size,
                                                                byte_t *const             dst,
                                                                const transfer_size_t     size,
                                                                const apply_delta_options options) noexcept
    {
        auto dsc  = descriptor();
        auto view = views::apply_delta_descriptor(dsc);

        view.operation()            = static_cast<operation_t>(operation::apply_delta);
        view.delta_record_address() = reinterpret_cast<address_t>(delta_record);
        view.destination_address()  = reinterpret_cast<address_t>(dst);
        view.transfer_size()        = size;
        view.delta_record_size()    = delta_size;
        view.flags()                = static_cast<flags_t>(options);

        return dsc;
    }

    /**
     * @todo
     */
    [[nodiscard]] inline descriptor make_cache_flush_descriptor(byte_t *const             dst,
                                                                const transfer_size_t     size,
                                                                const cache_flush_options options) noexcept
    {
        auto dsc  = descriptor();
        auto view = views::cache_flush_descriptor(dsc);

        view.operation()           = static_cast<operation_t>(operation::cache_flush);
        view.destination_address() = reinterpret_cast<address_t>(dst);
        view.transfer_size()       = size;
        view.flags()               = static_cast<flags_t>(options);

        return dsc;
    }

    /**
     * @todo
     */
    [[nodiscard]] inline descriptor make_dif_check_descriptor(const byte_t              *src,
                                                              transfer_size_t            transfer_size,
                                                              dif_parameters             src_parameters,
                                                              dif_check_options          options,
                                                              dif_additional_options     additional_options,
                                                              dif_additional_src_options additional_src_options) noexcept
    {
        auto dsc  = descriptor();
        auto view = views::dif_check_descriptor(dsc);

        view.operation()           = static_cast<operation_t>(operation::dif_check);
        view.source_address()      = reinterpret_cast<address_t>(src);
        view.transfer_size()       = transfer_size;
        view.flags()               = static_cast<flags_t>(options);
        view.dif_flags()           = static_cast<dif_flags_t>(additional_options);
        view.source_dif_flags()    = static_cast<dif_flags_t>(additional_src_options);
        view.source_ref_tag()      = src_parameters.ref_tag_seed;
        view.source_app_tag()      = src_parameters.app_tag_seed;
        view.source_app_tag_mask() = src_parameters.app_tag_mask;

        return dsc;
    }

    /**
     * @todo
     */
    [[nodiscard]] inline descriptor make_dif_insert_descriptor(const byte_t              *src,
                                                               byte_t                    *dst,
                                                               transfer_size_t            transfer_size,
                                                               dif_parameters             dst_parameters,
                                                               dif_insert_options         options,
                                                               dif_additional_options     additional_options,
                                                               dif_additional_dst_options additional_dst_options) noexcept
    {
        auto dsc  = descriptor();
        auto view = views::dif_insert_descriptor(dsc);

        view.operation()                = static_cast<operation_t>(operation::dif_insert);
        view.source_address()           = reinterpret_cast<address_t>(src);
        view.destination_address()      = reinterpret_cast<address_t>(dst);
        view.transfer_size()            = transfer_size;
        view.flags()                    = static_cast<flags_t>(options);
        view.dif_flags()                = static_cast<dif_flags_t>(additional_options);
        view.destination_dif_flags()    = static_cast<dif_flags_t>(additional_dst_options);
        view.destination_ref_tag()      = dst_parameters.ref_tag_seed;
        view.destination_app_tag()      = dst_parameters.app_tag_seed;
        view.destination_app_tag_mask() = dst_parameters.app_tag_mask;

        return dsc;
    }

    /**
     * @todo
     */
    [[nodiscard]] inline descriptor make_dif_strip_descriptor(const byte_t              *src,
                                                              byte_t                    *dst,
                                                              transfer_size_t            transfer_size,
                                                              dif_parameters             src_parameters,
                                                              dif_strip_options          options,
                                                              dif_additional_options     additional_options,
                                                              dif_additional_src_options additional_src_options) noexcept
    {
        auto dsc  = descriptor();
        auto view = views::dif_strip_descriptor(dsc);

        view.operation()           = static_cast<operation_t>(operation::dif_strip);
        view.source_address()      = reinterpret_cast<address_t>(src);
        view.destination_address() = reinterpret_cast<address_t>(dst);
        view.transfer_size()       = transfer_size;
        view.flags()               = static_cast<flags_t>(options);
        view.dif_flags()           = static_cast<dif_flags_t>(additional_options);
        view.source_dif_flags()    = static_cast<dif_flags_t>(additional_src_options);
        view.source_ref_tag()      = src_parameters.ref_tag_seed;
        view.source_app_tag()      = src_parameters.app_tag_seed;
        view.source_app_tag_mask() = src_parameters.app_tag_mask;

        return dsc;
    }

    /**
     * @todo
     */
    [[nodiscard]] inline descriptor make_dif_update_descriptor(const byte_t              *src,
                                                               byte_t                    *dst,
                                                               transfer_size_t            transfer_size,
                                                               dif_parameters             src_parameters,
                                                               dif_parameters             dst_parameters,
                                                               dif_update_options         options,
                                                               dif_additional_options     additional_options,
                                                               dif_additional_src_options additional_src_options,
                                                               dif_additional_dst_options additional_dst_options) noexcept
    {
        auto dsc  = descriptor();
        auto view = views::dif_update_descriptor(dsc);

        view.operation()           = static_cast<operation_t>(operation::dif_update);
        view.source_address()      = reinterpret_cast<address_t>(src);
        view.destination_address() = reinterpret_cast<address_t>(dst);
        view.transfer_size()       = transfer_size;
        view.flags()               = static_cast<flags_t>(options);
        view.dif_flags()           = static_cast<dif_flags_t>(additional_options);

        view.source_dif_flags()    = static_cast<dif_flags_t>(additional_src_options);
        view.source_ref_tag()      = src_parameters.ref_tag_seed;
        view.source_app_tag()      = src_parameters.app_tag_seed;
        view.source_app_tag_mask() = src_parameters.app_tag_mask;

        view.destination_dif_flags()    = static_cast<dif_flags_t>(additional_dst_options);
        view.destination_ref_tag()      = dst_parameters.ref_tag_seed;
        view.destination_app_tag()      = dst_parameters.app_tag_seed;
        view.destination_app_tag_mask() = dst_parameters.app_tag_mask;

        return dsc;
    }

    /**
     * @todo
     */
    [[nodiscard]] inline descriptor make_batch_descriptor(const descriptor *const src,
                                                          const transfer_size_t   length,
                                                          const batch_options     options) noexcept
    {
        auto dsc  = descriptor();
        auto view = views::batch_descriptor(dsc);

        view.operation()               = static_cast<operation_t>(operation::batch);
        view.descriptor_list_address() = reinterpret_cast<address_t>(src);
        view.descriptors_count()       = length;
        view.flags()                   = static_cast<flags_t>(options);

        return dsc;
    }
}  // namespace dml::ml

#endif  //DML_ML_OPERATION_HPP
