/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_ML_OWN_ACCUMULATE_RECORDS_HPP
#define DML_ML_OWN_ACCUMULATE_RECORDS_HPP

#include <core/descriptor_views.hpp>
#include <dml/detail/ml/impl/make_descriptor.hpp>

namespace dml::detail::ml
{
    static void accumulate_records_mem_move(descriptor& dsc, const completion_record& prev_record) noexcept;

    static void accumulate_records_fill(descriptor& dsc, const completion_record& prev_record) noexcept;

    static void accumulate_records_compare(descriptor& dsc, const completion_record& prev_record) noexcept;

    static void accumulate_records_compare_pattern(descriptor& dsc, const completion_record& prev_record) noexcept;

    static void accumulate_records_create_delta(descriptor& dsc, const completion_record& prev_record) noexcept;

    static void accumulate_records_apply_delta(descriptor& dsc, const completion_record& prev_record) noexcept;

    static void accumulate_records_dualcast(descriptor& dsc, const completion_record& prev_record) noexcept;

    static void accumulate_records_crc(descriptor& dsc, const completion_record& prev_record) noexcept;

    static void accumulate_records_copy_crc(descriptor& dsc, const completion_record& prev_record) noexcept;

    static void accumulate_records_dif_check(descriptor& dsc, const completion_record& prev_record) noexcept;

    static void accumulate_records_dif_insert(descriptor& dsc, const completion_record& prev_record) noexcept;

    static void accumulate_records_dif_strip(descriptor& dsc, const completion_record& prev_record) noexcept;

    static void accumulate_records_dif_update(descriptor& dsc, const completion_record& prev_record) noexcept;

    static void accumulate_records_cache_flush(descriptor& dsc, const completion_record& prev_record) noexcept;

