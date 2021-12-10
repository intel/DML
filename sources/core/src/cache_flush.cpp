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

#include <dml/detail/common/flags.hpp>
#include <dml/detail/common/status.hpp>
#include <dml/detail/common/utils/enum.hpp>
#include <optimization_dispatcher.hpp>

#include "immintrin.h"
#include "kernels.hpp"

namespace dml::core::kernels
{
    void cache_flush(cache_flush_descriptor dsc, cache_flush_completion_record record) noexcept
    {
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
