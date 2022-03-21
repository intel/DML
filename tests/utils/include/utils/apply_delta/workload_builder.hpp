/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_APPLY_DELTA_WORKLOAD_BUILDER_HPP
#define DML_TESTING_APPLY_DELTA_WORKLOAD_BUILDER_HPP

#include <utils/apply_delta/workload.hpp>
#include <utils/memory.hpp>
#include <utils/operation.hpp>

namespace dml::testing
{
    enum class delta_size_e
    {
        min,
        medium,
        max
    };

    static inline std::ostream& operator<<(std::ostream& ostream, delta_size_e delta_size);

    template <>
    class WorkloadBuilder<ApplyDeltaOperation>
    {
    public:
        WorkloadBuilder() noexcept: transfer_size_(8), delta_size_(delta_size_e::min), alignment_(8u)
        {
        }

        auto& set_transfer_size(std::uint32_t size) noexcept
        {
            transfer_size_ = size;

            return *this;
        }

        auto& set_delta_size(delta_size_e delta_size) noexcept
        {
            delta_size_ = delta_size;

            return *this;
        }

        auto& set_alignment(std::uint32_t alignment) noexcept
        {
            alignment_ = alignment;

            return *this;
        }

        [[nodiscard]] auto build()
        {
            const auto required_size = (transfer_size_ / 8u) * 10u;
            const auto min_size      = (required_size < 10) ? 10 : required_size;
            const auto max_size      = (required_size > 10) ? required_size : 10;
            const auto medium_size   = min_size + ((max_size - min_size) / 2u);

            auto memory =
                MemoryBuilder()
                    .add_region(RegionConfiguration()
                                    .set_size(delta_size_ == delta_size_e::min   ? min_size
                                              : delta_size_ == delta_size_e::max ? max_size
                                                                                 : medium_size)
                                    .set_alignment(alignment_))
                    .add_region(RegionConfiguration().set_size(transfer_size_).set_alignment(alignment_))
                    .build();

            for (auto& elem : memory.get_region(0))
            {
                elem = 0;
            }

            for (auto& byte : memory.get_region(1))
            {
                byte = 0u;
            }

            for (auto i = 0u; i < memory.get_region(1).size() / sizeof(std::uint64_t); ++i)
            {
                if ((i * 10u) > memory.get_region(0).size()) break;

                auto delta = memory.get_region(0).data() + (i * 10u);

                *(reinterpret_cast<std::uint16_t *>(delta)) = std::uint16_t(i);
                *(reinterpret_cast<std::uint64_t *>(delta + 2u)) = std::uint64_t(i);
            }

            return Workload<ApplyDeltaOperation>(std::move(memory));
        }

    private:
        std::uint32_t transfer_size_;
        delta_size_e  delta_size_;
        std::uint32_t alignment_;
    };

    static inline std::ostream& operator<<(std::ostream& ostream, delta_size_e delta_size)
    {
        if (delta_size == delta_size_e::min)
        {
            return ostream << "min_delta_size";
        }
        else if (delta_size == delta_size_e::medium)
        {
            return ostream << "medium_delta_size";
        }
        else if (delta_size == delta_size_e::max)
        {
            return ostream << "max_delta_size";
        }
        else
        {
            return ostream << "unexpected_enumeration";
        }
    }
}  // namespace dml::testing

#endif  //DML_TESTING_APPLY_DELTA_WORKLOAD_BUILDER_HPP
