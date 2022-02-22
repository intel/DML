/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_ML_OWN_PARTIAL_COMPLETION_HPP
#define DML_ML_OWN_PARTIAL_COMPLETION_HPP

#include <core/descriptor_views.hpp>
#include <dml/detail/ml/impl/make_descriptor.hpp>

namespace dml::detail::ml
{
    static void update_mem_move_for_continuation(descriptor& dsc) noexcept;

    static void update_fill_for_continuation(descriptor& dsc) noexcept;

    static void update_compare_for_continuation(descriptor& dsc) noexcept;

    static void update_compare_pattern_for_continuation(descriptor& dsc) noexcept;

    static void update_create_delta_for_continuation(descriptor& dsc) noexcept;

    static void update_apply_delta_for_continuation(descriptor& dsc) noexcept;

    static void update_dualcast_for_continuation(descriptor& dsc) noexcept;

    static void update_crc_for_continuation(descriptor& dsc) noexcept;

    static void update_copy_crc_for_continuation(descriptor& dsc) noexcept;

    static void update_dif_check_for_continuation(descriptor& dsc) noexcept;

    static void update_dif_insert_for_continuation(descriptor& dsc) noexcept;

    static void update_dif_strip_for_continuation(descriptor& dsc) noexcept;

    static void update_dif_update_for_continuation(descriptor& dsc) noexcept;

    static void update_cache_flush_for_continuation(descriptor& dsc) noexcept;

    static void update_for_continuation(descriptor& dsc) noexcept
    {
        auto record = core::get_completion_record(dsc);
        for (auto& byte : record.bytes)
        {
            byte = 0;
        }

        auto operation = static_cast<core::operation>(core::any_descriptor(dsc).operation());

        switch (operation)
        {
            case core::operation::nop:
                break;
            case core::operation::drain:
                break;
            case core::operation::batch:
                break;
            case core::operation::mem_move:
                update_mem_move_for_continuation(dsc);
                break;
            case core::operation::fill:
                update_fill_for_continuation(dsc);
                break;
            case core::operation::compare:
                update_compare_for_continuation(dsc);
                break;
            case core::operation::compare_pattern:
                update_compare_pattern_for_continuation(dsc);
                break;
            case core::operation::create_delta:
                update_create_delta_for_continuation(dsc);
                break;
            case core::operation::apply_delta:
                update_apply_delta_for_continuation(dsc);
                break;
            case core::operation::dualcast:
                update_dualcast_for_continuation(dsc);
                break;
            case core::operation::crc:
                update_crc_for_continuation(dsc);
                break;
            case core::operation::copy_crc:
                update_copy_crc_for_continuation(dsc);
                break;
            case core::operation::dif_check:
                update_dif_check_for_continuation(dsc);
                break;
            case core::operation::dif_insert:
                update_dif_insert_for_continuation(dsc);
                break;
            case core::operation::dif_strip:
                update_dif_strip_for_continuation(dsc);
                break;
            case core::operation::dif_update:
                update_dif_update_for_continuation(dsc);
                break;
            case core::operation::cache_flush:
                update_cache_flush_for_continuation(dsc);
                break;
        }
    }

    static void update_mem_move_for_continuation(descriptor& dsc) noexcept
    {
        auto mem_move_dsc    = core::make_view<core::operation::mem_move>(dsc);
        auto mem_move_record = core::make_view<core::operation::mem_move>(core::get_completion_record(dsc));

        if (0 == mem_move_record.result())
        {
            mem_move_dsc.source_address() += mem_move_record.bytes_completed();
            mem_move_dsc.destination_address() += mem_move_record.bytes_completed();
        }

        mem_move_dsc.transfer_size() -= mem_move_record.bytes_completed();
    }

    static void update_fill_for_continuation(descriptor& dsc) noexcept
    {
        auto fill_dsc    = core::make_view<core::operation::fill>(dsc);
        auto fill_record = core::make_view<core::operation::fill>(core::get_completion_record(dsc));

        fill_dsc.transfer_size() -= fill_record.bytes_completed();
        fill_dsc.destination_address() += fill_record.bytes_completed();
    }

    static void update_compare_for_continuation(descriptor& dsc) noexcept
    {
        auto compare_dsc    = core::make_view<core::operation::compare>(dsc);
        auto compare_record = core::make_view<core::operation::compare>(core::get_completion_record(dsc));

        compare_dsc.transfer_size() -= compare_record.bytes_completed();
        compare_dsc.source_1_address() += compare_record.bytes_completed();
        compare_dsc.source_2_address() += compare_record.bytes_completed();
    }

    static void update_compare_pattern_for_continuation(descriptor& dsc) noexcept
    {
        auto compare_pattern_dsc    = core::make_view<core::operation::compare_pattern>(dsc);
        auto compare_pattern_record = core::make_view<core::operation::compare_pattern>(core::get_completion_record(dsc));

        compare_pattern_dsc.transfer_size() -= compare_pattern_record.bytes_completed();
        compare_pattern_dsc.source_address() += compare_pattern_record.bytes_completed();
    }

    static void update_create_delta_for_continuation(descriptor& dsc) noexcept
    {
        auto create_delta_dsc    = core::make_view<core::operation::create_delta>(dsc);
        auto create_delta_record = core::make_view<core::operation::create_delta>(core::get_completion_record(dsc));

        create_delta_dsc.transfer_size() -= create_delta_record.bytes_completed();
        create_delta_dsc.source_1_address() += create_delta_record.bytes_completed();
        create_delta_dsc.source_2_address() += create_delta_record.bytes_completed();
        create_delta_dsc.maximum_delta_record_size() -= create_delta_record.delta_record_size();
        create_delta_dsc.delta_record_address() += create_delta_record.delta_record_size();
    }

