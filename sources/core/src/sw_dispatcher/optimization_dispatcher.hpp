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

#ifndef DML_CORE_OWN_KERNELS_OPTIMIZATION_DISPATCHER_HPP
#define DML_CORE_OWN_KERNELS_OPTIMIZATION_DISPATCHER_HPP

#include <cstdint>
#include <tuple>

namespace dml::core::dispatch
{
    void mem_move(const uint8_t* src, uint8_t* dst, uint32_t transfer_size) noexcept;

    void fill(uint64_t pattern, uint8_t* dst, uint32_t transfer_size) noexcept;

    std::tuple<uint32_t, uint8_t> compare(const uint8_t* src1, const uint8_t* src2, uint32_t transfer_size) noexcept;

    std::tuple<uint32_t, uint8_t> compare_pattern(uint64_t pattern, const uint8_t* src, uint32_t transfer_size) noexcept;

    std::tuple<uint32_t, uint8_t> create_delta(const uint8_t* src1,
                                               const uint8_t* src2,
                                               uint32_t       transfer_size,
                                               uint8_t*       delta_record,
                                               uint32_t       delta_max_size) noexcept;

    void apply_delta(const uint8_t* delta_record, uint8_t* dst, uint32_t transfer_size) noexcept;

    void dualcast(const uint8_t* src, uint8_t* dst1, uint8_t* dst2, uint32_t transfer_size) noexcept;

    uint32_t crc(const uint8_t* src, uint32_t transfer_size, uint32_t crc_seed, uint32_t polynomial = 0x1EDC6F41u) noexcept;

    uint32_t crc_reflected(const uint8_t* src, uint32_t transfer_size, uint32_t crc_seed, uint32_t polynomial = 0x1EDC6F41u) noexcept;

    void cache_flush(uint8_t* dst, uint32_t transfer_size) noexcept;

    void cache_write_back(uint8_t* dst, uint32_t transfer_size) noexcept;
}  // namespace dml::core::dispatch

#endif  //DML_CORE_OWN_KERNELS_OPTIMIZATION_DISPATCHER_HPP
