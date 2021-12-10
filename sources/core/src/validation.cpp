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
#include <core/validation.hpp>
#include <dml/detail/common/status.hpp>

#include "utils.hpp"

namespace dml::core
{
    static constexpr uint32_t dif_block_sizes[4] = { 512u, 520u, 4096u, 4104u };

    static dml::detail::validation_status validate(nop_descriptor nop) noexcept;

    static dml::detail::validation_status validate(batch_descriptor batch) noexcept;

    static dml::detail::validation_status validate(drain_descriptor drain) noexcept;

    static dml::detail::validation_status validate(mem_move_descriptor mem_move) noexcept;

    static dml::detail::validation_status validate(fill_descriptor fill) noexcept;

    static dml::detail::validation_status validate(compare_descriptor compare) noexcept;

    static dml::detail::validation_status validate(compare_pattern_descriptor compare_pattern) noexcept;

    static dml::detail::validation_status validate(create_delta_descriptor create_delta) noexcept;

    static dml::detail::validation_status validate(apply_delta_descriptor apply_delta) noexcept;

    static dml::detail::validation_status validate(dualcast_descriptor dualcast) noexcept;

    static dml::detail::validation_status validate(crc_descriptor crc) noexcept;

    static dml::detail::validation_status validate(copy_crc_descriptor copy_crc) noexcept;

    static dml::detail::validation_status validate(dif_check_descriptor dif_check) noexcept;

    static dml::detail::validation_status validate(dif_insert_descriptor dif_insert) noexcept;

    static dml::detail::validation_status validate(dif_strip_descriptor dif_strip) noexcept;

    static dml::detail::validation_status validate(dif_update_descriptor dif_update) noexcept;

    static dml::detail::validation_status validate(cache_flush_descriptor cache_flush) noexcept;

    dml::detail::validation_status validate(descriptor &dsc) noexcept
    {
        auto view = any_descriptor(dsc);

        switch (static_cast<operation>(view.operation()))
        {
            case operation::nop:
                return validate(nop_descriptor(dsc));
            case operation::batch:
                return validate(batch_descriptor(dsc));
            case operation::drain:
                return validate(drain_descriptor(dsc));
            case operation::memory_move:
                return validate(mem_move_descriptor(dsc));
            case operation::fill:
                return validate(fill_descriptor(dsc));
            case operation::compare:
                return validate(compare_descriptor(dsc));
            case operation::compare_pattern:
                return validate(compare_pattern_descriptor(dsc));
            case operation::create_delta:
                return validate(create_delta_descriptor(dsc));
            case operation::apply_delta:
                return validate(apply_delta_descriptor(dsc));
            case operation::dualcast:
                return validate(dualcast_descriptor(dsc));
            case operation::crc:
                return validate(crc_descriptor(dsc));
            case operation::copy_crc:
                return validate(copy_crc_descriptor(dsc));
            case operation::dif_check:
                return validate(dif_check_descriptor(dsc));
            case operation::dif_insert:
                return validate(dif_insert_descriptor(dsc));
            case operation::dif_strip:
                return validate(dif_strip_descriptor(dsc));
            case operation::dif_update:
                return validate(dif_update_descriptor(dsc));
            case operation::cache_flush:
                return validate(cache_flush_descriptor(dsc));
            default:
                return dml::detail::validation_status::unsupported_operation;
        }
    }

    static dml::detail::validation_status validate(nop_descriptor nop) noexcept
    {
        static_cast<void>(nop);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(drain_descriptor drain) noexcept
    {
        static_cast<void>(drain);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(mem_move_descriptor mem_move) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(mem_move.source_address(), mem_move.destination_address()),
                         dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(mem_move.transfer_size()), dml::detail::validation_status::null_size);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(fill_descriptor fill) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(fill.destination_address()), dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(fill.transfer_size()), dml::detail::validation_status::null_size);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(compare_descriptor compare) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(compare.source_1_address(), compare.source_2_address()),
                         dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(compare.transfer_size()), dml::detail::validation_status::null_size);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(compare_pattern_descriptor compare_pattern) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(compare_pattern.source_address()), dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(compare_pattern.transfer_size()), dml::detail::validation_status::null_size);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(create_delta_descriptor create_delta) noexcept
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

    static dml::detail::validation_status validate(apply_delta_descriptor apply_delta) noexcept
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

    static dml::detail::validation_status validate(dualcast_descriptor dualcast) noexcept
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

    static dml::detail::validation_status validate(crc_descriptor crc) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(crc.source_address()), dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(crc.transfer_size()), dml::detail::validation_status::null_size);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(copy_crc_descriptor copy_crc) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(copy_crc.source_address(), copy_crc.destination_address()),
                         dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(copy_crc.transfer_size()), dml::detail::validation_status::null_size);

        RETURN_STATUS_IF(overlaps(copy_crc.source_address(), copy_crc.destination_address(), copy_crc.transfer_size()),
                         dml::detail::validation_status::overlapping);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(cache_flush_descriptor cache_flush) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(cache_flush.destination_address()), dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(cache_flush.transfer_size()), dml::detail::validation_status::null_size);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(dif_check_descriptor dif_check) noexcept
    {
        const auto block_size = dif_block_sizes[dif_check.dif_flags() & 0b11];

        RETURN_STATUS_IF(any_equal_zero(dif_check.source_address()), dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(any_equal_zero(dif_check.transfer_size()), dml::detail::validation_status::null_size);
        RETURN_STATUS_IF(dif_check.transfer_size() % (block_size + sizeof(uint64_t)) != 0, dml::detail::validation_status::wrong_dif_size);

        return dml::detail::validation_status::success;
    }

    static dml::detail::validation_status validate(dif_insert_descriptor dif_insert) noexcept
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

    static dml::detail::validation_status validate(dif_strip_descriptor dif_strip) noexcept
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

    static dml::detail::validation_status validate(dif_update_descriptor dif_update) noexcept
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

    static dml::detail::validation_status validate(batch_descriptor batch) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(batch.descriptor_list_address()), dml::detail::validation_status::null_address);
        RETURN_STATUS_IF(batch.descriptors_count() < 4, dml::detail::validation_status::wrong_batch_size);

        return dml::detail::validation_status::success;
    }

}  // namespace dml::core
