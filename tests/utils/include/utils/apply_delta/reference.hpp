/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_APPLY_DELTA_REFERENCE_HPP
#define DML_TESTING_APPLY_DELTA_REFERENCE_HPP

#include <cstring>
#include <utils/apply_delta/result_builder.hpp>
#include <utils/apply_delta/workload.hpp>

namespace dml::testing
{
    inline auto ReferenceImplementation(Workload<ApplyDeltaOperation>& workload) noexcept
    {
        auto dst        = reinterpret_cast<std::uint64_t*>(workload.get_dst().data());
        auto delta      = workload.get_delta().data();
        auto delta_size = workload.get_delta().size();

        for (auto i = 0u; i < delta_size / 10u; ++i)
        {
            const auto offset = *reinterpret_cast<std::uint16_t*>(delta + (i * 10u));
            const auto data   = *reinterpret_cast<std::uint64_t*>(delta + (i * 10u) + 2);
            dst[offset]       = data;
        }

        return ResultBuilder<ApplyDeltaOperation>().set_status(StatusCode::Success).build();
    }
}  // namespace dml::testing

#endif  //DML_TESTING_APPLY_DELTA_REFERENCE_HPP
