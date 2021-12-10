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

#ifndef DML_DETAIL_COMMON_TYPES_HPP
#define DML_DETAIL_COMMON_TYPES_HPP

#include <cstddef>
#include <cstdint>

namespace dml::detail
{
    using byte_t = std::uint8_t;

    using size_t = std::size_t;

    using transfer_size_t = std::uint32_t;

    using operation_t = std::uint8_t;

    using status_t = std::uint8_t;

    using flags_t = std::uint16_t;

    using operation_specific_flags_t = std::uint8_t;

    using completion_interrupt_handle_t = std::uint16_t;

    using transfer_size_t = std::uint32_t;

    using address_t = uint64_t;

    using pattern_t = uint64_t;

    using result_t = uint8_t;

    using crc_value_t = uint32_t;

    using dif_flags_t = uint8_t;

    using dif_status_t = uint8_t;

    using dif_ref_tag_t = uint32_t;

    using dif_app_tag_t = uint16_t;

    struct dif_parameters
    {
        dif_ref_tag_t ref_tag_seed;
        dif_app_tag_t app_tag_mask;
        dif_app_tag_t app_tag_seed;
    };
}  // namespace dml::detail

#endif  //DML_DETAIL_COMMON_TYPES_HPP
