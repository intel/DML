/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <core/utils.hpp>
#include <dml/detail/common/flags.hpp>
#include <dml/detail/common/status.hpp>
#include <dml/detail/common/utils/enum.hpp>
#include <optimization_dispatcher.hpp>
#include <tuple>

#include "immintrin.h"
#include "kernels.hpp"

namespace dml::core::kernels
{
    void create_delta(const_view<descriptor, operation::create_delta> dsc) noexcept
    {
        auto record = make_view<operation::create_delta>(get_completion_record(dsc));

        const auto src1            = reinterpret_cast<byte_t *>(dsc.source_1_address());
        const auto src2            = reinterpret_cast<byte_t *>(dsc.source_2_address());
        const auto delta_record    = reinterpret_cast<byte_t *>(dsc.delta_record_address());
        const auto delta_max_size  = dsc.maximum_delta_record_size();
        const auto transfer_size   = dsc.transfer_size();
        const auto expected_result = dsc.expected_result_mask() & 0b111;
        const auto check_result    = intersects(dsc.flags(), dml::detail::create_delta_flag::check_result);

        std::tie(record.delta_record_size(), record.result()) =
            dispatch::create_delta(src1, src2, transfer_size, delta_record, delta_max_size);

        _mm_mfence();

        // This complexity is due to strange logic behind CreateDelta operation.
        // It uses some values to bit index magic. See the specification.
        if (check_result)
        {
            if (expected_result == 0b000)
            {
                record.status() = to_underlying(dml::detail::execution_status::false_predicate_success);
            }
            else if (expected_result == 0b111)
            {
                record.status() = to_underlying(dml::detail::execution_status::success);
            }
            else
            {
                const auto expected = (expected_result >> 1) == record.result();

                record.status() = to_underlying((expected) ? dml::detail::execution_status::success
                                                           : dml::detail::execution_status::false_predicate_success);
            }
        }
        else
        {
            record.status() = to_underlying(dml::detail::execution_status::success);
        }
    }
}  // namespace dml::core::kernels
