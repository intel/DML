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
    void dualcast(const_view<descriptor, operation::dualcast> dsc) noexcept
    {
        auto record = make_view<operation::dualcast>(get_completion_record(dsc));

        const auto src           = reinterpret_cast<byte_t *>(dsc.source_address());
        const auto dst1          = reinterpret_cast<byte_t *>(dsc.destination_1_address());
        const auto dst2          = reinterpret_cast<byte_t *>(dsc.destination_2_address());
        const auto transfer_size = dsc.transfer_size();

        dispatch::dualcast(src, dst1, dst2, transfer_size);

        _mm_mfence();
        record.status() = to_underlying(dml::detail::execution_status::success);
    }
}  // namespace dml::core::kernels
