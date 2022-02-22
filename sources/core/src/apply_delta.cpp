/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <core/utils.hpp>
#include <dml/detail/common/status.hpp>
#include <dml/detail/common/utils/enum.hpp>
#include <optimization_dispatcher.hpp>

#include "immintrin.h"
#include "kernels.hpp"

namespace dml::core::kernels
{
    void apply_delta(const_view<descriptor, operation::apply_delta> dsc) noexcept
    {
        auto record = make_view<operation::apply_delta>(get_completion_record(dsc));

        const auto dst               = reinterpret_cast<byte_t *>(dsc.destination_address());
        const auto delta_record      = reinterpret_cast<byte_t *>(dsc.delta_record_address());
        const auto delta_record_size = dsc.delta_record_size();

        dispatch::apply_delta(delta_record, dst, delta_record_size);

        _mm_mfence();
        record.status() = to_underlying(dml::detail::execution_status::success);
    }
}  // namespace dml::core::kernels
