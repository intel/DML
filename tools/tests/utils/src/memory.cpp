/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <iterator>
#include <stdexcept>
#include <utils/memory.hpp>

namespace dml::testing
{
    namespace detail
    {
        static inline std::uint32_t difference(std::uint32_t lhs, std::uint32_t rhs) noexcept
        {
            // Promote to 8-bytes signed type to prevent overflow
            return std::uint32_t(std::abs(std::int64_t(lhs) - std::int64_t(rhs)));
        }

        static inline std::uint32_t difference(std::int32_t lhs, std::int32_t rhs) noexcept
        {
            // Promote to 8-bytes signed type to prevent overflow
            return std::uint32_t(std::abs(std::int64_t(lhs) - std::int64_t(rhs)));
        }
    }  // namespace detail

    Memory MemoryBuilder::build() const
    {
        // This function calculates begin pointer and size for each region (offset and size)
        // Then creates mapping (Regions -> Memory)
        using RegionParams = struct
        {
            std::uint32_t offset;
            std::uint32_t size;
        };

        for (auto i = 1u; i < configurations_.size(); ++i)
        {
            if (configurations_[i].distance_ != 0u &&
                configurations_[i].alignment_ != configurations_[i - 1u].alignment_)
            {
                throw std::logic_error("Distance is allowed only for regions with same alignment");
            }
        }

        const auto max_align = std::max_element(configurations_.begin(),
                                                configurations_.end(),
                                                [](const auto& lhs, const auto& rhs)
                                                {
                                                    return lhs.alignment_ < rhs.alignment_;
                                                })
                                   ->alignment_;

        auto regions_params = std::vector<RegionParams>();

        std::transform(
            configurations_.begin(),
            configurations_.end(),
            std::back_inserter(regions_params),
            [max_align, prev_size = 0u, prev_offset = 0u, prev_alignment = max_align](
                const auto region) mutable
            {
                auto params = RegionParams{};
                params.size = region.size_;

                if (region.distance_ == 0)
                {
                    params.offset =
                        prev_offset + detail::difference(prev_alignment, region.alignment_);
                }
                else
                {
                    params.offset = (prev_offset - prev_size) + region.distance_;
                }

                // Promote a type or demote alignment
                auto should_offset = region.offset_ * static_cast<int32_t>(region.alignment_);

                if ((should_offset < 0) && detail::difference(static_cast<int32_t>(params.offset),
                                                              should_offset) > params.offset)
                {
                    params.offset = 0;
                }
                else
                {
                    params.offset += should_offset;
                }

                prev_offset = params.offset + params.size;
                prev_size   = params.size;

                for (auto align = max_align; align != 0; align >>= 1)
                {
                    const auto address = prev_alignment + prev_offset;
                    if (address % align == 0)
                    {
                        prev_alignment = align;
                        break;
                    }
                }

                return params;
            });

        const auto highest_region =
            std::max_element(regions_params.begin(),
                             regions_params.end(),
                             [](const auto& lhs, const auto& rhs)
                             {
                                 return (lhs.offset + lhs.size) < (rhs.offset + rhs.size);
                             });

        const auto max_size = highest_region->offset + highest_region->size;

        auto memory = RawMemory(max_size, max_align);

        auto regions = std::vector<MemoryRegion>();
        std::transform(regions_params.begin(),
                       regions_params.end(),
                       std::back_inserter(regions),
                       [&](const auto& region)
                       {
                           return MemoryRegion(
                               reinterpret_cast<uint8_t*>(memory.data()) + region.offset,
                               region.size);
                       });

        return Memory(std::move(memory), std::move(regions));
    }
}  // namespace dml::testing
