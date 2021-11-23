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

#include "dml/cpp/middle_layer/core.hpp"

#include <immintrin.h>

#include "core_api.h"
//#include "dif.hpp"
#include "dml/cpp/middle_layer/descriptor_views.hpp"
#include "dml/cpp/middle_layer/options.hpp"
#include "dml/cpp/middle_layer/result_views.hpp"

// TODO: Only for DIFs
#include <dml/dmldefs.h>

#include <cstring>

#include "sw_path_legacy/dif.h"

namespace dml::ml::core
{
    static inline void write_status(execution_status from_status, status_t &to_status) noexcept
    {
        _mm_sfence();
        to_status = static_cast<status_t>(from_status);
    }

    static inline execution_status evaluate(views::nop_descriptor dsc) noexcept;

    static inline execution_status evaluate(views::batch_descriptor dsc) noexcept;

    static inline execution_status evaluate(views::drain_descriptor dsc) noexcept;

    static inline execution_status evaluate(views::mem_move_descriptor dsc) noexcept;

    static inline execution_status evaluate(views::fill_descriptor dsc) noexcept;

    static inline execution_status evaluate(views::compare_descriptor dsc) noexcept;

    static inline execution_status evaluate(views::compare_pattern_descriptor dsc) noexcept;

    static inline execution_status evaluate(views::create_delta_descriptor dsc) noexcept;

    static inline execution_status evaluate(views::apply_delta_descriptor dsc) noexcept;

    static inline execution_status evaluate(views::dualcast_descriptor dsc) noexcept;

    static inline execution_status evaluate(views::crc_descriptor dsc) noexcept;

    static inline execution_status evaluate(views::copy_crc_descriptor dsc) noexcept;

    static inline execution_status evaluate(views::dif_check_descriptor dsc) noexcept;

    static inline execution_status evaluate(views::dif_insert_descriptor dsc) noexcept;

    static inline execution_status evaluate(views::dif_strip_descriptor dsc) noexcept;

    static inline execution_status evaluate(views::dif_update_descriptor dsc) noexcept;

    static inline execution_status evaluate(views::cache_flush_descriptor dsc) noexcept;

    execution_status submit(descriptor &dsc) noexcept
    {
        auto view = views::any_descriptor(dsc);

        switch (static_cast<operation>(view.operation()))
        {
            case operation::nop:
                return core::evaluate(views::nop_descriptor(dsc));
                break;
            case operation::batch:
                return core::evaluate(views::batch_descriptor(dsc));
                break;
            case operation::drain:
                return core::evaluate(views::drain_descriptor(dsc));
                break;
            case operation::memory_move:
                return core::evaluate(views::mem_move_descriptor(dsc));
                break;
            case operation::fill:
                return core::evaluate(views::fill_descriptor(dsc));
                break;
            case operation::compare:
                return core::evaluate(views::compare_descriptor(dsc));
                break;
            case operation::compare_pattern:
                return core::evaluate(views::compare_pattern_descriptor(dsc));
                break;
            case operation::create_delta:
                return core::evaluate(views::create_delta_descriptor(dsc));
                break;
            case operation::apply_delta:
                return core::evaluate(views::apply_delta_descriptor(dsc));
                break;
            case operation::dualcast:
                return core::evaluate(views::dualcast_descriptor(dsc));
                break;
            case operation::crc:
                return core::evaluate(views::crc_descriptor(dsc));
                break;
            case operation::copy_crc:
                return core::evaluate(views::copy_crc_descriptor(dsc));
                break;
            case operation::dif_check:
                return core::evaluate(views::dif_check_descriptor(dsc));
                break;
            case operation::dif_insert:
                return core::evaluate(views::dif_insert_descriptor(dsc));
                break;
            case operation::dif_strip:
                return core::evaluate(views::dif_strip_descriptor(dsc));
                break;
            case operation::dif_update:
                return core::evaluate(views::dif_update_descriptor(dsc));
                break;
            case operation::cache_flush:
                return core::evaluate(views::cache_flush_descriptor(dsc));
                break;
            default:
                return execution_status::unexpected;
        }
    }

