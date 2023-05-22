/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_COMPARE_PATTERN_WORKLOAD_BUILDER_HPP
#define DML_TESTING_COMPARE_PATTERN_WORKLOAD_BUILDER_HPP

#include <utils/compare_pattern/workload.hpp>
#include <utils/memory.hpp>
#include <utils/operation.hpp>

namespace dml::testing
{
    enum class expect_e
    {
        none,
        equal,
        not_equal
    };

    static inline std::ostream& operator<<(std::ostream& ostream, expect_e check);

    enum class mismatch_e
    {
        none,
        first,
        middle,
        last
    };

    static inline std::ostream& operator<<(std::ostream& ostream, mismatch_e check);

    enum class block_on_fault_e
    {
        block,
        dont_block
    };

    static inline std::ostream& operator<<(std::ostream& ostream, block_on_fault_e block_on_fault);

    template <>
    class WorkloadBuilder<ComparePatternOperation>
    {
    public:
        WorkloadBuilder() noexcept:
            transfer_size_(1),
            src_alignment_(1),
            expected_result_(expect_e::none),
            mismatch_(mismatch_e::none),
            block_on_fault_(block_on_fault_e::dont_block)
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

        auto& set_expected_result(expect_e expect) noexcept
        {
            expected_result_ = expect;

            return *this;
        }

        auto& set_mismatch(mismatch_e mismatch) noexcept
        {
            mismatch_ = mismatch;

            return *this;
        }

        auto& set_block_on_fault(block_on_fault_e block_on_fault) noexcept
        {
            block_on_fault_ = block_on_fault;

            return *this;
        }

        [[nodiscard]] auto build()
        {
            auto memory = MemoryBuilder()
                              .add_region(RegionConfiguration()
                                              .set_size(transfer_size_)
                                              .set_alignment(src_alignment_))
                              .build();

            // Little-endian
            std::uint8_t pattern[8] = { 0xF8u, 0xF7, 0xF6, 0xF5, 0xF4, 0xF3, 0xF2, 0xF1 };
            auto i = 0u;
            for (auto& byte : memory.get_region(0))
            {
                byte = pattern[i];

                i = (i + 1u) % 8u;
            }

            if (mismatch_ == mismatch_e::first)
            {
                memory.get_region(0).data()[0] = ~memory.get_region(0).data()[0];
            }
            else if (mismatch_ == mismatch_e::middle)
            {
                memory.get_region(0).data()[transfer_size_ / 2u] = ~memory.get_region(0).data()[0];
            }
            else if (mismatch_ == mismatch_e::last)
            {
                memory.get_region(0).data()[transfer_size_ - 1] = ~memory.get_region(0).data()[0];
            }

            return Workload<ComparePatternOperation>(
                std::move(memory),
                0xF1F2F3F4F5F6F7F8u,
                expected_result_ == expect_e::not_equal ? 1 : 0,
                expected_result_ != expect_e::none,
                block_on_fault_ == block_on_fault_e::block);
        }

    private:
        std::uint32_t     transfer_size_;
        std::uint32_t     src_alignment_;
        expect_e          expected_result_;
        mismatch_e        mismatch_;
        block_on_fault_e  block_on_fault_;
        
    };

    static inline std::ostream& operator<<(std::ostream& ostream, expect_e check)
    {
        if (check == expect_e::none)
        {
            return ostream << "expect_none";
        }
        else if (check == expect_e::equal)
        {
            return ostream << "expect_equal";
        }
        else if (check == expect_e::not_equal)
        {
            return ostream << "expect_not_equal";
        }
        else
        {
            throw std::logic_error("Unexpected enumeration");
        }
    }

    static inline std::ostream& operator<<(std::ostream& ostream, mismatch_e check)
    {
        if (check == mismatch_e::none)
        {
            return ostream << "mismatch_none";
        }
        if (check == mismatch_e::first)
        {
            return ostream << "mismatch_first";
        }
        else if (check == mismatch_e::middle)
        {
            return ostream << "mismatch_middle";
        }
        else if (check == mismatch_e::last)
        {
            return ostream << "mismatch_last";
        }
        else
        {
            throw std::logic_error("Unexpected enumeration");
        }
    }

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

#endif  //DML_TESTING_COMPARE_PATTERN_WORKLOAD_BUILDER_HPP
