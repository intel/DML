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

#include <dml/cpp/middle_layer/descriptor_views.hpp>
#include <dml/cpp/middle_layer/validation.hpp>
#include <dml/cpp/middle_layer/values.hpp>

#include "utils.hpp"

namespace dml::ml
{
    static constexpr uint32_t dif_block_sizes[4] = { 512u, 520u, 4096u, 4104u };

    [[nodiscard]] static validation_status validate(views::nop_descriptor nop) noexcept;

    [[nodiscard]] static validation_status validate(views::batch_descriptor batch) noexcept;

    [[nodiscard]] static validation_status validate(views::drain_descriptor drain) noexcept;

    [[nodiscard]] static validation_status validate(views::mem_move_descriptor mem_move) noexcept;

    [[nodiscard]] static validation_status validate(views::fill_descriptor fill) noexcept;

    [[nodiscard]] static validation_status validate(views::compare_descriptor compare) noexcept;

    [[nodiscard]] static validation_status validate(views::compare_pattern_descriptor compare_pattern) noexcept;

    [[nodiscard]] static validation_status validate(views::create_delta_descriptor create_delta) noexcept;

    [[nodiscard]] static validation_status validate(views::apply_delta_descriptor apply_delta) noexcept;

    [[nodiscard]] static validation_status validate(views::dualcast_descriptor dualcast) noexcept;

    [[nodiscard]] static validation_status validate(views::crc_descriptor crc) noexcept;

    [[nodiscard]] static validation_status validate(views::copy_crc_descriptor copy_crc) noexcept;

    [[nodiscard]] static validation_status validate(views::dif_check_descriptor dif_check) noexcept;

    [[nodiscard]] static validation_status validate(views::dif_insert_descriptor dif_insert) noexcept;

    [[nodiscard]] static validation_status validate(views::dif_strip_descriptor dif_strip) noexcept;

    [[nodiscard]] static validation_status validate(views::dif_update_descriptor dif_update) noexcept;

    [[nodiscard]] static validation_status validate(views::cache_flush_descriptor cache_flush) noexcept;

    validation_status validate(descriptor &dsc) noexcept
    {
        auto view = views::any_descriptor(dsc);

        switch (static_cast<operation>(view.operation()))
        {
            case operation::nop:
                return validate(views::nop_descriptor(dsc));
            case operation::batch:
                return validate(views::batch_descriptor(dsc));
            case operation::drain:
                return validate(views::drain_descriptor(dsc));
            case operation::memory_move:
                return validate(views::mem_move_descriptor(dsc));
            case operation::fill:
                return validate(views::fill_descriptor(dsc));
            case operation::compare:
                return validate(views::compare_descriptor(dsc));
            case operation::compare_pattern:
                return validate(views::compare_pattern_descriptor(dsc));
            case operation::create_delta:
                return validate(views::create_delta_descriptor(dsc));
            case operation::apply_delta:
                return validate(views::apply_delta_descriptor(dsc));
            case operation::dualcast:
                return validate(views::dualcast_descriptor(dsc));
            case operation::crc:
                return validate(views::crc_descriptor(dsc));
            case operation::copy_crc:
                return validate(views::copy_crc_descriptor(dsc));
            case operation::dif_check:
                return validate(views::dif_check_descriptor(dsc));
            case operation::dif_insert:
                return validate(views::dif_insert_descriptor(dsc));
            case operation::dif_strip:
                return validate(views::dif_strip_descriptor(dsc));
            case operation::dif_update:
                return validate(views::dif_update_descriptor(dsc));
            case operation::cache_flush:
                return validate(views::cache_flush_descriptor(dsc));
            default:
                return validation_status::unsupported_operation;
        }
    }

    [[nodiscard]] static validation_status validate(views::nop_descriptor nop) noexcept
    {
        static_cast<void>(nop);

        return validation_status::success;
    }

    [[nodiscard]] static validation_status validate(views::drain_descriptor drain) noexcept
    {
        static_cast<void>(drain);

        return validation_status::success;
    }