    static inline execution_status evaluate(views::nop_descriptor dsc) noexcept
    {
        auto final_status = execution_status::success;
        if (dsc.completion_record_address())
        {
            auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
            auto result = views::nop_result(*record);

            write_status(final_status, result.status());
        }

        return final_status;
    }

    static inline execution_status evaluate(views::drain_descriptor dsc) noexcept
    {
        auto final_status = execution_status::success;
        if (dsc.completion_record_address())
        {
            auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
            auto result = views::drain_result(*record);

            write_status(final_status, result.status());
        }

        return final_status;
    }

    static inline execution_status evaluate(views::mem_move_descriptor dsc) noexcept
    {
        const auto src           = reinterpret_cast<byte_t *>(dsc.source_address());
        const auto dst           = reinterpret_cast<byte_t *>(dsc.destination_address());
        const auto transfer_size = dsc.transfer_size();

        const auto status = dmlc_move_8u(src, dst, transfer_size);

        auto final_status = (status == DML_STATUS_OK) ? execution_status::success : execution_status::unexpected;
        if (dsc.completion_record_address())
        {
            auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
            auto result = views::mem_move_result(*record);

            write_status(final_status, result.status());
        }

        return final_status;
    }

    static inline execution_status evaluate(views::fill_descriptor dsc) noexcept
    {
        const auto pattern       = dsc.pattern();
        const auto dst           = reinterpret_cast<byte_t *>(dsc.destination_address());
        const auto transfer_size = dsc.transfer_size();

        const auto status = dmlc_fill_with_pattern_8u(pattern, dst, transfer_size);

        auto final_status = (status == DML_STATUS_OK) ? execution_status::success : execution_status::unexpected;
        if (dsc.completion_record_address())
        {
            auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
            auto result = views::fill_result(*record);

            write_status(final_status, result.status());
        }

        return final_status;
    }

    static inline execution_status evaluate(views::compare_descriptor dsc) noexcept
    {
        const auto src1            = reinterpret_cast<byte_t *>(dsc.source_1_address());
        const auto src2            = reinterpret_cast<byte_t *>(dsc.source_2_address());
        const auto transfer_size   = dsc.transfer_size();
        const auto expected_result = dsc.expected_result();
        const auto options         = compare_options(dsc.flags());

        auto mismatch = transfer_size_t(0);

        const auto status = dmlc_compare_8u(src1, src2, transfer_size, &mismatch);

        result_t actual_result;
        if (status == DML_COMPARE_STATUS_EQ)
        {
            actual_result = 0;
        }
        else if (status == DML_COMPARE_STATUS_NE)
        {
            actual_result = 1;
        }
        else
        {
            if (dsc.completion_record_address())
            {
                auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
                auto result = views::compare_result(*record);
                write_status(execution_status::unexpected, result.status());
            }
            return execution_status::unexpected;
        }

        auto final_status = options.contains(compare_option::check_result)
                                ? (expected_result == actual_result) ? execution_status::success : execution_status::false_predicate_success
                                : execution_status::success;

        if (dsc.completion_record_address())
        {
            auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
            auto result = views::compare_result(*record);

            result.result()          = actual_result;
            result.bytes_completed() = mismatch;

            write_status(final_status, result.status());
        }

        return final_status;
    }

