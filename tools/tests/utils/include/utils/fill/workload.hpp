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

        bool operator==(const Workload& other) const noexcept
        {
            return this->memory_ == other.memory_;
        }

    private:
        friend WorkloadBuilder<FillOperation>;

        Workload(Memory&& memory, std::uint64_t pattern) noexcept:
            WorkloadBase(std::move(memory)),
            pattern_(pattern)
        {
        }

    private:
        std::uint64_t pattern_;
    };
}  // namespace dml::testing

#endif  //DML_TESTING_FILL_WORKLOAD_HPP
