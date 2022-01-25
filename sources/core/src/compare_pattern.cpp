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

#include <core/utils.hpp>
#include <dml/detail/common/flags.hpp>
#include <dml/detail/common/specific_flags.hpp>
#include <dml/detail/common/status.hpp>
#include <dml/detail/common/utils/enum.hpp>
#include <optimization_dispatcher.hpp>
#include <tuple>

#include "immintrin.h"
#include "kernels.hpp"

namespace dml::core::kernels
{
    void compare_pattern(const_view<descriptor, operation::compare_pattern> dsc) noexcept
    {
        auto record = make_view<operation::compare_pattern>(get_completion_record(dsc));

        const auto pattern         = dsc.pattern();
        const auto src             = reinterpret_cast<byte_t *>(dsc.source_address());
        const auto transfer_size   = dsc.transfer_size();
        const auto expected_result = dsc.expected_result();
        const auto check_result    = intersects(dsc.flags(), dml::detail::compare_flag::check_result);

        std::tie(record.bytes_completed(), record.result()) = dispatch::compare_pattern(pattern, src, transfer_size);

        _mm_mfence();
        record.status() =
            to_underlying(check_result ? (expected_result == record.result()) ? dml::detail::execution_status::success
                                                                              : dml::detail::execution_status::false_predicate_success
                                       : dml::detail::execution_status::success);
    }
}  // namespace dml::core::kernels
