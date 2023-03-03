/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_CREATE_DELTA_RESULT_BUILDER_HPP
#define DML_TESTING_CREATE_DELTA_RESULT_BUILDER_HPP

#include <utils/create_delta/result.hpp>
#include <utils/status.hpp>
#include <utils/operation.hpp>
#include <utils/workload_builder.hpp>

namespace dml::testing
{
    template <>
    class ResultBuilder<CreateDeltaOperation>
    {
    public:
        ResultBuilder() noexcept: status_(StatusCode::Unknown), result_(0), written_size_(0)
        {
        }

        auto& set_status(Status status) noexcept
        {
            status_ = status;

            return *this;
        }

        auto& set_result(std::uint8_t result) noexcept
        {
            result_ = result;

            return *this;
        }

        auto& set_written_size(std::uint32_t written_size) noexcept
        {
            written_size_ = written_size;

            return *this;
        }

        [[nodiscard]] auto build() const noexcept
        {
            return Result<CreateDeltaOperation>(status_, result_, written_size_);
        }

    private:
        Status  status_;
        std::uint8_t  result_;
        std::uint32_t written_size_;
    };
}  // namespace dml::testing

#endif  //DML_TESTING_CREATE_DELTA_RESULT_BUILDER_HPP