    static inline execution_status evaluate(views::compare_pattern_descriptor dsc) noexcept
    {
        const auto pattern         = dsc.pattern();
        const auto src             = reinterpret_cast<byte_t *>(dsc.source_address());
        const auto transfer_size   = dsc.transfer_size();
        const auto expected_result = dsc.expected_result();
        const auto options         = compare_options(dsc.flags());

        auto mismatch = transfer_size_t(0);

        const auto status = dmlc_compare_with_pattern_8u(src, pattern, transfer_size, &mismatch);

        result_t actual_result;
        if (status == DML_COMPARE_STATUS_EQ)
        {
            actual_result = 0;
        }
        else if (status == DML_COMPARE_STATUS_NE)
        {
            actual_result = 1;
        }
        else
        {
            if (dsc.completion_record_address())
            {
                auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
                auto result = views::compare_result(*record);
                write_status(execution_status::unexpected, result.status());
            }
            return execution_status::unexpected;
        }

        auto final_status = options.contains(compare_option::check_result)
                                ? (expected_result == actual_result) ? execution_status::success : execution_status::false_predicate_success
                                : execution_status::success;

        if (dsc.completion_record_address())
        {
            auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
            auto result = views::compare_result(*record);

            result.result()          = actual_result;
            result.bytes_completed() = mismatch;
            write_status(final_status, result.status());
        }

        return final_status;
    }

    static inline execution_status evaluate(views::create_delta_descriptor dsc) noexcept
    {
        const auto src1            = reinterpret_cast<byte_t *>(dsc.source_1_address());
        const auto src2            = reinterpret_cast<byte_t *>(dsc.source_2_address());
        const auto delta_record    = reinterpret_cast<byte_t *>(dsc.delta_record_address());
        const auto delta_max_size  = dsc.maximum_delta_record_size();
        const auto transfer_size   = dsc.transfer_size();
        const auto expected_result = dsc.expected_result_mask();
        const auto options         = create_delta_options(dsc.flags());

        auto delta_record_size = transfer_size_t(0);

        // Flip src1 and src2 due to core differs from hardware spec
        const auto status = dmlc_create_delta_record_8u(src2, src1, transfer_size, delta_max_size, delta_record, &delta_record_size);

        result_t actual_result;
        if (status == DML_STATUS_DELTA_RECORD_SIZE_ERROR)
        {
            actual_result = static_cast<result_t>(delta_expected_result_option::expect_overflow);
        }
        else if (status == DML_STATUS_OK)
        {
            actual_result = delta_record_size ? static_cast<result_t>(delta_expected_result_option::expect_not_equal)
                                              : static_cast<result_t>(delta_expected_result_option::expect_equal);
        }
        else
        {
            if (dsc.completion_record_address())
            {
                auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
                auto result = views::create_delta_result(*record);

                write_status(execution_status::unexpected, result.status());
            }
            return execution_status::unexpected;
        }

        auto final_status = options.contains(compare_option::check_result)
                                ? (expected_result == actual_result) ? execution_status::success : execution_status::false_predicate_success
                                : execution_status::success;

        if (dsc.completion_record_address())
        {
            auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
            auto result = views::create_delta_result(*record);

            result.delta_record_size() = delta_record_size;
            result.result()            = actual_result >> 1; // Hack, because of result and result mask uses different values
            write_status(final_status, result.status());
        }

        return final_status;
    }

    static inline execution_status evaluate(views::apply_delta_descriptor dsc) noexcept
    {
        const auto dst           = reinterpret_cast<byte_t *>(dsc.destination_address());
        const auto delta_record  = reinterpret_cast<byte_t *>(dsc.delta_record_address());
        const auto delta_size    = dsc.delta_record_size();
        const auto transfer_size = dsc.transfer_size();

        const auto status = dmlc_apply_delta_record_8u(dst, delta_record, transfer_size, delta_size);

        auto final_status = status == DML_STATUS_OK ? execution_status::success : execution_status::unexpected;
        if (dsc.completion_record_address())
        {
            auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
            auto result = views::apply_delta_result(*record);

            write_status(final_status, result.status());
        }

        return final_status;
    }

    static inline execution_status evaluate(views::dualcast_descriptor dsc) noexcept
    {
        const auto src           = reinterpret_cast<byte_t *>(dsc.source_address());
        const auto dst1          = reinterpret_cast<byte_t *>(dsc.destination_1_address());
        const auto dst2          = reinterpret_cast<byte_t *>(dsc.destination_2_address());
        const auto transfer_size = dsc.transfer_size();

        const auto status = dmlc_dualcast_copy_8u(src, dst1, dst2, transfer_size);

        auto final_status = status == DML_STATUS_OK ? execution_status::success : execution_status::unexpected;
        if (dsc.completion_record_address())
        {
            auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
            auto result = views::dualcast_result(*record);

            write_status(final_status, result.status());
        }

        return final_status;
    }

