/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_CRC_REFERENCE_HPP
#define DML_TESTING_CRC_REFERENCE_HPP

#include <utils/crc/result_builder.hpp>
#include <utils/crc/workload.hpp>

namespace dml::testing
{
    inline auto reverse(std::uint8_t value)
    {
        value = ((value & 0x55u) << 1u) | ((value & 0xAAu) >> 1u);
        value = ((value & 0x33u) << 2u) | ((value & 0xCCu) >> 2u);
        value = ((value & 0x0Fu) << 4u) | ((value & 0xF0u) >> 4u);

        return value;
    }

    inline auto reverse(std::uint32_t value)
    {
        value = (value & 0x55555555u) << 1u | (value & 0xAAAAAAAAu) >> 1u;
        value = (value & 0x33333333u) << 2u | (value & 0xCCCCCCCCu) >> 2u;
        value = (value & 0x0F0F0F0Fu) << 4u | (value & 0xF0F0F0F0u) >> 4u;
        value = (value & 0x00FF00FFu) << 8u | (value & 0xFF00FF00u) >> 8u;
        value = (value & 0x0000FFFFu) << 16u | (value & 0xFFFF0000u) >> 16u;

        return value;
    }

    inline auto ReferenceImplementation(Workload<CrcOperation>& workload) noexcept
    {
        constexpr auto crc_shift  = 24u;
        constexpr auto high_bit   = 0x80000000u;
        constexpr auto polynomial = 0x1EDC6F41u;

        auto crc_value = workload.get_crc_seed();

        if (!workload.bypass_reflection_enabled())
        {
            crc_value = (~crc_value);
            crc_value = reverse(crc_value);
        }

        for (auto byte : workload.get_src())
        {
            crc_value ^= uint32_t((workload.bypass_data_reflection_enabled() ? byte : reverse(byte)) << crc_shift);

            for (auto bit = 0u; bit < 8u; ++bit)
            {
                crc_value =
                    (crc_value & high_bit) ? ((crc_value << 1u) ^ polynomial) : (crc_value << 1u);
            }
        }

        if (!workload.bypass_reflection_enabled())
        {
            crc_value = (~crc_value);
            crc_value = reverse(crc_value);
        }

        return ResultBuilder<CrcOperation>().set_status(StatusCode::Success).set_crc_value(crc_value).build();
    }
}  // namespace dml::testing

#endif  //DML_TESTING_CRC_REFERENCE_HPP
