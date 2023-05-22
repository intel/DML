/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_CREATE_DELTA_WORKLOAD_BUILDER_HPP
#define DML_TESTING_CREATE_DELTA_WORKLOAD_BUILDER_HPP

#include <utils/create_delta/workload.hpp>
#include <utils/memory.hpp>
#include <utils/operation.hpp>

namespace dml::testing
{
    enum class expect_e
    {
        none,
        equal,
        not_equal,
        overflow
    };

    static inline std::ostream& operator<<(std::ostream& ostream, expect_e expect);

    enum class mismatch_e
    {
        none,
        first,
        last,
        full
    };

    static inline std::ostream& operator<<(std::ostream& ostream, mismatch_e mismatch_e);

    enum class delta_size_e
    {
        min,
        medium,
        max
    };

    static inline std::ostream& operator<<(std::ostream& ostream, delta_size_e delta_size);

    enum class exec_e
    {
        async = 0,
        sync = 1
    };

    static inline std::ostream& operator<<(std::ostream& ostream, exec_e path);

    enum class block_on_fault_e
    {
        block,
        dont_block
    };

    static inline std::ostream& operator<<(std::ostream& ostream, block_on_fault_e block_on_fault);

    template <>
    class WorkloadBuilder<CreateDeltaOperation>
    {
    public:
        WorkloadBuilder() noexcept:
            transfer_size_(8),
            expected_result_(expect_e::none),
            mismatch_(mismatch_e::none),
            delta_size_(delta_size_e::min),
            alignment_(8),
            block_on_fault_(block_on_fault_e::dont_block)
        {
        }

        auto& set_transfer_size(std::uint32_t size) noexcept
        {
            transfer_size_ = size;

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

        auto& set_delta_size(delta_size_e size) noexcept
        {
            delta_size_ = size;

            return *this;
        }

        auto& set_alignment(std::uint32_t alignment) noexcept
        {
            alignment_ = alignment;

            return *this;
        }

        auto& set_block_on_fault(block_on_fault_e block_on_fault) noexcept
        {
            block_on_fault_ = block_on_fault;

            return *this;
        }

        [[nodiscard]] auto build() const
        {
            auto src1_config = RegionConfiguration().set_size(transfer_size_).set_alignment(alignment_);
            auto src2_config = RegionConfiguration().set_size(transfer_size_).set_alignment(alignment_);

            const auto required_size = (transfer_size_ / 8u) * 10u;
            const auto min_size      = (required_size < 80) ? 80 : required_size;
            const auto max_size      = (required_size > 80) ? required_size : 80;
            const auto medium_size   = min_size + ((max_size - min_size) / 2u);

            auto delta_config = RegionConfiguration()
                                    .set_size(delta_size_ == delta_size_e::min   ? min_size
                                              : delta_size_ == delta_size_e::max ? max_size
                                                                                 : medium_size)
                                    .set_alignment(alignment_);

            auto memory = MemoryBuilder()
                              .add_region(src1_config)
                              .add_region(src2_config)
                              .add_region(delta_config)
                              .build();

            if (mismatch_ == mismatch_e::first)
            {
                memory.get_region(0).data()[0] = ~memory.get_region(0).data()[0];
            }
            else if (mismatch_ == mismatch_e::last)
            {
                memory.get_region(0).data()[transfer_size_ - 1] = ~memory.get_region(0).data()[transfer_size_ - 1];
            }
            else if (mismatch_ == mismatch_e::full)
            {
                for (auto& elem : memory.get_region(0))
                {
                    elem = ~elem;
                }
            }

            return Workload<CreateDeltaOperation>(std::move(memory),
                                                  expected_result_ != expect_e::none,
                                                  expected_result_ == expect_e::equal       ? 1
                                                  : expected_result_ == expect_e::not_equal ? 2
                                                  : expected_result_ == expect_e::overflow  ? 4
                                                                                            : 0,
                                                  block_on_fault_ == block_on_fault_e::block);
        }

    private:
        std::uint32_t     transfer_size_;
        expect_e          expected_result_;
        mismatch_e        mismatch_;
        delta_size_e      delta_size_;
        std::uint32_t     alignment_;
        block_on_fault_e  block_on_fault_;
    };

    static inline std::ostream& operator<<(std::ostream& ostream, expect_e expect)
    {
        if (expect == expect_e::none)
        {
            return ostream << "expect_none";
        }
        else if (expect == expect_e::equal)
        {
            return ostream << "expect_equal";
        }
        else if (expect == expect_e::not_equal)
        {
            return ostream << "expect_not_equal";
        }
        else if (expect == expect_e::overflow)
        {
            return ostream << "expect_overflow";
        }
        else
        {
            return ostream << "unexpected_enumeration";
        }
    }

    static inline std::ostream& operator<<(std::ostream& ostream, mismatch_e mismatch)
    {
        if (mismatch == mismatch_e::none)
        {
            return ostream << "mismatch_none";
        }
        else if (mismatch == mismatch_e::first)
        {
            return ostream << "mismatch_first";
        }
        else if (mismatch == mismatch_e::last)
        {
            return ostream << "mismatch_last";
        }
        else if (mismatch == mismatch_e::full)
        {
            return ostream << "mismatch_full";
        }
        else
        {
            return ostream << "unexpected_enumeration";
        }
    }

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

    static inline std::ostream& operator<<(std::ostream& ostream, exec_e path)
    {
        switch(path){
            case(exec_e::async):
                return ostream << "async";
            case(exec_e::sync):
                return ostream << "sync";
            default:
                return ostream << "unexpected_enumeration";
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

#endif  //DML_TESTING_CREATE_DELTA_WORKLOAD_BUILDER_HPP