    static inline execution_status evaluate(views::crc_descriptor dsc) noexcept
    {
        const auto src           = reinterpret_cast<byte_t *>(dsc.source_address());
        const auto transfer_size = dsc.transfer_size();
        const auto crc_seed      = dsc.crc_seed();
        const auto options       = crc_additional_options(dsc.operation_specific_flags());

        constexpr auto polynomial = 0x1EDC6F41u;
        auto           reverse    = [](uint32_t value)
        {
            value = (value & 0x55555555u) << 1u | (value & 0xAAAAAAAAu) >> 1u;
            value = (value & 0x33333333u) << 2u | (value & 0xCCCCCCCCu) >> 2u;
            value = (value & 0x0F0F0F0Fu) << 4u | (value & 0xF0F0F0F0u) >> 4u;
            value = (value & 0x00FF00FFu) << 8u | (value & 0xFF00FF00u) >> 8u;
            value = (value & 0x0000FFFFu) << 16u | (value & 0xFFFF0000u) >> 16u;

            return value;
        };

        const auto bypass_reflection      = options.contains(crc_additional_option::bypass_reflection);
        const auto bypass_data_reflection = options.contains(crc_additional_option::bypass_data_reflection);

        auto crc_value = crc_seed;

        // Bypass inversion and use reverse bit order for CRC completion_record
        if (!bypass_reflection)
        {
            crc_value = ~(crc_value);
            crc_value = reverse(crc_value);
        }

        // Bypass Data Reflection in case if DML_FLAG_DATA_REFLECTION set
        auto status = (!bypass_data_reflection) ? dmlc_calculate_crc_reflected_32u(src, transfer_size, &crc_value, polynomial)
                                                : dmlc_calculate_crc_32u(src, transfer_size, &crc_value, polynomial);

        // Bypass inversion and use reverse bit order for CRC completion_record
        if (!bypass_reflection)
        {
            crc_value = reverse(crc_value);
            crc_value = ~(crc_value);
        }

        auto final_status = status == DML_STATUS_OK ? execution_status::success : execution_status::unexpected;
        if (dsc.completion_record_address())
        {
            auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
            auto result = views::crc_result(*record);

            result.crc_value() = crc_value;
            write_status(final_status, result.status());
        }

        return final_status;
    }

    static inline execution_status evaluate(views::copy_crc_descriptor dsc) noexcept
    {
        const auto src           = reinterpret_cast<byte_t *>(dsc.source_address());
        const auto dst           = reinterpret_cast<byte_t *>(dsc.destination_address());
        const auto transfer_size = dsc.transfer_size();
        const auto crc_seed      = dsc.crc_seed();
        const auto options       = crc_additional_options(dsc.operation_specific_flags());

        {
            const auto status = dmlc_move_8u(src, dst, transfer_size);

            if (status != DML_STATUS_OK)
            {
                auto final_status = execution_status::unexpected;
                if (dsc.completion_record_address())
                {
                    auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
                    auto result = views::any_result(*record);

                    write_status(final_status, result.status());
                }
                return final_status;
            }
        }

        constexpr auto polynomial = 0x1EDC6F41u;
        auto           reverse    = [](uint32_t value)
        {
            value = (value & 0x55555555u) << 1u | (value & 0xAAAAAAAAu) >> 1u;
            value = (value & 0x33333333u) << 2u | (value & 0xCCCCCCCCu) >> 2u;
            value = (value & 0x0F0F0F0Fu) << 4u | (value & 0xF0F0F0F0u) >> 4u;
            value = (value & 0x00FF00FFu) << 8u | (value & 0xFF00FF00u) >> 8u;
            value = (value & 0x0000FFFFu) << 16u | (value & 0xFFFF0000u) >> 16u;

            return value;
        };

        const auto bypass_reflection      = options.contains(crc_additional_option::bypass_reflection);
        const auto bypass_data_reflection = options.contains(crc_additional_option::bypass_data_reflection);

        auto crc_value = crc_seed;

        // Bypass inversion and use reverse bit order for CRC completion_record
        if (!bypass_reflection)
        {
            crc_value = ~(crc_value);
            crc_value = reverse(crc_value);
        }

        // Bypass Data Reflection in case if DML_FLAG_DATA_REFLECTION set
        const auto status = (!bypass_data_reflection) ? dmlc_calculate_crc_reflected_32u(src, transfer_size, &crc_value, polynomial)
                                                      : dmlc_calculate_crc_32u(src, transfer_size, &crc_value, polynomial);

        // Bypass inversion and use reverse bit order for CRC completion_record
        if (!bypass_reflection)
        {
            crc_value = reverse(crc_value);
            crc_value = ~(crc_value);
        }

        auto final_status = status == DML_STATUS_OK ? execution_status::success : execution_status::unexpected;
        if (dsc.completion_record_address())
        {
            auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
            auto result = views::crc_result(*record);

            result.crc_value() = crc_value;
            write_status(final_status, result.status());
        }

        return final_status;
    }

