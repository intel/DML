/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_CREATE_DELTA_WORKLOAD_HPP
#define DML_TESTING_CREATE_DELTA_WORKLOAD_HPP

#include <utils/memory.hpp>
#include <utils/operation.hpp>
#include <utils/workload.hpp>
#include <utils/workload_builder.hpp>

namespace dml::testing
{
    template <>
    class Workload<CreateDeltaOperation>: public WorkloadBase
    {
    public:
        Workload() = delete;

        auto& get_src1() noexcept
        {
            return memory_.get_region(0);
        }

        auto& get_src2() noexcept
        {
            return memory_.get_region(1);
        }

        auto& get_delta() noexcept
        {
            return memory_.get_region(2);
        }

        [[nodiscard]] bool check_result_enabled() const noexcept
        {
            return check_result_;
        }

        [[nodiscard]] std::uint8_t get_expected_result() const noexcept
        {
            return expected_result_;
        }

        bool operator==(const Workload& other) const noexcept
        {
            return this->memory_ == other.memory_;
        }

    private:
        friend WorkloadBuilder<CreateDeltaOperation>;

        Workload(Memory&& memory, bool check_result, std::uint8_t expected_result) noexcept:
            WorkloadBase(std::move(memory)),
            check_result_(check_result),
            expected_result_(expected_result)
        {
        }

    private:
        bool         check_result_;
        std::uint8_t expected_result_;
    };
}  // namespace dml::testing

#endif  //DML_TESTING_CREATE_DELTA_WORKLOAD_HPP
