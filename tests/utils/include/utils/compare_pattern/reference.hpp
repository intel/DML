/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_COMPARE_PATTERN_REFERENCE_HPP
#define DML_TESTING_COMPARE_PATTERN_REFERENCE_HPP

#include <cstring>
#include <utils/compare_pattern/result_builder.hpp>
#include <utils/compare_pattern/workload.hpp>

namespace dml::testing
{
    inline auto ReferenceImplementation(Workload<ComparePatternOperation>& workload) noexcept
    {
        std::uint8_t pattern[sizeof(std::uint64_t)];
        for (auto i = 0u; i < sizeof(std::uint64_t); ++i)
        {
            pattern[i] = reinterpret_cast<std::uint8_t*>(&workload.get_pattern())[i];
        }

        auto mismatch = 0u;
        auto result = 0u;

        for (auto i = 0u; i < workload.get_src().size(); ++i)
        {
            if (workload.get_src().data()[i] != pattern[i % sizeof(std::uint64_t)])
            {
                mismatch = i;
                result = 1;
                break;
            }
        }

        auto status =
            (workload.check_result_enabled()) ? ((result == workload.get_expected_result()) ? StatusCode::Success : StatusCode::FalsePredicate) : StatusCode::Success;

        return ResultBuilder<ComparePatternOperation>()
            .set_status(status)
            .set_result(result)
            .set_mismatch(mismatch)
            .build();
    }
}  // namespace dml::testing

#endif  //DML_TESTING_COMPARE_PATTERN_REFERENCE_HPP
