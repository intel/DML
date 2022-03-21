/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_COPY_CRC_RESULT_BUILDER_HPP
#define DML_TESTING_COPY_CRC_RESULT_BUILDER_HPP

#include <utils/copy_crc/result.hpp>
#include <utils/operation.hpp>
#include <utils/status.hpp>
#include <utils/workload_builder.hpp>

namespace dml::testing
{
    template <>
    class ResultBuilder<CopyCrcOperation>
    {
    public:
        ResultBuilder() noexcept: status_(StatusCode::Unknown), crc_value_(0)
        {
        }

        auto&set_status(Status status) noexcept
        {
            status_ = status;

            return *this;
        }

        auto& set_crc_value(std::uint32_t crc_value) noexcept
        {
            crc_value_ = crc_value;

            return *this;
        }

        [[nodiscard]] auto build() const noexcept
        {
            return Result<CopyCrcOperation>(status_, crc_value_);
        }

    private:
        Status status_;
        std::uint32_t crc_value_;
    };
}  // namespace dml::testing

#endif  //DML_TESTING_COPY_CRC_RESULT_BUILDER_HPP
