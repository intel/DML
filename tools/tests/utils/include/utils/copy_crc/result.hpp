/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_COPY_CRC_RESULT_HPP
#define DML_TESTING_COPY_CRC_RESULT_HPP

#include <utils/operation.hpp>
#include <utils/result.hpp>
#include <utils/status.hpp>
#include <utils/result_builder.hpp>

namespace dml::testing
{
    template <>
    class Result<CopyCrcOperation>
    {
    public:
        Result() = delete;

        bool operator==(const Result<CopyCrcOperation>& rhs) const
        {
            return std::tie(status_, crc_value_) == std::tie(rhs.status_, rhs.crc_value_);
        }

        bool operator==(Status status) const noexcept
        {
            return this->status_ == status;
        }

        friend std::ostream &operator<<(std::ostream &os, const Result<CopyCrcOperation> &result)
        {
            os << "status: " << result.status_ << " | ";
            os << "crc: " << result.crc_value_;
            return os;
        }

    private:
        friend ResultBuilder<CopyCrcOperation>;

        explicit Result(Status status, std::uint32_t crc_value) noexcept:
            status_(status),
            crc_value_(crc_value)
        {
        }

    private:
        Status  status_;
        std::uint32_t crc_value_;
    };
}  // namespace dml::testing

#endif  //DML_TESTING_COPY_CRC_RESULT_HPP
