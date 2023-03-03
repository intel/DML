/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_APPLY_DELTA_RESULT_BUILDER_HPP
#define DML_TESTING_APPLY_DELTA_RESULT_BUILDER_HPP

#include <utils/apply_delta/result.hpp>
#include <utils/operation.hpp>
#include <utils/status.hpp>
#include <utils/workload_builder.hpp>

namespace dml::testing
{
    template <>
    class ResultBuilder<ApplyDeltaOperation>
    {
    public:
        ResultBuilder() noexcept: status_(StatusCode::Unknown)
        {
        }

        auto&set_status(Status status) noexcept
        {
            status_ = status;

            return *this;
        }

        [[nodiscard]] auto build() const noexcept
        {
            return Result<ApplyDeltaOperation>(status_);
        }

    private:
        Status status_;
    };
}  // namespace dml::testing

#endif  //DML_TESTING_APPLY_DELTA_RESULT_BUILDER_HPP
