/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_MEM_MOVE_RESULT_HPP
#define DML_TESTING_MEM_MOVE_RESULT_HPP

#include <utils/operation.hpp>
#include <utils/status.hpp>
#include <utils/result.hpp>
#include <utils/result_builder.hpp>
#include <ostream>

namespace dml::testing
{
    template <>
    class Result<MemMoveOperation>
    {
    public:
        Result() = delete;

        bool operator==(const Result& other) const noexcept
        {
            return this->status_ == other.status_;
        }

        bool operator==(Status status) const noexcept
        {
            return this->status_ == status;
        }

        friend std::ostream &operator<<(std::ostream &os, const Result<MemMoveOperation> &result)
        {
            os << result.status_;
            return os;
        }

      private:
        friend ResultBuilder<MemMoveOperation>;

        explicit Result(Status status) noexcept: status_(status)
        {
        }

    private:
        Status status_;
    };
}  // namespace dml::testing

#endif  //DML_TESTING_MEM_MOVE_RESULT_HPP
