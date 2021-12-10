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

#include <core/completion_record_views.hpp>
#include <core/descriptor_views.hpp>
#include <core/operations.hpp>
#include <dml/detail/common/status.hpp>
#include <dml/detail/common/utils/enum.hpp>

#include "immintrin.h"
#include "kernels.hpp"

namespace dml::core::kernels
{
    void batch(batch_descriptor dsc, batch_completion_record record) noexcept
    {
        const auto operations        = reinterpret_cast<descriptor *>(dsc.descriptor_list_address());
        const auto descriptors_count = dsc.descriptors_count();

        auto status = dml::detail::execution_status::processing;
        auto index  = size_t(0);

        while (index < descriptors_count && status == dml::detail::execution_status::processing)
        {
            auto &current_dsc    = operations[index];
            auto &current_record = *reinterpret_cast<completion_record *>(any_descriptor(current_dsc).completion_record_address());

            auto op = operation(any_descriptor(current_dsc).operation());

            switch (op)
            {
                case operation::nop:
                    kernels::nop(nop_descriptor(current_dsc), nop_completion_record(current_record));
                    break;
                case operation::memory_move:
                    kernels::mem_move(mem_move_descriptor(current_dsc), mem_move_completion_record(current_record));
                    break;
                case operation::fill:
                    kernels::fill(fill_descriptor(current_dsc), fill_completion_record(current_record));
                    break;
                case operation::compare:
                    kernels::compare(compare_descriptor(current_dsc), compare_completion_record(current_record));
                    break;
                case operation::compare_pattern:
                    kernels::compare_pattern(compare_pattern_descriptor(current_dsc), compare_pattern_completion_record(current_record));
                    break;
                case operation::create_delta:
                    kernels::create_delta(create_delta_descriptor(current_dsc), create_delta_completion_record(current_record));
                    break;
                case operation::apply_delta:
                    kernels::apply_delta(apply_delta_descriptor(current_dsc), apply_delta_completion_record(current_record));
                    break;
                case operation::dualcast:
                    kernels::dualcast(dualcast_descriptor(current_dsc), dualcast_completion_record(current_record));
                    break;
                case operation::crc:
                    kernels::crc(crc_descriptor(current_dsc), crc_completion_record(current_record));
                    break;
                case operation::copy_crc:
                    kernels::copy_crc(copy_crc_descriptor(current_dsc), crc_completion_record(current_record));
                    break;
                case operation::dif_check:
                    kernels::dif_check(dif_check_descriptor(current_dsc), dif_check_completion_record(current_record));
                    break;
                case operation::dif_insert:
                    kernels::dif_insert(dif_insert_descriptor(current_dsc), dif_insert_completion_record(current_record));
                    break;
                case operation::dif_strip:
                    kernels::dif_strip(dif_strip_descriptor(current_dsc), dif_strip_completion_record(current_record));
                    break;
                case operation::dif_update:
                    kernels::dif_update(dif_update_descriptor(current_dsc), dif_update_completion_record(current_record));
                    break;
                case operation::cache_flush:
                    kernels::cache_flush(cache_flush_descriptor(current_dsc), cache_flush_completion_record(current_record));
                    break;
                default:
                    status = dml::detail::execution_status::batch_error;
            }

            if (any_completion_record(current_record).status() != to_underlying(dml::detail::execution_status::success))
            {
                status = dml::detail::execution_status::batch_error;
            }
            else
            {
                ++index;
            }
        }

        if (index == descriptors_count)
        {
            status = dml::detail::execution_status::success;
        }

        record.descriptors_completed() = static_cast<transfer_size_t>(index);

        _mm_mfence();
        record.status() = to_underlying(status);
    }
}  // namespace dml::core::kernels
