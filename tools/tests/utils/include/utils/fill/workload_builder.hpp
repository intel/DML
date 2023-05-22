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
    enum class block_on_fault_e
    {
        block,
        dont_block
    };

    static inline std::ostream& operator<<(std::ostream& ostream, block_on_fault_e block_on_fault);

    template <>
    class WorkloadBuilder<FillOperation>
    {
    public:
        WorkloadBuilder() noexcept:
            transfer_size_(1),
            dst_alignment_(1),
            pattern_(0xF1F2F3F4F5F6F7F8u),
            block_on_fault_(block_on_fault_e::dont_block)
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

        auto& set_block_on_fault(block_on_fault_e block_on_fault) noexcept
        {
            block_on_fault_ = block_on_fault;

            return *this;
        }

        [[nodiscard]] auto build()
        {
            return Workload<FillOperation>(MemoryBuilder()
                                               .add_region(RegionConfiguration()
                                                               .set_size(transfer_size_)
                                                               .set_alignment(dst_alignment_))
                                               .build(),
                                           pattern_,
                                           block_on_fault_ == block_on_fault_e::block);
        }

    private:
        std::uint32_t     transfer_size_;
        std::uint32_t     dst_alignment_;
        uint64_t          pattern_;
        block_on_fault_e  block_on_fault_;
    };

    static inline std::ostream& operator<<(std::ostream& ostream, block_on_fault_e block_on_fault)
    {
        if (block_on_fault == block_on_fault_e::block)
        {
            return ostream << "block_on_fault";
        }
        else if (block_on_fault == block_on_fault_e::dont_block)
        {
            return ostream << "dont_block_on_fault";
        }
        else
        {
            throw std::logic_error("Unexpected enumeration");
        }
    }
}  // namespace dml::testing

#endif  //DML_TESTING_FILL_WORKLOAD_BUILDER_HPP