    static inline execution_status evaluate(views::dif_check_descriptor dsc) noexcept
    {
        constexpr uint32_t dif_block_sizes[4] = { 512u, 520u, 4096u, 4104u };

        const auto src              = reinterpret_cast<byte_t *>(dsc.source_address());
        const auto transfer_size    = dsc.transfer_size();
        const auto options          = dif_check_options(dsc.flags());
        const auto dif_options      = dif_additional_options(dsc.dif_flags());
        const auto dif_src_options  = dif_additional_src_options(dsc.source_dif_flags());
        const auto src_app_tag_mask = dsc.source_app_tag_mask();

        const auto src_ref_tag = dsc.source_ref_tag();
        const auto src_app_tag = dsc.source_app_tag();

        dml_job_t job;
        memset(&job, 0, sizeof(dml_job_t));
        job.source_first_ptr                       = src;
        job.source_length                          = transfer_size;
        job.operation                              = DML_OP_DIF_CHECK;
        job.dif_config.source_reference_tag_seed   = src_ref_tag;
        job.dif_config.source_application_tag_seed = src_app_tag;
        job.dif_config.source_application_tag_mask = src_app_tag_mask;
        job.dif_config.block_size = static_cast<dml_dif_block_size_t>(static_cast<operation_specific_flags_t>(dif_options) & 0b11);

        // Job API composes DIF flags into one 64-bit value via shifting, check dmldefs.h
        job.dif_config.flags =
            (static_cast<uint64_t>(static_cast<dif_flags_t>(dif_options)) << 16) | (static_cast<dif_flags_t>(dif_src_options));

        job.flags = static_cast<flags_t>(options);

        auto status = dml_legacy_dif_check(&job);

        // Unsupported operation
        auto final_status = (status == DML_STATUS_OK) ? execution_status::success : execution_status::dif_control_error;
        if (dsc.completion_record_address())
        {
            auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
            auto result = views::dif_check_result(*record);

            result.dif_status()      = job.result;
            result.bytes_completed() = job.offset;
            // TODO: Should also write values for tags
            write_status(final_status, result.status());
        }

        return final_status;
    }

