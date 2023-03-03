/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_CREATE_DELTA_RESULT_HPP
#define DML_TESTING_CREATE_DELTA_RESULT_HPP

#include <utils/operation.hpp>
#include <utils/status.hpp>
#include <utils/result.hpp>
#include <utils/result_builder.hpp>

namespace dml::testing
{
    template <>
    class Result<CreateDeltaOperation>
    {
    public:
        Result() = delete;

        bool operator==(const Result<CreateDeltaOperation>& rhs) const
        {
            return std::tie(status_, result_, written_size_) ==
                   std::tie(rhs.status_, rhs.result_, rhs.written_size_);
        }

        bool operator==(Status status) const noexcept
        {
            return this->status_ == status;
        }

        friend std::ostream &operator<<(std::ostream &os, const Result<CreateDeltaOperation> &result)
        {
            os << "status: " << result.status_ << " | ";
            os << "result: " << static_cast<int>(result.result_) << " | ";
            os << "size: " << result.written_size_;
            return os;
        }

    private:
        friend ResultBuilder<CreateDeltaOperation>;

        explicit Result(Status  status,
                        std::uint8_t  result,
                        std::uint32_t written_size) noexcept:
            status_(status),
            result_(result),
            written_size_(written_size)
        {
        }

    private:
        Status  status_;
        std::uint8_t  result_;
        std::uint32_t written_size_;
    };
}  // namespace dml::testing

#endif  //DML_TESTING_CREATE_DELTA_RESULT_HPP
