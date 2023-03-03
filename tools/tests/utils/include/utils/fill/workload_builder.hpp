/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_FILL_WORKLOAD_BUILDER_HPP
#define DML_TESTING_FILL_WORKLOAD_BUILDER_HPP

#include <utils/fill/workload.hpp>
#include <utils/memory.hpp>
#include <utils/operation.hpp>

namespace dml::testing
{
    template <>
    class WorkloadBuilder<FillOperation>
    {
    public:
        WorkloadBuilder() noexcept:
            transfer_size_(1),
            dst_alignment_(1),
            pattern_(0xF1F2F3F4F5F6F7F8u)
        {
        }

        auto& set_transfer_size(std::uint32_t size) noexcept
        {
            transfer_size_ = size;

            return *this;
        }

        auto& set_dst_alignment(std::uint32_t size) noexcept
        {
            dst_alignment_ = size;

            return *this;
        }

        [[nodiscard]] auto build()
        {
            return Workload<FillOperation>(MemoryBuilder()
                                               .add_region(RegionConfiguration()
                                                               .set_size(transfer_size_)
                                                               .set_alignment(dst_alignment_))
                                               .build(),
                                           pattern_);
        }

    private:
        std::uint32_t transfer_size_;
        std::uint32_t dst_alignment_;
        uint64_t      pattern_;
    };
}  // namespace dml::testing

#endif  //DML_TESTING_FILL_WORKLOAD_BUILDER_HPP
