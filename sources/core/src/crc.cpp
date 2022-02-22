/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <core/utils.hpp>
#include <dml/detail/common/specific_flags.hpp>
#include <dml/detail/common/status.hpp>
#include <dml/detail/common/utils/enum.hpp>
#include <optimization_dispatcher.hpp>

#include "immintrin.h"
#include "kernels.hpp"

namespace dml::core::kernels
{
    void crc(const_view<descriptor, operation::crc> dsc) noexcept
    {
        auto record = make_view<operation::crc>(get_completion_record(dsc));

        const auto src           = reinterpret_cast<byte_t *>(dsc.source_address());
        const auto transfer_size = dsc.transfer_size();
        const auto crc_seed      = dsc.crc_seed();
        const auto bypass_reflection =
            intersects(dsc.operation_specific_flags(), dml::detail::crc_specific_flag::bypass_crc_inversion_and_reflection);
        const auto bypass_data_reflection =
            intersects(dsc.operation_specific_flags(), dml::detail::crc_specific_flag::bypass_data_reflection);

        auto reverse = [](uint32_t value)
        {
            value = (value & 0x55555555u) << 1u | (value & 0xAAAAAAAAu) >> 1u;
            value = (value & 0x33333333u) << 2u | (value & 0xCCCCCCCCu) >> 2u;
            value = (value & 0x0F0F0F0Fu) << 4u | (value & 0xF0F0F0F0u) >> 4u;
            value = (value & 0x00FF00FFu) << 8u | (value & 0xFF00FF00u) >> 8u;
            value = (value & 0x0000FFFFu) << 16u | (value & 0xFFFF0000u) >> 16u;

            return value;
        };

        auto crc_value = crc_seed;

        // Bypass inversion and use reverse bit order for CRC completion_record
        if (!bypass_reflection)
        {
            crc_value = ~(crc_value);
            crc_value = reverse(crc_value);
        }

        // Bypass Data Reflection in case if DML_FLAG_DATA_REFLECTION set
        crc_value =
            !bypass_data_reflection ? dispatch::crc_reflected(src, transfer_size, crc_value) : dispatch::crc(src, transfer_size, crc_value);

        // Bypass inversion and use reverse bit order for CRC completion_record
        if (!bypass_reflection)
        {
            crc_value = reverse(crc_value);
            crc_value = ~(crc_value);
        }

        record.crc_value() = crc_value;

        _mm_mfence();
        record.status() = to_underlying(dml::detail::execution_status::success);
    }
}  // namespace dml::core::kernels
