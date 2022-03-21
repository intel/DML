/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_COMPARE_RESULT_BUILDER_HPP
#define DML_TESTING_COMPARE_RESULT_BUILDER_HPP

#include <utils/compare/result.hpp>
#include <utils/operation.hpp>
#include <utils/status.hpp>
#include <utils/workload_builder.hpp>

namespace dml::testing
{
    template <>
    class ResultBuilder<CompareOperation>
    {
    public:
        ResultBuilder() noexcept: status_(StatusCode::Unknown), result_(0), mismatch_(0)
        {
        }

        auto&set_status(Status status) noexcept
        {
            status_ = status;

            return *this;
        }

        auto& set_result(std::uint8_t result)
        {
            result_ = result;

            return *this;
        }

        auto& set_mismatch(std::uint32_t mismatch)
        {
            mismatch_ = mismatch;

            return *this;
        }

        [[nodiscard]] auto build() const noexcept
        {
            return Result<CompareOperation>(status_, result_, mismatch_);
        }

    private:
        Status  status_;
        std::uint8_t  result_;
        std::uint32_t mismatch_;
    };
}  // namespace dml::testing

#endif  //DML_TESTING_COMPARE_RESULT_BUILDER_HPP
