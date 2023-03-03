/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_COMPARE_RESULT_HPP
#define DML_TESTING_COMPARE_RESULT_HPP

#include <utils/operation.hpp>
#include <utils/result.hpp>
#include <utils/status.hpp>
#include <utils/result_builder.hpp>

namespace dml::testing
{
    template <>
    class Result<CompareOperation>
    {
    public:
        Result() = delete;

        bool operator==(const Result<CompareOperation>& rhs) const
        {
            return std::tie(status_, result_, mismatch_) ==
                   std::tie(rhs.status_, rhs.result_, rhs.mismatch_);
        }

        bool operator==(Status status) const noexcept
        {
            return this->status_ == status;
        }

        friend std::ostream &operator<<(std::ostream &os, const Result<CompareOperation> &result)
        {
            os << "status: " << result.status_ << " | ";
            os << "result: " << static_cast<int>(result.result_) << " | ";
            os << "mismatch: " << result.mismatch_;
            return os;
        }

    private:
        friend ResultBuilder<CompareOperation>;

        explicit Result(Status status,
                        std::uint8_t  result,
                        std::uint32_t mismatch) noexcept:
            status_(status),
            result_(result),
            mismatch_(mismatch)
        {
        }

    private:
        Status  status_;
        std::uint8_t  result_;
        std::uint32_t mismatch_;
    };
}  // namespace dml::testing

#endif  //DML_TESTING_COMPARE_RESULT_HPP