    static inline execution_status evaluate(views::dif_insert_descriptor dsc) noexcept
    {
        const auto src              = reinterpret_cast<byte_t *>(dsc.source_address());
        const auto dst              = reinterpret_cast<byte_t *>(dsc.destination_address());
        const auto transfer_size    = dsc.transfer_size();
        const auto options          = dif_insert_options(dsc.flags());
        const auto dif_options      = dif_additional_options(dsc.dif_flags());
        const auto dif_dst_options  = dif_additional_src_options(dsc.destination_dif_flags());
        const auto dst_app_tag_mask = dsc.destination_app_tag_mask();

        auto dst_ref_tag = dsc.destination_ref_tag();
        auto dst_app_tag = dsc.destination_app_tag();

        dml_job_t job;
        memset(&job, 0, sizeof(dml_job_t));
        job.source_first_ptr                            = src;
        job.destination_first_ptr                       = dst;
        job.source_length                               = transfer_size;
        job.operation                                   = DML_OP_DIF_INSERT;
        job.dif_config.destination_reference_tag_seed   = dst_ref_tag;
        job.dif_config.destination_application_tag_seed = dst_app_tag;
        job.dif_config.destination_application_tag_mask = dst_app_tag_mask;
        job.dif_config.block_size = static_cast<dml_dif_block_size_t>(static_cast<operation_specific_flags_t>(dif_options) & 0b11);

        // Job API composes DIF flags into one 64-bit value via shifting, check dmldefs.h
        job.dif_config.flags =
            (static_cast<uint64_t>(static_cast<dif_flags_t>(dif_options)) << 16) | (static_cast<dif_flags_t>(dif_dst_options) << 8);

        job.flags = static_cast<flags_t>(options);

        auto status = dml_legacy_dif_insert(&job);

        // Unsupported operation
        auto final_status = (status == DML_STATUS_OK) ? execution_status::success : execution_status::dif_control_error;
        if (dsc.completion_record_address())
        {
            auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
            auto result = views::dif_insert_result(*record);

            result.bytes_completed() = job.offset;
            // TODO: Should also write values for tags
            write_status(final_status, result.status());
        }

        return final_status;
    }

    static inline execution_status evaluate(views::dif_strip_descriptor dsc) noexcept
    {
        const auto src              = reinterpret_cast<byte_t *>(dsc.source_address());
        const auto dst              = reinterpret_cast<byte_t *>(dsc.destination_address());
        const auto transfer_size    = dsc.transfer_size();
        const auto options          = dif_strip_options(dsc.flags());
        const auto dif_options      = dif_additional_options(dsc.dif_flags());
        const auto dif_src_options  = dif_additional_src_options(dsc.source_dif_flags());
        const auto src_app_tag_mask = dsc.source_app_tag_mask();

        auto src_ref_tag = dsc.source_ref_tag();
        auto src_app_tag = dsc.source_app_tag();

        dml_job_t job;
        memset(&job, 0, sizeof(dml_job_t));
        job.source_first_ptr                       = src;
        job.destination_first_ptr                  = dst;
        job.source_length                          = transfer_size;
        job.operation                              = DML_OP_DIF_STRIP;
        job.dif_config.source_reference_tag_seed   = src_ref_tag;
        job.dif_config.source_application_tag_seed = src_app_tag;
        job.dif_config.source_application_tag_mask = src_app_tag_mask;
        job.dif_config.block_size = static_cast<dml_dif_block_size_t>(static_cast<operation_specific_flags_t>(dif_options) & 0b11);

        // Job API composes DIF flags into one 64-bit value via shifting, check dmldefs.h
        job.dif_config.flags =
            (static_cast<uint64_t>(static_cast<dif_flags_t>(dif_options)) << 16) | (static_cast<dif_flags_t>(dif_src_options));

        job.flags = static_cast<flags_t>(options);

        auto status = dml_legacy_dif_strip(&job);

        // Unsupported operation
        auto final_status = (status == DML_STATUS_OK) ? execution_status::success : execution_status::dif_control_error;
        if (dsc.completion_record_address())
        {
            auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
            auto result = views::dif_strip_result(*record);

            result.dif_status()      = job.result;
            result.bytes_completed() = job.offset;
            // TODO: Should also write values for tags
            write_status(final_status, result.status());
        }

        return final_status;
    }