    static void update_apply_delta_for_continuation(descriptor& dsc) noexcept
    {
        auto apply_delta_dsc    = core::make_view<core::operation::apply_delta>(dsc);
        auto apply_delta_record = core::make_view<core::operation::apply_delta>(core::get_completion_record(dsc));

        apply_delta_dsc.delta_record_size() -= apply_delta_record.bytes_completed();
        apply_delta_dsc.delta_record_address() += apply_delta_record.bytes_completed();
    }

    static void update_dualcast_for_continuation(descriptor& dsc) noexcept
    {
        auto dualcast_dsc    = core::make_view<core::operation::dualcast>(dsc);
        auto dualcast_record = core::make_view<core::operation::dualcast>(core::get_completion_record(dsc));

        dualcast_dsc.destination_1_address() += dualcast_record.bytes_completed();
        dualcast_dsc.destination_2_address() += dualcast_record.bytes_completed();
        dualcast_dsc.source_address() += dualcast_record.bytes_completed();
        dualcast_dsc.transfer_size() -= dualcast_record.bytes_completed();
    }

    static void update_crc_for_continuation(descriptor& dsc) noexcept
    {
        auto crc_dsc    = core::make_view<core::operation::crc>(dsc);
        auto crc_record = core::make_view<core::operation::crc>(core::get_completion_record(dsc));

        crc_dsc.crc_seed() = crc_record.crc_value();
        crc_dsc.source_address() += crc_record.bytes_completed();
        crc_dsc.transfer_size() -= crc_record.bytes_completed();
    }

    static void update_copy_crc_for_continuation(descriptor& dsc) noexcept
    {
        auto copy_crc_dsc    = core::make_view<core::operation::copy_crc>(dsc);
        auto copy_crc_record = core::make_view<core::operation::copy_crc>(core::get_completion_record(dsc));

        copy_crc_dsc.crc_seed() = copy_crc_record.crc_value();
        copy_crc_dsc.source_address() += copy_crc_record.bytes_completed();
        copy_crc_dsc.destination_address() += copy_crc_record.bytes_completed();
        copy_crc_dsc.transfer_size() -= copy_crc_record.bytes_completed();
    }

    static void update_dif_check_for_continuation(descriptor& dsc) noexcept
    {
        auto dif_check_dsc    = core::make_view<core::operation::dif_check>(dsc);
        auto dif_check_record = core::make_view<core::operation::dif_check>(core::get_completion_record(dsc));

        dif_check_dsc.source_app_tag() = dif_check_record.source_app_tag();
        dif_check_dsc.source_ref_tag() = dif_check_record.source_ref_tag();
        dif_check_dsc.source_address() += dif_check_record.bytes_completed();
        dif_check_dsc.transfer_size() -= dif_check_record.bytes_completed();
    }

    static void update_dif_insert_for_continuation(descriptor& dsc) noexcept
    {
        auto dif_insert_dsc    = core::make_view<core::operation::dif_insert>(dsc);
        auto dif_insert_record = core::make_view<core::operation::dif_insert>(core::get_completion_record(dsc));

        dif_insert_dsc.destination_app_tag() = dif_insert_record.destination_app_tag();
        dif_insert_dsc.destination_ref_tag() = dif_insert_record.destination_ref_tag();
        dif_insert_dsc.source_address() += dif_insert_record.bytes_completed();
        dif_insert_dsc.destination_address() += dif_insert_record.bytes_completed();
        dif_insert_dsc.transfer_size() -= dif_insert_record.bytes_completed();
    }

    static void update_dif_strip_for_continuation(descriptor& dsc) noexcept
    {
        auto dif_strip_dsc    = core::make_view<core::operation::dif_strip>(dsc);
        auto dif_strip_record = core::make_view<core::operation::dif_strip>(core::get_completion_record(dsc));

        dif_strip_dsc.source_app_tag() = dif_strip_record.source_app_tag();
        dif_strip_dsc.source_ref_tag() = dif_strip_record.source_ref_tag();
        dif_strip_dsc.source_address() += dif_strip_record.bytes_completed();
        dif_strip_dsc.destination_address() += dif_strip_record.bytes_completed();
        dif_strip_dsc.transfer_size() -= dif_strip_record.bytes_completed();
    }

    static void update_dif_update_for_continuation(descriptor& dsc) noexcept
    {
        auto dif_update_dsc    = core::make_view<core::operation::dif_update>(dsc);
        auto dif_update_record = core::make_view<core::operation::dif_update>(core::get_completion_record(dsc));

        dif_update_dsc.source_app_tag()      = dif_update_record.source_app_tag();
        dif_update_dsc.source_app_tag()      = dif_update_record.source_app_tag();
        dif_update_dsc.destination_app_tag() = dif_update_record.destination_app_tag();
        dif_update_dsc.destination_ref_tag() = dif_update_record.destination_ref_tag();
        dif_update_dsc.source_ref_tag()      = dif_update_record.source_ref_tag();
        dif_update_dsc.source_address() += dif_update_record.bytes_completed();
        dif_update_dsc.destination_address() += dif_update_record.bytes_completed();
        dif_update_dsc.transfer_size() -= dif_update_record.bytes_completed();
    }

    static void update_cache_flush_for_continuation(descriptor& dsc) noexcept
    {
        auto cache_flush_dsc    = core::make_view<core::operation::cache_flush>(dsc);
        auto cache_flush_record = core::make_view<core::operation::cache_flush>(core::get_completion_record(dsc));

        cache_flush_dsc.destination_address() += cache_flush_record.bytes_completed();
        cache_flush_dsc.transfer_size() -= cache_flush_record.bytes_completed();
    }
}  // namespace dml::detail::ml

#endif  // DML_ML_OWN_PARTIAL_COMPLETION_HPP
