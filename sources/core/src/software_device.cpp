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

#include "core/device.hpp"
#include "kernels.hpp"

namespace dml::core
{
    dml::detail::submission_status software_device::submit(descriptor& dsc, completion_record& completion_record) noexcept
    {
        auto dsc_view = any_descriptor(dsc);
        auto op       = operation(dsc_view.operation());

        switch (op)
        {
            case operation::nop:
                kernels::nop(nop_descriptor(dsc), nop_completion_record(completion_record));
                break;
            case operation::batch:
                kernels::batch(batch_descriptor(dsc), batch_completion_record(completion_record));
                break;
            case operation::drain:
                kernels::drain(drain_descriptor(dsc), drain_completion_record(completion_record));
                break;
            case operation::memory_move:
                kernels::mem_move(mem_move_descriptor(dsc), mem_move_completion_record(completion_record));
                break;
            case operation::fill:
                kernels::fill(fill_descriptor(dsc), fill_completion_record(completion_record));
                break;
            case operation::compare:
                kernels::compare(compare_descriptor(dsc), compare_completion_record(completion_record));
                break;
            case operation::compare_pattern:
                kernels::compare_pattern(compare_pattern_descriptor(dsc), compare_pattern_completion_record(completion_record));
                break;
            case operation::create_delta:
                kernels::create_delta(create_delta_descriptor(dsc), create_delta_completion_record(completion_record));
                break;
            case operation::apply_delta:
                kernels::apply_delta(apply_delta_descriptor(dsc), apply_delta_completion_record(completion_record));
                break;
            case operation::dualcast:
                kernels::dualcast(dualcast_descriptor(dsc), dualcast_completion_record(completion_record));
                break;
            case operation::crc:
                kernels::crc(crc_descriptor(dsc), crc_completion_record(completion_record));
                break;
            case operation::copy_crc:
                kernels::copy_crc(copy_crc_descriptor(dsc), crc_completion_record(completion_record));
                break;
            case operation::dif_check:
                kernels::dif_check(dif_check_descriptor(dsc), dif_check_completion_record(completion_record));
                break;
            case operation::dif_insert:
                kernels::dif_insert(dif_insert_descriptor(dsc), dif_insert_completion_record(completion_record));
                break;
            case operation::dif_strip:
                kernels::dif_strip(dif_strip_descriptor(dsc), dif_strip_completion_record(completion_record));
                break;
            case operation::dif_update:
                kernels::dif_update(dif_update_descriptor(dsc), dif_update_completion_record(completion_record));
                break;
            case operation::cache_flush:
                kernels::cache_flush(cache_flush_descriptor(dsc), cache_flush_completion_record(completion_record));
                break;
            default:
                return dml::detail::submission_status::failure;
        }

        return dml::detail::submission_status::success;
    }
}  // namespace dml::core
