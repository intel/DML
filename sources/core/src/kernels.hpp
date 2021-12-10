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

#ifndef DML_CORE_OWN_KERNELS_HPP
#define DML_CORE_OWN_KERNELS_HPP

#include <core/completion_record_views.hpp>
#include <core/descriptor_views.hpp>

namespace dml::core::kernels
{
    void nop(nop_descriptor dsc, nop_completion_record record) noexcept;

    void batch(batch_descriptor dsc, batch_completion_record record) noexcept;

    void drain(drain_descriptor dsc, drain_completion_record record) noexcept;

    void mem_move(mem_move_descriptor dsc, mem_move_completion_record record) noexcept;

    void fill(fill_descriptor dsc, fill_completion_record record) noexcept;

    void compare(compare_descriptor dsc, compare_completion_record record) noexcept;

    void compare_pattern(compare_pattern_descriptor dsc, compare_pattern_completion_record record) noexcept;

    void create_delta(create_delta_descriptor dsc, create_delta_completion_record record) noexcept;

    void apply_delta(apply_delta_descriptor dsc, apply_delta_completion_record record) noexcept;

    void dualcast(dualcast_descriptor dsc, dualcast_completion_record record) noexcept;

    void crc(crc_descriptor dsc, crc_completion_record record) noexcept;

    void copy_crc(copy_crc_descriptor dsc, crc_completion_record record) noexcept;

    void dif_check(dif_check_descriptor dsc, dif_check_completion_record record) noexcept;

    void dif_insert(dif_insert_descriptor dsc, dif_insert_completion_record record) noexcept;

    void dif_strip(dif_strip_descriptor dsc, dif_strip_completion_record record) noexcept;

    void dif_update(dif_update_descriptor dsc, dif_update_completion_record record) noexcept;

    void cache_flush(cache_flush_descriptor dsc, cache_flush_completion_record record) noexcept;
}  // namespace dml::core::kernels

#endif  //DML_CORE_OWN_KERNELS_HPP
