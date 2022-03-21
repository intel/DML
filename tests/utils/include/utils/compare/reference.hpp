/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_COMPARE_REFERENCE_HPP
#define DML_TESTING_COMPARE_REFERENCE_HPP

#include <utils/compare/result_builder.hpp>
#include <utils/compare/workload.hpp>

namespace dml::testing
{
    inline auto ReferenceImplementation(Workload<CompareOperation>& workload) noexcept
    {
        auto src1_begin = workload.get_src1().begin();
        auto src1_end   = workload.get_src1().end();
        auto src2_begin = workload.get_src2().begin();
        auto src2_end   = workload.get_src2().end();

        auto [src1_mismatch, src2_mismatch] =
            std::mismatch(src1_begin, src1_end, src2_begin, src2_end);

        auto result =
            (std::tie(src1_mismatch, src2_mismatch) == std::tie(src1_end, src2_end)) ? 0u : 1u;

        auto mismatch = (result == 1u)
                            ? static_cast<std::uint32_t>(std::distance(src1_begin, src1_mismatch))
                            : 0u;

        auto status = (workload.check_result_enabled())
                          ? ((result == workload.get_expected_result()) ? StatusCode::Success : StatusCode::FalsePredicate)
                          : StatusCode::Success;

        return ResultBuilder<CompareOperation>()
            .set_status(status)
            .set_result(result)
            .set_mismatch(mismatch)
            .build();
    }
}  // namespace dml::testing

#endif  //DML_TESTING_COMPARE_REFERENCE_HPP
