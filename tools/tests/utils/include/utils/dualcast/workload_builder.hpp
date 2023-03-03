/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_DUALCAST_WORKLOAD_BUILDER_HPP
#define DML_TESTING_DUALCAST_WORKLOAD_BUILDER_HPP

#include <utils/dualcast/workload.hpp>
#include <utils/memory.hpp>
#include <utils/operation.hpp>

namespace dml::testing
{
    template <>
    class WorkloadBuilder<DualcastOperation>
    {
    public:
        WorkloadBuilder() noexcept: transfer_size_(1), src_alignment_(1), dsts_alignment_(1)
        {
        }

        auto& set_transfer_size(std::uint32_t size) noexcept
        {
            transfer_size_ = size;

            return *this;
        }

        auto& set_src_alignment(std::uint32_t size) noexcept
        {
            src_alignment_ = size;

            return *this;
        }

        auto& set_dst_alignment(std::uint32_t size) noexcept
        {
            dsts_alignment_ = size;

            return *this;
        }

        [[nodiscard]] auto build()
        {
            constexpr auto padding = 0b111111111111u + 1u;  // 12 bits
            auto distance = padding * ((transfer_size_ / padding) + 1);

            return Workload<DualcastOperation>(
                MemoryBuilder()
                    .add_region(RegionConfiguration()
                                    .set_size(transfer_size_)
                                    .set_alignment(src_alignment_))
                    .add_region(RegionConfiguration()
                                    .set_size(transfer_size_)
                                    .set_alignment(dsts_alignment_))
                    .add_region(RegionConfiguration()
                                    .set_size(transfer_size_)
                                    .set_alignment(dsts_alignment_)
                                    .set_distance(distance))
                    .build());
        }

    private:
        std::uint32_t transfer_size_;
        std::uint32_t src_alignment_;
        std::uint32_t dsts_alignment_;
    };
}  // namespace dml::testing

#endif  //DML_TESTING_DUALCAST_WORKLOAD_BUILDER_HPP
