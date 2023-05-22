/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_COMPARE_WORKLOAD_HPP
#define DML_TESTING_COMPARE_WORKLOAD_HPP

#include <utils/memory.hpp>
#include <utils/operation.hpp>
#include <utils/workload.hpp>
#include <utils/workload_builder.hpp>

namespace dml::testing
{
    template <>
    class Workload<CompareOperation>: public WorkloadBase
    {
    public:
        Workload() = delete;

        [[nodiscard]] auto& get_src1() noexcept
        {
            return memory_.get_region(0);
        }

        [[nodiscard]] auto& get_src2() noexcept
        {
            return memory_.get_region(1);
        }

        [[nodiscard]] auto get_expected_result() const noexcept
        {
            return expected_result_;
        }

        [[nodiscard]] auto check_result_enabled() const noexcept
        {
            return check_result_;
        }

        [[nodiscard]] bool block_on_fault_enabled() noexcept
        {
            return block_on_fault_;
        }

        bool operator==(const Workload& other) const noexcept
        {
            return this->memory_ == other.memory_;
        }

    private:
        friend WorkloadBuilder<CompareOperation>;

        Workload(Memory&& memory, std::uint8_t expected_result, bool check_result, bool block_on_fault = false) noexcept:
            WorkloadBase(std::move(memory)),
            expected_result_(expected_result),
            check_result_(check_result),
            block_on_fault_(block_on_fault)
        {
        }

    private:
        std::uint8_t expected_result_;
        bool         check_result_;
        bool         block_on_fault_;
    };
}  // namespace dml::testing

#endif  //DML_TESTING_COMPARE_WORKLOAD_HPP
