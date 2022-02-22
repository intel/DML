/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <core/descriptor_views.hpp>
#include <core/operations.hpp>
#include <core/validation.hpp>
#include <core/view.hpp>
#include <dml/detail/common/status.hpp>

#include "utils.hpp"

namespace dml::core
{
    static constexpr uint32_t dif_block_sizes[4] = { 512u, 520u, 4096u, 4104u };

    static dml::detail::validation_status validate(const_view<descriptor, operation::nop> nop) noexcept;

    static dml::detail::validation_status validate(const_view<descriptor, operation::batch> batch) noexcept;

    static dml::detail::validation_status validate(const_view<descriptor, operation::drain> drain) noexcept;

    static dml::detail::validation_status validate(const_view<descriptor, operation::mem_move> mem_move) noexcept;

    static dml::detail::validation_status validate(const_view<descriptor, operation::fill> fill) noexcept;

    static dml::detail::validation_status validate(const_view<descriptor, operation::compare> compare) noexcept;

    static dml::detail::validation_status validate(const_view<descriptor, operation::compare_pattern> compare_pattern) noexcept;

    static dml::detail::validation_status validate(const_view<descriptor, operation::create_delta> create_delta) noexcept;

    static dml::detail::validation_status validate(const_view<descriptor, operation::apply_delta> apply_delta) noexcept;

    static dml::detail::validation_status validate(const_view<descriptor, operation::dualcast> dualcast) noexcept;

    static dml::detail::validation_status validate(const_view<descriptor, operation::crc> crc) noexcept;

    static dml::detail::validation_status validate(const_view<descriptor, operation::copy_crc> copy_crc) noexcept;

    static dml::detail::validation_status validate(const_view<descriptor, operation::dif_check> dif_check) noexcept;

    static dml::detail::validation_status validate(const_view<descriptor, operation::dif_insert> dif_insert) noexcept;

    static dml::detail::validation_status validate(const_view<descriptor, operation::dif_strip> dif_strip) noexcept;

    static dml::detail::validation_status validate(const_view<descriptor, operation::dif_update> dif_update) noexcept;

    static dml::detail::validation_status validate(const_view<descriptor, operation::cache_flush> cache_flush) noexcept;

    dml::detail::validation_status validate(const descriptor &dsc) noexcept
    {
        switch (static_cast<operation>(any_descriptor(dsc).operation()))
        {
            case operation::nop:
                return validate(make_view<operation::nop>(dsc));
            case operation::batch:
                return validate(make_view<operation::batch>(dsc));
            case operation::drain:
                return validate(make_view<operation::drain>(dsc));
            case operation::mem_move:
                return validate(make_view<operation::mem_move>(dsc));
            case operation::fill:
                return validate(make_view<operation::fill>(dsc));
            case operation::compare:
                return validate(make_view<operation::compare>(dsc));
            case operation::compare_pattern:
                return validate(make_view<operation::compare_pattern>(dsc));
            case operation::create_delta:
                return validate(make_view<operation::create_delta>(dsc));
            case operation::apply_delta:
                return validate(make_view<operation::apply_delta>(dsc));
            case operation::dualcast:
                return validate(make_view<operation::dualcast>(dsc));
            case operation::crc:
                return validate(make_view<operation::crc>(dsc));
            case operation::copy_crc:
                return validate(make_view<operation::copy_crc>(dsc));
            case operation::dif_check:
                return validate(make_view<operation::dif_check>(dsc));
            case operation::dif_insert:
                return validate(make_view<operation::dif_insert>(dsc));
            case operation::dif_strip:
                return validate(make_view<operation::dif_strip>(dsc));
            case operation::dif_update:
                return validate(make_view<operation::dif_update>(dsc));
            case operation::cache_flush:
                return validate(make_view<operation::cache_flush>(dsc));
            default:
                return dml::detail::validation_status::unsupported_operation;
        }
    }

