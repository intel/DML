/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_FILL_RESULT_BUILDER_HPP
#define DML_TESTING_FILL_RESULT_BUILDER_HPP

#include <utils/fill/result.hpp>
#include <utils/operation.hpp>
#include <utils/status.hpp>
#include <utils/workload_builder.hpp>

namespace dml::testing
{
    template <>
    class ResultBuilder<FillOperation>
    {
    public:
        ResultBuilder() noexcept: status_(StatusCode::Unknown)
        {
        }

        auto& set_status(Status status) noexcept
        {
            status_ = status;

            return *this;
        }

        [[nodiscard]] auto build() const noexcept
        {
            return Result<FillOperation>(status_);
        }

    private:
        Status status_;
    };
}  // namespace dml::testing

#endif  //DML_TESTING_FILL_RESULT_BUILDER_HPP
