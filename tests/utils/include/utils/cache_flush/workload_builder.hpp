/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_CACHE_FLUSH_WORKLOAD_BUILDER_HPP
#define DML_TESTING_CACHE_FLUSH_WORKLOAD_BUILDER_HPP

#include <utils/cache_flush/workload.hpp>
#include <utils/memory.hpp>
#include <utils/operation.hpp>

namespace dml::testing
{
    enum class cache_control_e
    {
        invalidate,
        dont_invalidate
    };

    static inline std::ostream& operator<<(std::ostream& ostream, cache_control_e cache);

    template <>
    class WorkloadBuilder<CacheFlushOperation>
    {
    public:
        WorkloadBuilder() noexcept:
            transfer_size_(1),
            dst_alignment_(1),
            cache_(cache_control_e::invalidate)
        {
        }

        auto& set_transfer_size(std::uint32_t size) noexcept
        {
            transfer_size_ = size;

            return *this;
        }

        auto& set_cache_control(cache_control_e cache) noexcept
        {
            cache_ = cache;

            return *this;
        }

        auto& set_dst_alignment(std::uint32_t size) noexcept
        {
            dst_alignment_ = size;

            return *this;
        }

        [[nodiscard]] auto build()
        {
            return Workload<CacheFlushOperation>(MemoryBuilder()
                                                     .add_region(RegionConfiguration()
                                                                     .set_size(transfer_size_)
                                                                     .set_alignment(dst_alignment_))
                                                     .build(),
                                                 cache_ == cache_control_e::dont_invalidate);
        }

    private:
        std::uint32_t   transfer_size_;
        std::uint32_t   dst_alignment_;
        cache_control_e cache_;
    };

    static inline std::ostream& operator<<(std::ostream& ostream, cache_control_e cache)
    {
        if (cache == cache_control_e::invalidate)
        {
            return ostream << "invalidate_cache";
        }
        else if (cache == cache_control_e::dont_invalidate)
        {
            return ostream << "dont_invalidate_cache";
        }
        else
        {
            return ostream << "unexpected_enumeration";
        }
    }
}  // namespace dml::testing

#endif  //DML_TESTING_CACHE_FLUSH_WORKLOAD_BUILDER_HPP