    static dml::detail::validation_status validate(const_view<descriptor, operation::nop> nop) noexcept
    {
        static_cast<void>(nop);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(const_view<descriptor, operation::drain> drain) noexcept
    {
        static_cast<void>(drain);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(const_view<descriptor, operation::mem_move> mem_move) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(mem_move.source_address(), mem_move.destination_address()),
                         dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(mem_move.transfer_size()), dml::detail::validation_status::null_size);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(const_view<descriptor, operation::fill> fill) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(fill.destination_address()), dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(fill.transfer_size()), dml::detail::validation_status::null_size);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(const_view<descriptor, operation::compare> compare) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(compare.source_1_address(), compare.source_2_address()),
                         dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(compare.transfer_size()), dml::detail::validation_status::null_size);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(const_view<descriptor, operation::compare_pattern> compare_pattern) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(compare_pattern.source_address()), dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(compare_pattern.transfer_size()), dml::detail::validation_status::null_size);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(const_view<descriptor, operation::create_delta> create_delta) noexcept
    {
        constexpr auto max_size = 0x80000;

        RETURN_STATUS_IF(
            any_equal_zero(create_delta.source_1_address(), create_delta.source_2_address(), create_delta.delta_record_address()),
            dml::detail::validation_status::null_address);

        RETURN_STATUS_IF(any_equal_zero(create_delta.transfer_size(), create_delta.maximum_delta_record_size()),
                         dml::detail::validation_status::null_size);

        RETURN_STATUS_IF(
            any_misaligned<8u>(create_delta.source_1_address(), create_delta.source_2_address(), create_delta.delta_record_address()),
            dml::detail::validation_status::misalignment);

        RETURN_STATUS_IF(create_delta.transfer_size() % 8 != 0, dml::detail::validation_status::wrong_size);

        RETURN_STATUS_IF(create_delta.transfer_size() > max_size, dml::detail::validation_status::large_size);

        RETURN_STATUS_IF(create_delta.maximum_delta_record_size() % 10 != 0 || create_delta.maximum_delta_record_size() < 80,
                         dml::detail::validation_status::wrong_delta_size);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(const_view<descriptor, operation::apply_delta> apply_delta) noexcept
    {
        constexpr auto max_size = 0x80000;

        RETURN_STATUS_IF(any_equal_zero(apply_delta.destination_address(), apply_delta.delta_record_address()),
                         dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(apply_delta.transfer_size(), apply_delta.delta_record_size()),
                         dml::detail::validation_status::null_size);

        RETURN_STATUS_IF(overlaps(apply_delta.delta_record_address(),
                                  apply_delta.delta_record_size(),
                                  apply_delta.destination_address(),
                                  apply_delta.transfer_size()),
                         dml::detail::validation_status::overlapping);

        RETURN_STATUS_IF(any_misaligned<8u>(apply_delta.destination_address(), apply_delta.delta_record_address()),
                         dml::detail::validation_status::misalignment);

        RETURN_STATUS_IF(apply_delta.transfer_size() % 8 != 0, dml::detail::validation_status::wrong_size);

        RETURN_STATUS_IF(apply_delta.transfer_size() > max_size, dml::detail::validation_status::large_size);

        RETURN_STATUS_IF(apply_delta.delta_record_size() % 10 != 0, dml::detail::validation_status::wrong_delta_size);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(const_view<descriptor, operation::dualcast> dualcast) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(dualcast.source_address(), dualcast.destination_1_address(), dualcast.destination_2_address()),
                         dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(dualcast.transfer_size()), dml::detail::validation_status::null_size);

        RETURN_STATUS_IF((dualcast.destination_1_address() & 0xFFFu) != (dualcast.destination_2_address() & 0xFFFu),
                         dml::detail::validation_status::wrong_dualcast_address);

        RETURN_STATUS_IF(overlaps(dualcast.source_address(), dualcast.destination_1_address(), dualcast.transfer_size()),
                         dml::detail::validation_status::overlapping);

        RETURN_STATUS_IF(overlaps(dualcast.source_address(), dualcast.destination_2_address(), dualcast.transfer_size()),
                         dml::detail::validation_status::overlapping);

        RETURN_STATUS_IF(overlaps(dualcast.destination_1_address(), dualcast.destination_2_address(), dualcast.transfer_size()),
                         dml::detail::validation_status::overlapping);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(const_view<descriptor, operation::crc> crc) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(crc.source_address()), dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(crc.transfer_size()), dml::detail::validation_status::null_size);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(const_view<descriptor, operation::copy_crc> copy_crc) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(copy_crc.source_address(), copy_crc.destination_address()),
                         dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(copy_crc.transfer_size()), dml::detail::validation_status::null_size);

        RETURN_STATUS_IF(overlaps(copy_crc.source_address(), copy_crc.destination_address(), copy_crc.transfer_size()),
                         dml::detail::validation_status::overlapping);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(const_view<descriptor, operation::cache_flush> cache_flush) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(cache_flush.destination_address()), dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(cache_flush.transfer_size()), dml::detail::validation_status::null_size);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(const_view<descriptor, operation::dif_check> dif_check) noexcept
    {
        const auto block_size = dif_block_sizes[dif_check.dif_flags() & 0b11];

        RETURN_STATUS_IF(any_equal_zero(dif_check.source_address()), dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(dif_check.transfer_size()), dml::detail::validation_status::null_size);
        RETURN_STATUS_IF(dif_check.transfer_size() % (block_size + sizeof(uint64_t)) != 0, dml::detail::validation_status::wrong_dif_size);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(const_view<descriptor, operation::dif_insert> dif_insert) noexcept
    {
        const auto block_size = dif_block_sizes[dif_insert.dif_flags() & 0b11];

        RETURN_STATUS_IF(any_equal_zero(dif_insert.source_address(), dif_insert.destination_address()),
                         dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(dif_insert.transfer_size()), dml::detail::validation_status::null_size);
        RETURN_STATUS_IF(dif_insert.transfer_size() % block_size != 0, dml::detail::validation_status::wrong_dif_size);

        const auto src_size = dif_insert.transfer_size();
        const auto dst_size = src_size + ((src_size / block_size) * 8);
        RETURN_STATUS_IF(overlaps(dif_insert.source_address(), src_size, dif_insert.destination_address(), dst_size),
                         dml::detail::validation_status::overlapping);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(const_view<descriptor, operation::dif_strip> dif_strip) noexcept
    {
        const auto block_size = dif_block_sizes[dif_strip.dif_flags() & 0b11];

        RETURN_STATUS_IF(any_equal_zero(dif_strip.source_address(), dif_strip.destination_address()),
                         dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(dif_strip.transfer_size()), dml::detail::validation_status::null_size);
        RETURN_STATUS_IF(dif_strip.transfer_size() % (block_size + sizeof(uint64_t)) != 0, dml::detail::validation_status::wrong_dif_size);

        const auto src_size = dif_strip.transfer_size();
        const auto dst_size = src_size - ((src_size / block_size) * 8);
        RETURN_STATUS_IF(overlaps(dif_strip.source_address(), src_size, dif_strip.destination_address(), dst_size),
                         dml::detail::validation_status::overlapping);

        // Hardware bug workaround
        if (dif_strip.destination_address() < dif_strip.source_address())
        {
            if ((dif_strip.destination_address() + dst_size) <= dif_strip.source_address() &&
                dif_strip.source_address() <= (dif_strip.destination_address() + src_size))
            {
                return dml::detail::validation_status::dif_strip_adjacent;
            }
        }

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(const_view<descriptor, operation::dif_update> dif_update) noexcept
    {
        const auto block_size = dif_block_sizes[dif_update.dif_flags() & 0b11];

        RETURN_STATUS_IF(any_equal_zero(dif_update.source_address(), dif_update.destination_address()),
                         dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(dif_update.transfer_size()), dml::detail::validation_status::null_size);
        RETURN_STATUS_IF(dif_update.transfer_size() % (block_size + sizeof(uint64_t)) != 0, dml::detail::validation_status::wrong_dif_size);
        RETURN_STATUS_IF(overlaps(dif_update.source_address(), dif_update.destination_address(), dif_update.transfer_size()),
                         dml::detail::validation_status::overlapping);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(const_view<descriptor, operation::batch> batch) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(batch.descriptor_list_address()), dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(batch.descriptors_count() < 4, dml::detail::validation_status::wrong_batch_size);

        return dml::detail::validation_status::success;
    }

}  // namespace dml::core
