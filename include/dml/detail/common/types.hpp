/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_DETAIL_COMMON_TYPES_HPP
#define DML_DETAIL_COMMON_TYPES_HPP

#include <cstddef>
#include <cstdint>

namespace dml::detail
{
    using byte_t = std::uint8_t;

    struct alignas(64u) descriptor
    {
        descriptor() noexcept : bytes()
        {
            for (auto& byte : bytes)
            {
                byte = 0u;
            }
        }

        byte_t bytes[64];
    };

    struct alignas(32) completion_record
    {
        completion_record() noexcept : bytes()
        {
            for (auto& byte : bytes)
            {
                byte = 0u;
            }
        }

        byte_t bytes[32];
    };

    using size_t = std::size_t;

    using transfer_size_t = std::uint32_t;

    using operation_t = std::uint8_t;

    using status_t = std::uint8_t;

    using flags_t = std::uint16_t;

    using operation_specific_flags_t = std::uint8_t;

    using completion_interrupt_handle_t = std::uint16_t;

    using transfer_size_t = std::uint32_t;

    using address_t = uintptr_t;

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
