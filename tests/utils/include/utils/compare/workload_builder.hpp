/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_COMPARE_WORKLOAD_BUILDER_HPP
#define DML_TESTING_COMPARE_WORKLOAD_BUILDER_HPP

#include <utils/compare/workload.hpp>
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

    template <>
    class WorkloadBuilder<CompareOperation>
    {
    public:
        WorkloadBuilder() noexcept:
            transfer_size_(1),
            src1_alignment_(1),
            src2_alignment_(1),
            expected_result_(expect_e::none),
            mismatch_(mismatch_e::none)
        {
        }

        auto& set_transfer_size(std::uint32_t size) noexcept
        {
            transfer_size_ = size;

            return *this;
        }

        auto& set_src1_alignment(std::uint32_t size) noexcept
        {
            src1_alignment_ = size;

            return *this;
        }

        auto& set_src2_alignment(std::uint32_t size) noexcept
        {
            src2_alignment_ = size;

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

        [[nodiscard]] auto build()
        {
            auto memory = MemoryBuilder()
                              .add_region(RegionConfiguration()
                                              .set_size(transfer_size_)
                                              .set_alignment(src1_alignment_))
                              .add_region(RegionConfiguration()
                                              .set_size(transfer_size_)
                                              .set_alignment(src2_alignment_))
                              .build();

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

            return Workload<CompareOperation>(std::move(memory),
                                              expected_result_ == expect_e::not_equal ? 1 : 0,
                                              expected_result_ != expect_e::none);
        }

    private:
        std::uint32_t transfer_size_;
        std::uint32_t src1_alignment_;
        std::uint32_t src2_alignment_;
        expect_e      expected_result_;
        mismatch_e    mismatch_;
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

}  // namespace dml::testing

#endif  //DML_TESTING_COMPARE_WORKLOAD_BUILDER_HPP
