/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_CACHE_FLUSH_WORKLOAD_HPP
#define DML_TESTING_CACHE_FLUSH_WORKLOAD_HPP

#include <utils/memory.hpp>
#include <utils/operation.hpp>
#include <utils/workload.hpp>
#include <utils/workload_builder.hpp>

namespace dml::testing
{
    template <>
    class Workload<CacheFlushOperation>: public WorkloadBase
    {
    public:
        Workload() = delete;

        auto& get_dst() noexcept
        {
            return memory_.get_region(0);
        }

        [[nodiscard]] auto cache_control_enabled() const noexcept
        {
            return cache_control_;
        }

        bool operator==(const Workload& other) const noexcept
        {
            return this->memory_ == other.memory_;
        }

    private:
        friend WorkloadBuilder<CacheFlushOperation>;

        Workload(Memory&& memory, bool cache_control) noexcept:
            WorkloadBase(std::move(memory)),
            cache_control_(cache_control)
        {
        }

    private:
        bool   cache_control_;
    };
}  // namespace dml::testing

#endif  //DML_TESTING_CACHE_FLUSH_WORKLOAD_HPP
