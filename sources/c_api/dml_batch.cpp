/*
 * Copyright 2020-2021 Intel Corporation.
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

#include <dml/detail/common/flags.hpp>
#include <dml/detail/common/specific_flags.hpp>
#include <dml/detail/common/types.hpp>
#include <dml/detail/ml/operation.hpp>
#include <dml/detail/ml/options.hpp>
#include <dml/detail/ml/result.hpp>
#include <dml/detail/ml/validation.hpp>

#include "dml/dml.h"
#include "macros.hpp"
#include "range_check.hpp"
#include "status.hpp"
#include "utils.hpp"

namespace dml
{

    constexpr auto get_task_size() noexcept -> uint32_t
    {
        return sizeof(dml::detail::ml::operation) + sizeof(dml::detail::ml::result);
    }

    class batch
    {
    public:
        batch(uint8_t *batch_data, uint32_t tasks_count) noexcept: batch_data_(dml::align(batch_data)), tasks_count_(tasks_count)
        {
        }

        template <typename make_operation>
        void add_by_index(uint32_t index, make_operation &&make) const noexcept
        {
            const auto operations = reinterpret_cast<dml::detail::ml::operation *>(batch_data_);

            operations[index] = make();

            const auto results = reinterpret_cast<dml::detail::ml::result *>(operations + tasks_count_);

            dml::detail::ml::bind(operations[index], results[index]);
        }

        [[nodiscard]] auto get_status(uint32_t index) const noexcept
        {
            const auto operations = reinterpret_cast<dml::detail::ml::operation *>(batch_data_);

            const auto records = reinterpret_cast<dml::detail::ml::result *>(operations + tasks_count_);

            return to_own_status(dml::detail::ml::get_status(records[index]));
        }

        [[nodiscard]] auto get_result(uint32_t index) const noexcept
        {
            const auto operations = reinterpret_cast<dml::detail::ml::operation *>(batch_data_);

            const auto records = reinterpret_cast<dml::detail::ml::result *>(operations + tasks_count_);

            return dml::detail::ml::get_result(records[index]);
        }

    private:
        uint8_t *batch_data_;
        uint32_t tasks_count_;
    };
}  // namespace dml

extern "C" dml_status_t dml_get_batch_size(const dml_job_t *dml_job_ptr, uint32_t task_count, uint32_t *byte_size_ptr)
{
    CHECK_NULL(dml_job_ptr);
    CHECK_NULL(byte_size_ptr);
    if (task_count <= 3u)
    {
        return DML_STATUS_BATCH_SIZE_ERROR;
    }

    const uint32_t required_alignment = 64u;

    *byte_size_ptr = dml::get_task_size() * task_count + required_alignment;

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_batch_set_nop_by_index(dml_job_t *dml_job_ptr, uint32_t task_index, dml_operation_flags_t flags)
{
    CHECK_NULL(dml_job_ptr);
    CHECK_NULL(dml_job_ptr->destination_length);

    const auto task_count = dml_job_ptr->destination_length / dml::get_task_size();
    if (task_index >= task_count)
    {
        return DML_STATUS_BATCH_TASK_INDEX_OVERFLOW;
    }

    dml::batch(dml_job_ptr->destination_first_ptr, task_count)
        .add_by_index(task_index,
                      [&]
                      {
                          return dml::detail::ml::make_nop_operation(dml::detail::ml::nop_options(static_cast<uint16_t>(flags & 0xFFFF)));
                      });

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_batch_set_mem_move_by_index(dml_job_t            *dml_job_ptr,
                                                        uint32_t              task_index,
                                                        uint8_t              *source_ptr,
                                                        uint8_t              *destination_ptr,
                                                        uint32_t              byte_length,
                                                        dml_operation_flags_t flags)
{
    CHECK_NULL(dml_job_ptr);
    CHECK_NULL(dml_job_ptr->destination_length);

    const auto task_count = dml_job_ptr->destination_length / dml::get_task_size();
    if (task_index >= task_count)
    {
        return DML_STATUS_BATCH_TASK_INDEX_OVERFLOW;
    }

    auto status = dml::range_check_mem_move(source_ptr, destination_ptr, byte_length, static_cast<uint16_t>(flags));
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    auto operation = dml::detail::ml::make_mem_move_operation(source_ptr,
                                                              destination_ptr,
                                                              byte_length,
                                                              dml::detail::ml::mem_move_options(static_cast<uint16_t>(flags & 0xFFFF)));

    status = dml::to_own_status(dml::detail::ml::validate(operation));
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    dml::batch(dml_job_ptr->destination_first_ptr, task_count)
        .add_by_index(task_index,
                      [&]
                      {
                          return operation;
                      });

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_batch_set_dualcast_by_index(dml_job_t            *dml_job_ptr,
                                                        uint32_t              task_index,
                                                        uint8_t              *source_ptr,
                                                        uint8_t              *destination_first_ptr,
                                                        uint8_t              *destination_second_ptr,
                                                        uint32_t              byte_length,
                                                        dml_operation_flags_t flags)
{
    CHECK_NULL(dml_job_ptr);
    CHECK_NULL(dml_job_ptr->destination_length);

    const auto task_count = dml_job_ptr->destination_length / dml::get_task_size();
    if (task_index >= task_count)
    {
        return DML_STATUS_BATCH_TASK_INDEX_OVERFLOW;
    }

    auto operation =
        dml::detail::ml::make_dualcast_operation(source_ptr,
                                                 destination_first_ptr,
                                                 destination_second_ptr,
                                                 byte_length,
                                                 dml::detail::ml::dualcast_options(static_cast<uint16_t>(flags & 0xFFFF)),
                                                 dml::detail::ml::dualcast_specific_options(static_cast<uint8_t>((flags >> 16) & 0xFF)));

    auto status = dml::to_own_status(dml::detail::ml::validate(operation));
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    dml::batch(dml_job_ptr->destination_first_ptr, task_count)
        .add_by_index(task_index,
                      [&]
                      {
                          return operation;
                      });

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_batch_set_compare_by_index(dml_job_t            *dml_job_ptr,
                                                       uint32_t              task_index,
                                                       uint8_t              *source_first_ptr,
                                                       uint8_t              *source_second_ptr,
                                                       uint32_t              byte_length,
                                                       dml_meta_result_t     expected_result,
                                                       dml_operation_flags_t flags)
{
    CHECK_NULL(dml_job_ptr);
    CHECK_NULL(dml_job_ptr->destination_length);

    const auto task_count = dml_job_ptr->destination_length / dml::get_task_size();
    if (task_index >= task_count)
    {
        return DML_STATUS_BATCH_TASK_INDEX_OVERFLOW;
    }

    auto operation = make_compare_operation(source_first_ptr,
                                            source_second_ptr,
                                            byte_length,
                                            dml::detail::ml::compare_options(static_cast<uint16_t>(flags & 0xFFFF)),
                                            dml::detail::compare_result(expected_result));
    auto status    = dml::to_own_status(dml::detail::ml::validate(operation));
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    dml::batch(dml_job_ptr->destination_first_ptr, task_count)
        .add_by_index(task_index,
                      [&]
                      {
                          return operation;
                      });

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_batch_set_compare_pattern_by_index(dml_job_t            *dml_job_ptr,
                                                               uint32_t              task_index,
                                                               uint8_t              *source_ptr,
                                                               uint8_t              *pattern_ptr,
                                                               uint32_t              byte_length,
                                                               dml_meta_result_t     expected_result,
                                                               dml_operation_flags_t flags)
{
    CHECK_NULL(dml_job_ptr);
    CHECK_NULL(dml_job_ptr->destination_length);

    const auto task_count = dml_job_ptr->destination_length / dml::get_task_size();
    CHECK_NULL(pattern_ptr);
    if (task_index >= task_count)
    {
        return DML_STATUS_BATCH_TASK_INDEX_OVERFLOW;
    }

    auto operation =
        dml::detail::ml::make_compare_pattern_operation(*reinterpret_cast<uint64_t *>(pattern_ptr),
                                                        source_ptr,
                                                        byte_length,
                                                        dml::detail::ml::compare_pattern_options(static_cast<uint16_t>(flags & 0xFFFF)),
                                                        dml::detail::compare_result(expected_result));
    auto status = dml::to_own_status(dml::detail::ml::validate(operation));
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    dml::batch(dml_job_ptr->destination_first_ptr, task_count)
        .add_by_index(task_index,
                      [&]
                      {
                          return operation;
                      });

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_batch_set_crc_by_index(dml_job_t            *dml_job_ptr,
                                                   uint32_t              task_index,
                                                   uint8_t              *source_ptr,
                                                   uint32_t              byte_length,
                                                   uint32_t             *crc_seed_ptr,
                                                   dml_operation_flags_t flags)
{
    CHECK_NULL(dml_job_ptr);
    CHECK_NULL(dml_job_ptr->destination_length);

    const auto task_count = dml_job_ptr->destination_length / dml::get_task_size();
    CHECK_NULL(crc_seed_ptr);
    if (task_index >= task_count)
    {
        return DML_STATUS_BATCH_TASK_INDEX_OVERFLOW;
    }

    auto status = dml::range_check_crc(source_ptr, crc_seed_ptr, byte_length);
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    auto operation = dml::detail::ml::make_crc_operation(source_ptr,
                                                         byte_length,
                                                         *crc_seed_ptr,
                                                         dml::detail::ml::crc_options(static_cast<uint16_t>(flags & 0xFFFF)),
                                                         dml::detail::ml::crc_specific_options(static_cast<uint8_t>((flags >> 16) & 0xFF)));
    status         = dml::to_own_status(dml::detail::ml::validate(operation));
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    dml::batch(dml_job_ptr->destination_first_ptr, task_count)
        .add_by_index(task_index,
                      [&]
                      {
                          return operation;
                      });

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_batch_set_copy_crc_by_index(dml_job_t            *dml_job_ptr,
                                                        uint32_t              task_index,
                                                        uint8_t              *source_ptr,
                                                        uint32_t              byte_length,
                                                        uint32_t             *crc_seed_ptr,
                                                        uint8_t              *destination_ptr,
                                                        dml_operation_flags_t flags)
{
    CHECK_NULL(dml_job_ptr);
    CHECK_NULL(dml_job_ptr->destination_length);

    const auto task_count = dml_job_ptr->destination_length / dml::get_task_size();
    CHECK_NULL(crc_seed_ptr);
    if (task_index >= task_count)
    {
        return DML_STATUS_BATCH_TASK_INDEX_OVERFLOW;
    }

    auto status = dml::range_check_copy_crc(source_ptr, destination_ptr, crc_seed_ptr, byte_length);
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    auto operation =
        dml::detail::ml::make_copy_crc_operation(source_ptr,
                                                 destination_ptr,
                                                 byte_length,
                                                 *crc_seed_ptr,
                                                 dml::detail::ml::copy_crc_options(static_cast<uint16_t>(flags & 0xFFFF)),
                                                 dml::detail::ml::copy_crc_specific_options(static_cast<uint8_t>((flags >> 16) & 0xFF)));
    status = dml::to_own_status(dml::detail::ml::validate(operation));
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    dml::batch(dml_job_ptr->destination_first_ptr, task_count)
        .add_by_index(task_index,
                      [&]
                      {
                          return operation;
                      });

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_batch_set_fill_by_index(dml_job_t            *dml_job_ptr,
                                                    uint32_t              task_index,
                                                    const uint8_t        *pattern_ptr,
                                                    uint8_t              *destination_ptr,
                                                    uint32_t              byte_length,
                                                    dml_operation_flags_t flags)
{
    CHECK_NULL(dml_job_ptr);
    CHECK_NULL(dml_job_ptr->destination_length);
    CHECK_NULL(pattern_ptr);

    const auto task_count = dml_job_ptr->destination_length / dml::get_task_size();
    if (task_index >= task_count)
    {
        return DML_STATUS_BATCH_TASK_INDEX_OVERFLOW;
    }

    auto operation = dml::detail::ml::make_fill_operation(*reinterpret_cast<const uint64_t *>(pattern_ptr),
                                                          destination_ptr,
                                                          byte_length,
                                                          dml::detail::ml::fill_options(static_cast<uint16_t>(flags & 0xFFFF)));
    auto status    = dml::to_own_status(dml::detail::ml::validate(operation));
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    dml::batch(dml_job_ptr->destination_first_ptr, task_count)
        .add_by_index(task_index,
                      [&]
                      {
                          return operation;
                      });

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_batch_set_cache_flush_by_index(dml_job_t            *dml_job_ptr,
                                                           uint32_t              task_index,
                                                           uint8_t              *destination_ptr,
                                                           uint32_t              byte_length,
                                                           dml_operation_flags_t flags)
{
    CHECK_NULL(dml_job_ptr);
    CHECK_NULL(dml_job_ptr->destination_length);

    const auto task_count = dml_job_ptr->destination_length / dml::get_task_size();
    if (task_index >= task_count)
    {
        return DML_STATUS_BATCH_TASK_INDEX_OVERFLOW;
    }

    auto operation =
        dml::detail::ml::make_cache_flush_operation(destination_ptr,
                                                    byte_length,
                                                    dml::detail::ml::cache_flush_options(static_cast<uint16_t>(flags & 0xFFFF)));
    auto status = dml::to_own_status(dml::detail::ml::validate(operation));
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    dml::batch(dml_job_ptr->destination_first_ptr, task_count)
        .add_by_index(task_index,
                      [&]
                      {
                          return operation;
                      });

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_batch_set_delta_create_by_index(dml_job_t            *dml_job_ptr,
                                                            uint32_t              task_index,
                                                            uint8_t              *source_ptr,
                                                            uint8_t              *reference_ptr,
                                                            uint32_t              compare_length,
                                                            uint8_t              *delta_record_ptr,
                                                            uint32_t              delta_record_length,
                                                            dml_meta_result_t     expected_result,
                                                            dml_operation_flags_t flags)
{
    CHECK_NULL(dml_job_ptr);
    CHECK_NULL(dml_job_ptr->destination_length);

    const auto task_count = dml_job_ptr->destination_length / dml::get_task_size();
    if (task_index >= task_count)
    {
        return DML_STATUS_BATCH_TASK_INDEX_OVERFLOW;
    }

    auto operation =
        dml::detail::ml::make_create_delta_operation(source_ptr,
                                                     reference_ptr,
                                                     compare_length,
                                                     delta_record_ptr,
                                                     delta_record_length,
                                                     dml::detail::ml::create_delta_options(static_cast<uint16_t>(flags & 0xFFFF)),
                                                     dml::detail::create_delta_result(expected_result));
    auto status = dml::to_own_status(dml::detail::ml::validate(operation));
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    dml::batch(dml_job_ptr->destination_first_ptr, task_count)
        .add_by_index(task_index,
                      [&]
                      {
                          return operation;
                      });

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_batch_set_delta_apply_by_index(dml_job_t            *dml_job_ptr,
                                                           uint32_t              task_index,
                                                           uint8_t              *delta_record_ptr,
                                                           uint32_t              delta_record_length,
                                                           uint8_t              *destination_ptr,
                                                           uint32_t              destination_length,
                                                           dml_operation_flags_t flags)
{
    CHECK_NULL(dml_job_ptr);
    CHECK_NULL(dml_job_ptr->destination_length);

    const auto task_count = dml_job_ptr->destination_length / dml::get_task_size();
    if (task_index >= task_count)
    {
        return DML_STATUS_BATCH_TASK_INDEX_OVERFLOW;
    }

    auto operation =
        dml::detail::ml::make_apply_delta_operation(delta_record_ptr,
                                                    delta_record_length,
                                                    destination_ptr,
                                                    destination_length,
                                                    dml::detail::ml::apply_delta_options(static_cast<uint16_t>(flags & 0xFFFF)));
    auto status = dml::to_own_status(dml::detail::ml::validate(operation));
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    dml::batch(dml_job_ptr->destination_first_ptr, task_count)
        .add_by_index(task_index,
                      [&]
                      {
                          return operation;
                      });

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_batch_set_dif_check_by_index(dml_job_t              *dml_job_ptr,
                                                         uint32_t                task_index,
                                                         uint8_t                *source_ptr,
                                                         uint32_t                source_length,
                                                         const dml_dif_config_t *dif_config_ptr,
                                                         dml_operation_flags_t   flags)
{
    CHECK_NULL(dml_job_ptr);
    CHECK_NULL(dml_job_ptr->destination_length);
    CHECK_NULL(source_ptr);
    CHECK_NULL(dif_config_ptr);

    const auto task_count = dml_job_ptr->destination_length / dml::get_task_size();
    if (task_index >= task_count)
    {
        return DML_STATUS_BATCH_TASK_INDEX_OVERFLOW;
    }

    auto operation = dml::detail::ml::make_dif_check_operation(
        source_ptr,
        source_length,
        { dif_config_ptr->source_reference_tag_seed,
          dif_config_ptr->source_application_tag_mask,
          dif_config_ptr->source_application_tag_seed },
        dml::detail::ml::dif_check_options(static_cast<uint16_t>(flags & 0xFFFF)),
        dml::detail::ml::dif_specific_options(static_cast<uint8_t>(((dif_config_ptr->flags >> 16) & 0xFF) | dif_config_ptr->block_size)),
        dml::detail::ml::dif_source_options(static_cast<uint8_t>(dif_config_ptr->flags & 0xFF)));
    auto status = dml::to_own_status(dml::detail::ml::validate(operation));
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    dml::batch(dml_job_ptr->destination_first_ptr, task_count)
        .add_by_index(task_index,
                      [&]
                      {
                          return operation;
                      });

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_batch_set_dif_update_by_index(dml_job_t              *dml_job_ptr,
                                                          uint32_t                task_index,
                                                          uint8_t                *source_ptr,
                                                          uint32_t                source_length,
                                                          const dml_dif_config_t *dif_config_ptr,
                                                          uint8_t                *destination_ptr,
                                                          uint32_t                destination_length,
                                                          dml_operation_flags_t   flags)
{
    static_cast<void>(destination_length);

    CHECK_NULL(dml_job_ptr);
    CHECK_NULL(dml_job_ptr->destination_length);
    CHECK_NULL(source_ptr);
    CHECK_NULL(dif_config_ptr);
    CHECK_NULL(destination_ptr);

    const auto task_count = dml_job_ptr->destination_length / dml::get_task_size();
    if (task_index >= task_count)
    {
        return DML_STATUS_BATCH_TASK_INDEX_OVERFLOW;
    }

    auto operation = dml::detail::ml::make_dif_update_operation(
        source_ptr,
        destination_ptr,
        source_length,
        { dif_config_ptr->source_reference_tag_seed,
          dif_config_ptr->source_application_tag_mask,
          dif_config_ptr->source_application_tag_seed },
        { dif_config_ptr->destination_reference_tag_seed,
          dif_config_ptr->destination_application_tag_mask,
          dif_config_ptr->destination_application_tag_seed },
        dml::detail::ml::dif_update_options(static_cast<uint16_t>(flags & 0xFFFF)),
        dml::detail::ml::dif_specific_options(static_cast<uint8_t>(((dif_config_ptr->flags >> 16) & 0xFF) | dif_config_ptr->block_size)),
        dml::detail::ml::dif_source_options(static_cast<uint8_t>(dif_config_ptr->flags & 0xFF)),
        dml::detail::ml::dif_destination_options(static_cast<uint8_t>((dif_config_ptr->flags >> 8) & 0xFF)));
    auto status = dml::to_own_status(dml::detail::ml::validate(operation));
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    dml::batch(dml_job_ptr->destination_first_ptr, task_count)
        .add_by_index(task_index,
                      [&]
                      {
                          return operation;
                      });

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_batch_set_dif_insert_by_index(dml_job_t              *dml_job_ptr,
                                                          uint32_t                task_index,
                                                          uint8_t                *source_ptr,
                                                          uint32_t                source_length,
                                                          const dml_dif_config_t *dif_config_ptr,
                                                          uint8_t                *destination_ptr,
                                                          uint32_t                destination_length,
                                                          dml_operation_flags_t   flags)
{
    static_cast<void>(destination_length);

    CHECK_NULL(dml_job_ptr);
    CHECK_NULL(dml_job_ptr->destination_length);
    CHECK_NULL(source_ptr);
    CHECK_NULL(dif_config_ptr);
    CHECK_NULL(destination_ptr);

    const auto task_count = dml_job_ptr->destination_length / dml::get_task_size();
    if (task_index >= task_count)
    {
        return DML_STATUS_BATCH_TASK_INDEX_OVERFLOW;
    }

    auto operation = dml::detail::ml::make_dif_insert_operation(
        source_ptr,
        destination_ptr,
        source_length,
        { dif_config_ptr->destination_reference_tag_seed,
          dif_config_ptr->destination_application_tag_mask,
          dif_config_ptr->destination_application_tag_seed },
        dml::detail::ml::dif_insert_options(static_cast<uint16_t>(flags & 0xFFFF)),
        dml::detail::ml::dif_specific_options(static_cast<uint8_t>(((dif_config_ptr->flags >> 16) & 0xFF) | dif_config_ptr->block_size)),
        dml::detail::ml::dif_destination_options(static_cast<uint8_t>((dif_config_ptr->flags >> 8) & 0xFF)));
    auto status = dml::to_own_status(dml::detail::ml::validate(operation));
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    dml::batch(dml_job_ptr->destination_first_ptr, task_count)
        .add_by_index(task_index,
                      [&]
                      {
                          return operation;
                      });

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_batch_set_dif_strip_by_index(dml_job_t              *dml_job_ptr,
                                                         uint32_t                task_index,
                                                         uint8_t                *source_ptr,
                                                         uint32_t                source_length,
                                                         const dml_dif_config_t *dif_config_ptr,
                                                         uint8_t                *destination_ptr,
                                                         uint32_t                destination_length,
                                                         dml_operation_flags_t   flags)
{
    static_cast<void>(destination_length);

    CHECK_NULL(dml_job_ptr);
    CHECK_NULL(dml_job_ptr->destination_length);
    CHECK_NULL(source_ptr);
    CHECK_NULL(dif_config_ptr);
    CHECK_NULL(destination_ptr);

    const auto task_count = dml_job_ptr->destination_length / dml::get_task_size();
    if (task_index >= task_count)
    {
        return DML_STATUS_BATCH_TASK_INDEX_OVERFLOW;
    }

    auto operation = dml::detail::ml::make_dif_strip_operation(
        source_ptr,
        destination_ptr,
        source_length,
        { dif_config_ptr->source_reference_tag_seed,
          dif_config_ptr->source_application_tag_mask,
          dif_config_ptr->source_application_tag_seed },
        dml::detail::ml::dif_strip_options(static_cast<uint16_t>(flags & 0xFFFF)),
        dml::detail::ml::dif_specific_options(static_cast<uint8_t>(((dif_config_ptr->flags >> 16) & 0xFF) | dif_config_ptr->block_size)),
        dml::detail::ml::dif_source_options(static_cast<uint8_t>(dif_config_ptr->flags & 0xFF)));

    auto status = dml::to_own_status(dml::detail::ml::validate(operation));
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    dml::batch(dml_job_ptr->destination_first_ptr, task_count)
        .add_by_index(task_index,
                      [&]
                      {
                          return operation;
                      });

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_batch_get_result(const dml_job_t *dml_job_ptr, uint32_t task_index, dml_meta_result_t *result_ptr)
{
    CHECK_NULL(dml_job_ptr);
    CHECK_NULL(result_ptr);

    const auto task_count = dml_job_ptr->destination_length / dml::get_task_size();
    if (task_index >= task_count)
    {
        return DML_STATUS_BATCH_TASK_INDEX_OVERFLOW;
    }

    *result_ptr = dml::batch(dml_job_ptr->destination_first_ptr, task_count).get_status(task_index);

    return DML_STATUS_OK;
}

extern "C" dml_status_t dml_batch_get_status(const dml_job_t *dml_job_ptr, uint32_t task_index, dml_status_t *status_ptr)
{
    CHECK_NULL(dml_job_ptr);
    CHECK_NULL(status_ptr);

    const auto task_count = dml_job_ptr->destination_length / dml::get_task_size();
    if (task_index >= task_count)
    {
        return DML_STATUS_BATCH_TASK_INDEX_OVERFLOW;
    }

    *status_ptr = ::dml::batch(dml_job_ptr->destination_first_ptr, task_count).get_status(task_index);

    return DML_STATUS_OK;
}
