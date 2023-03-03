/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_MEM_MOVE_WORKLOAD_HPP
#define DML_TESTING_MEM_MOVE_WORKLOAD_HPP

#include <utils/memory.hpp>
#include <utils/operation.hpp>
#include <utils/workload.hpp>
#include <utils/workload_builder.hpp>
#include <ostream>

namespace dml::testing
{
    template <>
    class Workload<MemMoveOperation>: public WorkloadBase
    {
    public:
        Workload() = delete;

        auto& get_src() noexcept
        {
            return memory_.get_region(src_index_);
        }

        auto& get_dst() noexcept
        {
            return memory_.get_region(dst_index_);
        }

        bool operator==(const Workload& other) const noexcept
        {
            return this->memory_ == other.memory_;
        }

    private:
        friend WorkloadBuilder<MemMoveOperation>;

        Workload(Memory&& memory, std::size_t src_index, std::size_t dst_index) noexcept:
            WorkloadBase(std::move(memory)),
            src_index_(src_index),
            dst_index_(dst_index)
        {
        }

    private:
        std::size_t src_index_;
        std::size_t dst_index_;
    };
}  // namespace dml::testing

#endif  //DML_TESTING_MEM_MOVE_WORKLOAD_HPP