    [[nodiscard]] static validation_status validate(views::mem_move_descriptor mem_move) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(mem_move.source_address(), mem_move.destination_address()), validation_status::address_is_null);
        RETURN_STATUS_IF(any_equal_zero(mem_move.transfer_size()), validation_status::size_is_null);

        return validation_status::success;
    }

    [[nodiscard]] static validation_status validate(views::fill_descriptor fill) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(fill.destination_address()), validation_status::address_is_null);
        RETURN_STATUS_IF(any_equal_zero(fill.transfer_size()), validation_status::size_is_null);

        return validation_status::success;
    }

    [[nodiscard]] static validation_status validate(views::compare_descriptor compare) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(compare.source_1_address(), compare.source_2_address()), validation_status::address_is_null);
        RETURN_STATUS_IF(any_equal_zero(compare.transfer_size()), validation_status::size_is_null);

        return validation_status::success;
    }

    [[nodiscard]] static validation_status validate(views::compare_pattern_descriptor compare_pattern) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(compare_pattern.source_address()), validation_status::address_is_null);
        RETURN_STATUS_IF(any_equal_zero(compare_pattern.transfer_size()), validation_status::size_is_null);

        return validation_status::success;
    }

    [[nodiscard]] static validation_status validate(views::create_delta_descriptor create_delta) noexcept
    {
        constexpr auto max_size = 0x80000;

        RETURN_STATUS_IF(
            any_equal_zero(create_delta.source_1_address(), create_delta.source_2_address(), create_delta.delta_record_address()),
            validation_status::address_is_null);

        RETURN_STATUS_IF(any_equal_zero(create_delta.transfer_size(), create_delta.maximum_delta_record_size()),
                         validation_status::size_is_null);

        RETURN_STATUS_IF(
            any_misaligned<8u>(create_delta.source_1_address(), create_delta.source_2_address(), create_delta.delta_record_address()),
            validation_status::address_is_misaligned);

        RETURN_STATUS_IF(create_delta.transfer_size() % 8 != 0, validation_status::delta_input_size_is_wrong);

        RETURN_STATUS_IF(create_delta.transfer_size() > max_size, validation_status::delta_input_size_overflow);

        RETURN_STATUS_IF(create_delta.maximum_delta_record_size() % 10 != 0 || create_delta.maximum_delta_record_size() < 80,
                         validation_status::delta_record_size_is_wrong);

        return validation_status::success;
    }

    [[nodiscard]] static validation_status validate(views::apply_delta_descriptor apply_delta) noexcept
    {
        constexpr auto max_size = 0x80000;

        RETURN_STATUS_IF(any_equal_zero(apply_delta.destination_address(), apply_delta.delta_record_address()),
                         validation_status::address_is_null);
        RETURN_STATUS_IF(any_equal_zero(apply_delta.transfer_size(), apply_delta.delta_record_size()), validation_status::size_is_null);

        RETURN_STATUS_IF(overlaps(apply_delta.delta_record_address(),
                                  apply_delta.delta_record_size(),
                                  apply_delta.destination_address(),
                                  apply_delta.transfer_size()),
                         validation_status::buffers_overlap);

        RETURN_STATUS_IF(any_misaligned<8u>(apply_delta.destination_address(), apply_delta.delta_record_address()),
                         validation_status::address_is_misaligned);

        RETURN_STATUS_IF(apply_delta.transfer_size() % 8 != 0, validation_status::delta_input_size_is_wrong);

        RETURN_STATUS_IF(apply_delta.transfer_size() > max_size, validation_status::delta_input_size_overflow);

        RETURN_STATUS_IF(apply_delta.delta_record_size() % 10 != 0, validation_status::delta_record_size_is_wrong);

        return validation_status::success;
    }

    [[nodiscard]] static validation_status validate(views::dualcast_descriptor dualcast) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(dualcast.source_address(), dualcast.destination_1_address(), dualcast.destination_2_address()),
                         validation_status::address_is_null);
        RETURN_STATUS_IF(any_equal_zero(dualcast.transfer_size()), validation_status::size_is_null);

        RETURN_STATUS_IF((dualcast.destination_1_address() & 0xFFFu) != (dualcast.destination_2_address() & 0xFFFu),
                         validation_status::dualcast_address_is_wrong);

        RETURN_STATUS_IF(overlaps(dualcast.source_address(), dualcast.destination_1_address(), dualcast.transfer_size()),
                         validation_status::buffers_overlap);

        RETURN_STATUS_IF(overlaps(dualcast.source_address(), dualcast.destination_2_address(), dualcast.transfer_size()),
                         validation_status::buffers_overlap);

        RETURN_STATUS_IF(overlaps(dualcast.destination_1_address(), dualcast.destination_2_address(), dualcast.transfer_size()),
                         validation_status::buffers_overlap);

        return validation_status::success;
    }

    [[nodiscard]] static validation_status validate(views::crc_descriptor crc) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(crc.source_address()), validation_status::address_is_null);
        RETURN_STATUS_IF(any_equal_zero(crc.transfer_size()), validation_status::size_is_null);

        return validation_status::success;
    }

    [[nodiscard]] static validation_status validate(views::copy_crc_descriptor copy_crc) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(copy_crc.source_address(), copy_crc.destination_address()), validation_status::address_is_null);
        RETURN_STATUS_IF(any_equal_zero(copy_crc.transfer_size()), validation_status::size_is_null);

        RETURN_STATUS_IF(overlaps(copy_crc.source_address(), copy_crc.destination_address(), copy_crc.transfer_size()),
                         validation_status::buffers_overlap);

        return validation_status::success;
    }

    [[nodiscard]] static validation_status validate(views::cache_flush_descriptor cache_flush) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(cache_flush.destination_address()), validation_status::address_is_null);
        RETURN_STATUS_IF(any_equal_zero(cache_flush.transfer_size()), validation_status::size_is_null);

        return validation_status::success;
    }

    [[nodiscard]] static validation_status validate(views::dif_check_descriptor dif_check) noexcept
    {
        const auto block_size = dif_block_sizes[dif_check.dif_flags() & 0b11];

        RETURN_STATUS_IF(any_equal_zero(dif_check.source_address()), validation_status::address_is_null);
        RETURN_STATUS_IF(any_equal_zero(dif_check.transfer_size()), validation_status::size_is_null);
        RETURN_STATUS_IF(dif_check.transfer_size() % (block_size + sizeof(uint64_t)) != 0, validation_status::dif_size_is_wrong);

        return validation_status::success;
    }

    [[nodiscard]] static validation_status validate(views::dif_insert_descriptor dif_insert) noexcept
    {
        const auto block_size = dif_block_sizes[dif_insert.dif_flags() & 0b11];

        RETURN_STATUS_IF(any_equal_zero(dif_insert.source_address(), dif_insert.destination_address()), validation_status::address_is_null);
        RETURN_STATUS_IF(any_equal_zero(dif_insert.transfer_size()), validation_status::size_is_null);
        RETURN_STATUS_IF(dif_insert.transfer_size() % block_size != 0, validation_status::dif_size_is_wrong);

        const auto src_size = dif_insert.transfer_size();
        const auto dst_size = (src_size / block_size) * (block_size + static_cast<transfer_size_t>(sizeof(uint64_t)));
        RETURN_STATUS_IF(overlaps(dif_insert.source_address(), src_size, dif_insert.destination_address(), dst_size),
                         validation_status::buffers_overlap);

        return validation_status::success;
    }

    [[nodiscard]] static validation_status validate(views::dif_strip_descriptor dif_strip) noexcept
    {
        const auto block_size = dif_block_sizes[dif_strip.dif_flags() & 0b11];

        RETURN_STATUS_IF(any_equal_zero(dif_strip.source_address(), dif_strip.destination_address()), validation_status::address_is_null);
        RETURN_STATUS_IF(any_equal_zero(dif_strip.transfer_size()), validation_status::size_is_null);
        RETURN_STATUS_IF(dif_strip.transfer_size() % (block_size + sizeof(uint64_t)) != 0, validation_status::dif_size_is_wrong);

        const auto src_size = dif_strip.transfer_size();
        const auto dst_size = (src_size / (block_size + static_cast<transfer_size_t>(sizeof(uint64_t)))) * block_size;
        RETURN_STATUS_IF(overlaps(dif_strip.source_address(), src_size, dif_strip.destination_address(), dst_size),
                         validation_status::buffers_overlap);

        return validation_status::success;
    }

    [[nodiscard]] static validation_status validate(views::dif_update_descriptor dif_update) noexcept
    {
        const auto block_size = dif_block_sizes[dif_update.dif_flags() & 0b11];

        RETURN_STATUS_IF(any_equal_zero(dif_update.source_address(), dif_update.destination_address()), validation_status::address_is_null);
        RETURN_STATUS_IF(any_equal_zero(dif_update.transfer_size()), validation_status::size_is_null);
        RETURN_STATUS_IF(dif_update.transfer_size() % (block_size + sizeof(uint64_t)) != 0, validation_status::dif_size_is_wrong);
        RETURN_STATUS_IF(overlaps(dif_update.source_address(), dif_update.destination_address(), dif_update.transfer_size()),
                         validation_status::buffers_overlap);

        return validation_status::success;
    }

    [[nodiscard]] static validation_status validate(views::batch_descriptor batch) noexcept
    {
        RETURN_STATUS_IF(any_equal_zero(batch.descriptor_list_address()), validation_status::address_is_null);
        RETURN_STATUS_IF(batch.descriptors_count() < 4, validation_status::batch_size_is_wrong);

        return validation_status::success;
    }

}  // namespace dml::ml
