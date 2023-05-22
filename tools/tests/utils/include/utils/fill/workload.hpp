/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_FILL_WORKLOAD_HPP
#define DML_TESTING_FILL_WORKLOAD_HPP

#include <utils/memory.hpp>
#include <utils/operation.hpp>
#include <utils/workload.hpp>
#include <utils/workload_builder.hpp>

namespace dml::testing
{
    template <>
    class Workload<FillOperation>: public WorkloadBase
    {
    public:
        Workload() = delete;

        auto& get_pattern() noexcept
        {
            return pattern_;
        }

        auto& get_dst() noexcept
        {
            return memory_.get_region(0);
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
        friend WorkloadBuilder<FillOperation>;

        Workload(Memory&& memory, std::uint64_t pattern, bool block_on_fault = false) noexcept:
            WorkloadBase(std::move(memory)),
            pattern_(pattern), 
            block_on_fault_(block_on_fault)
        {
        }

    private:
        std::uint64_t pattern_;
        bool          block_on_fault_;
    };
}  // namespace dml::testing

#endif  //DML_TESTING_FILL_WORKLOAD_HPP