    static inline execution_status evaluate(views::dif_update_descriptor dsc) noexcept
    {
        const auto src              = reinterpret_cast<byte_t *>(dsc.source_address());
        const auto dst              = reinterpret_cast<byte_t *>(dsc.destination_address());
        const auto transfer_size    = dsc.transfer_size();
        const auto options          = dif_update_options(dsc.flags());
        const auto dif_options      = dif_additional_options(dsc.dif_flags());
        const auto dif_src_options  = dif_additional_src_options(dsc.source_dif_flags());
        const auto dif_dst_options  = dif_additional_src_options(dsc.destination_dif_flags());
        const auto src_app_tag_mask = dsc.source_app_tag_mask();
        const auto dst_app_tag_mask = dsc.destination_app_tag_mask();

        auto src_ref_tag = dsc.source_ref_tag();
        auto dst_ref_tag = dsc.destination_ref_tag();
        auto src_app_tag = dsc.source_app_tag();
        auto dst_app_tag = dsc.destination_app_tag();

        dml_job_t job;
        memset(&job, 0, sizeof(dml_job_t));
        job.source_first_ptr                            = src;
        job.destination_first_ptr                       = dst;
        job.source_length                               = transfer_size;
        job.operation                                   = DML_OP_DIF_UPDATE;
        job.dif_config.source_reference_tag_seed        = src_ref_tag;
        job.dif_config.source_application_tag_seed      = src_app_tag;
        job.dif_config.source_application_tag_mask      = src_app_tag_mask;
        job.dif_config.destination_reference_tag_seed   = dst_ref_tag;
        job.dif_config.destination_application_tag_seed = dst_app_tag;
        job.dif_config.destination_application_tag_mask = dst_app_tag_mask;
        job.dif_config.block_size = static_cast<dml_dif_block_size_t>(static_cast<operation_specific_flags_t>(dif_options) & 0b11);

        // Job API composes DIF flags into one 64-bit value via shifting, check dmldefs.h
        job.dif_config.flags = (static_cast<uint64_t>(static_cast<dif_flags_t>(dif_options)) << 16) |
                               (static_cast<dif_flags_t>(dif_dst_options) << 8) | (static_cast<dif_flags_t>(dif_src_options));

        job.flags = static_cast<flags_t>(options);

        auto status = dml_legacy_dif_update(&job);

        // Unsupported operation
        auto final_status = (status == DML_STATUS_OK) ? execution_status::success : execution_status::dif_control_error;
        if (dsc.completion_record_address())
        {
            auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
            auto result = views::dif_update_result(*record);

            result.dif_status()      = job.result;
            result.bytes_completed() = job.offset;
            // TODO: Should also write values for tags
            write_status(final_status, result.status());
        }

        return final_status;
    }

    static inline execution_status evaluate(views::cache_flush_descriptor dsc) noexcept
    {
        const auto dst           = reinterpret_cast<byte_t *>(dsc.destination_address());
        const auto transfer_size = dsc.transfer_size();
        const auto options       = cache_flush_options(dsc.operation_specific_flags());

        auto status = options.contains(cache_flush_option::cache_control) ? dmlc_copy_cache_to_memory_8u(dst, transfer_size)
                                                                          : dmlc_move_cache_to_memory_8u(dst, transfer_size);

        auto final_status = status == DML_STATUS_OK ? execution_status::success : execution_status::unexpected;
        if (dsc.completion_record_address())
        {
            auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
            auto result = views::cache_flush_result(*record);

            write_status(final_status, result.status());
        }

        return final_status;
    }

    static inline execution_status evaluate(views::batch_descriptor dsc) noexcept
    {
        const auto operations        = reinterpret_cast<descriptor *>(dsc.descriptor_list_address());
        const auto descriptors_count = dsc.descriptors_count();

        auto final_status = execution_status::success;
        auto index        = transfer_size_t(0);
        for (index = 0; index < descriptors_count; ++index)
        {
            auto &op_dsc = operations[index];

            auto status = submit(op_dsc);

            if (status != execution_status::success)
            {
                final_status = status;
                break;
            }
        }

        if (dsc.completion_record_address())
        {
            auto record = reinterpret_cast<completion_record *>(dsc.completion_record_address());
            auto result = views::batch_result(*record);

            result.descriptors_completed() = index;
            write_status(final_status, result.status());
        }

        return final_status;
    }

}  // namespace dml::ml::core
