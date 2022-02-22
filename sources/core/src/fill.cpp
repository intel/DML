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
    void fill(const_view<descriptor, operation::fill> dsc) noexcept
    {
        auto record = make_view<operation::fill>(get_completion_record(dsc));

        const auto pattern       = dsc.pattern();
        const auto dst           = reinterpret_cast<byte_t *>(dsc.destination_address());
        const auto transfer_size = dsc.transfer_size();

        dispatch::fill(pattern, dst, transfer_size);

        _mm_mfence();
        record.status() = to_underlying(dml::detail::execution_status::success);
    }
}  // namespace dml::core::kernels