    static void accumulate_records(descriptor& dsc, const completion_record& prev_record) noexcept
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
                accumulate_records_mem_move(dsc, prev_record);
                break;
            case core::operation::fill:
                accumulate_records_fill(dsc, prev_record);
                break;
            case core::operation::compare:
                accumulate_records_compare(dsc, prev_record);
                break;
            case core::operation::compare_pattern:
                accumulate_records_compare_pattern(dsc, prev_record);
                break;
            case core::operation::create_delta:
                accumulate_records_create_delta(dsc, prev_record);
                break;
            case core::operation::apply_delta:
                accumulate_records_apply_delta(dsc, prev_record);
                break;
            case core::operation::dualcast:
                accumulate_records_dualcast(dsc, prev_record);
                break;
            case core::operation::crc:
                accumulate_records_crc(dsc, prev_record);
                break;
            case core::operation::copy_crc:
                accumulate_records_copy_crc(dsc, prev_record);
                break;
            case core::operation::dif_check:
                accumulate_records_dif_check(dsc, prev_record);
                break;
            case core::operation::dif_insert:
                accumulate_records_dif_insert(dsc, prev_record);
                break;
            case core::operation::dif_strip:
                accumulate_records_dif_strip(dsc, prev_record);
                break;
            case core::operation::dif_update:
                accumulate_records_dif_update(dsc, prev_record);
                break;
            case core::operation::cache_flush:
                accumulate_records_cache_flush(dsc, prev_record);
                break;
        }
    }

    static void accumulate_records_mem_move(descriptor& dsc, const completion_record& prev_record) noexcept
    {
        auto mem_move_record = core::make_view<core::operation::mem_move>(core::get_completion_record(dsc));
        auto mem_move_prev_record = core::make_view<core::operation::mem_move>(prev_record);

        mem_move_record.bytes_completed() += mem_move_prev_record.bytes_completed();
    }

    static void accumulate_records_fill(descriptor& dsc, const completion_record& prev_record) noexcept
    {
        auto fill_record = core::make_view<core::operation::fill>(core::get_completion_record(dsc));
        auto fill_prev_record = core::make_view<core::operation::fill>(prev_record);

        fill_record.bytes_completed() += fill_prev_record.bytes_completed();
    }

    static void accumulate_records_compare(descriptor& dsc, const completion_record& prev_record) noexcept
    {
        auto compare_record = core::make_view<core::operation::compare>(core::get_completion_record(dsc));
        auto compare_prev_record = core::make_view<core::operation::compare>(prev_record);

        compare_record.bytes_completed() = compare_prev_record.bytes_completed();
    }

    static void accumulate_records_compare_pattern(descriptor& dsc, const completion_record& prev_record) noexcept
    {
        auto compare_pattern_record = core::make_view<core::operation::compare_pattern>(core::get_completion_record(dsc));
        auto compare_pattern_prev_record = core::make_view<core::operation::compare_pattern>(prev_record);

        compare_pattern_record.bytes_completed() += compare_pattern_prev_record.bytes_completed();
    }

    static void accumulate_records_create_delta(descriptor& dsc, const completion_record& prev_record) noexcept
    {
        auto create_delta_record = core::make_view<core::operation::create_delta>(core::get_completion_record(dsc));
        auto create_delta_prev_record = core::make_view<core::operation::create_delta>(prev_record);

        create_delta_record.bytes_completed() += create_delta_prev_record.bytes_completed();
        create_delta_record.delta_record_size() += create_delta_prev_record.delta_record_size();
    }

    static void accumulate_records_apply_delta(descriptor& dsc, const completion_record& prev_record) noexcept
    {
        auto apply_delta_record = core::make_view<core::operation::apply_delta>(core::get_completion_record(dsc));
        auto apply_delta_prev_record = core::make_view<core::operation::apply_delta>(prev_record);

        apply_delta_record.bytes_completed() += apply_delta_prev_record.bytes_completed();
    }

    static void accumulate_records_dualcast(descriptor& dsc, const completion_record& prev_record) noexcept
    {
        auto dualcast_record = core::make_view<core::operation::dualcast>(core::get_completion_record(dsc));
        auto dualcast_prev_record = core::make_view<core::operation::dualcast>(prev_record);

        dualcast_record.bytes_completed() += dualcast_prev_record.bytes_completed();
    }

    static void accumulate_records_crc(descriptor& dsc, const completion_record& prev_record) noexcept
    {
        auto crc_record = core::make_view<core::operation::crc>(core::get_completion_record(dsc));
        auto crc_prev_record = core::make_view<core::operation::crc>(prev_record);

        crc_record.bytes_completed() += crc_prev_record.bytes_completed();
    }

    static void accumulate_records_copy_crc(descriptor& dsc, const completion_record& prev_record) noexcept
    {
        auto copy_crc_record = core::make_view<core::operation::copy_crc>(core::get_completion_record(dsc));
        auto copy_crc_prev_record = core::make_view<core::operation::copy_crc>(prev_record);

        copy_crc_record.bytes_completed() += copy_crc_prev_record.bytes_completed();
    }

    static void accumulate_records_dif_check(descriptor& dsc, const completion_record& prev_record) noexcept
    {
        auto dif_check_record = core::make_view<core::operation::dif_check>(core::get_completion_record(dsc));
        auto dif_check_prev_record = core::make_view<core::operation::dif_check>(prev_record);

        dif_check_record.bytes_completed() += dif_check_prev_record.bytes_completed();
    }

    static void accumulate_records_dif_insert(descriptor& dsc, const completion_record& prev_record) noexcept
    {
        auto dif_insert_record = core::make_view<core::operation::dif_insert>(core::get_completion_record(dsc));
        auto dif_insert_prev_record = core::make_view<core::operation::dif_insert>(prev_record);

        dif_insert_record.bytes_completed() += dif_insert_prev_record.bytes_completed();
    }

    static void accumulate_records_dif_strip(descriptor& dsc, const completion_record& prev_record) noexcept
    {
        auto dif_strip_record = core::make_view<core::operation::dif_strip>(core::get_completion_record(dsc));
        auto dif_strip_prev_record = core::make_view<core::operation::dif_strip>(prev_record);

        dif_strip_record.bytes_completed() += dif_strip_prev_record.bytes_completed();
    }

    static void accumulate_records_dif_update(descriptor& dsc, const completion_record& prev_record) noexcept
    {
        auto dif_update_record = core::make_view<core::operation::dif_update>(core::get_completion_record(dsc));
        auto dif_update_prev_record = core::make_view<core::operation::dif_update>(prev_record);

        dif_update_record.bytes_completed() += dif_update_prev_record.bytes_completed();
    }

    static void accumulate_records_cache_flush(descriptor& dsc, const completion_record& prev_record) noexcept
    {
        auto cache_flush_record = core::make_view<core::operation::cache_flush>(core::get_completion_record(dsc));
        auto cache_flush_prev_record = core::make_view<core::operation::cache_flush>(prev_record);

        cache_flush_record.bytes_completed() += cache_flush_prev_record.bytes_completed();
    }
}  // namespace dml::detail::ml

#endif  // DML_ML_OWN_ACCUMULATE_RECORDS_HPP
