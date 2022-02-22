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

#include "immintrin.h"
#include "kernels.hpp"

namespace dml::core::kernels
{
    void cache_flush(const_view<descriptor, operation::cache_flush> dsc) noexcept
    {
        auto record = make_view<operation::cache_flush>(get_completion_record(dsc));

        const auto dst              = reinterpret_cast<byte_t *>(dsc.destination_address());
        const auto transfer_size    = dsc.transfer_size();
        const auto invalidate_cache = intersects(dsc.flags(), dml::detail::cache_flush_flag::cache_control);

        if (invalidate_cache)
            dispatch::cache_flush(dst, transfer_size);
        else
            dispatch::cache_write_back(dst, transfer_size);

        _mm_mfence();
        record.status() = to_underlying(dml::detail::execution_status::success);
    }
}  // namespace dml::core::kernels
