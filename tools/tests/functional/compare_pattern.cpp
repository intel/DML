/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <gtest/gtest.h>

#include <utils/compare_pattern/actual.hpp>
#include <utils/compare_pattern/reference.hpp>
#include <utils/compare_pattern/workload_builder.hpp>

using dml::testing::expect_e;
using dml::testing::mismatch_e;
using dml::testing::block_on_fault_e;

namespace// anonymous
{
    struct constant
    {
        static constexpr std::uint32_t transfer_size = 1024u;
        static constexpr std::uint32_t src_alignment = 1u;
    };

    struct variable
    {
        static constexpr std::uint32_t transfer_size[] = {1, 3, 8, 15, 64, 255, 1024};
        static constexpr std::uint32_t src_alignment[] = {1, 2, 4, 8};
        static constexpr expect_e expect[] = {expect_e::none, expect_e::equal, expect_e::not_equal};
        static constexpr mismatch_e mismatch[] = {mismatch_e::none, mismatch_e::first, mismatch_e::middle,
                                                  mismatch_e::last};
        #if !defined(SW_PATH) && defined(__linux__)
        static constexpr block_on_fault_e block_on_fault[] = {block_on_fault_e::dont_block, block_on_fault_e::block};
        #endif
    };
}// namespace

using types = std::tuple<std::uint32_t, std::uint32_t, expect_e, mismatch_e>;

class compare_pattern: public ::testing::TestWithParam<types>
{
};

TEST_P(compare_pattern, success)
{
    auto [transfer_size, src_alignment, expect, mismatch] = GetParam();

    auto workload_builder = dml::testing::WorkloadBuilder<dml::testing::ComparePatternOperation>()
                                .set_transfer_size(transfer_size)
                                .set_src_alignment(src_alignment)
                                .set_expected_result(expect)
                                .set_mismatch(mismatch)
                                .set_mismatch(mismatch_e::none);

    auto actual_workload    = workload_builder.build();
    auto reference_workload = workload_builder.build();

    auto actual_result    = dml::testing::ActualImplementation(actual_workload);
    auto reference_result = dml::testing::ReferenceImplementation(reference_workload);

    ASSERT_EQ(actual_result, reference_result);
    ASSERT_EQ(actual_workload, reference_workload);
}

#if !defined(SW_PATH) && defined(__linux__)
#include <sys/mman.h>
#include <unistd.h>

using page_fault_types = std::tuple<block_on_fault_e>;

class compare_pattern_page_fault: public ::testing::TestWithParam<page_fault_types>
{
};

TEST_P(compare_pattern_page_fault, read)
{
    auto [block_on_fault] = GetParam();

    const auto page_size = getpagesize();
    const auto multiplier = 4u;
    const auto fault_page = 2u;

    auto workload_builder = dml::testing::WorkloadBuilder<dml::testing::ComparePatternOperation>()
                                .set_transfer_size(page_size * multiplier)
                                .set_src_alignment(page_size)
                                .set_mismatch(mismatch_e::none)
                                .set_block_on_fault(block_on_fault);

    auto actual_workload    = workload_builder.build();
    madvise(actual_workload.get_src().data() + page_size * fault_page, page_size, MADV_DONTNEED);
    auto actual_result    = dml::testing::ActualImplementation(actual_workload);

#if defined (HW_PATH)
    if(block_on_fault == block_on_fault_e::block){
        auto reference_workload = workload_builder.build();
        madvise(reference_workload.get_src().data() + page_size * fault_page, page_size, MADV_DONTNEED);
        auto reference_result = dml::testing::ReferenceImplementation(reference_workload);
        ASSERT_EQ(actual_result, reference_result);
        ASSERT_EQ(actual_workload, reference_workload);
    }
    else{
        ASSERT_EQ(actual_result, dml::testing::StatusCode::PageFault);
    }
#endif

#if defined (AUTO_PATH)
    auto reference_workload = workload_builder.build();
    madvise(reference_workload.get_src().data() + page_size * fault_page, page_size, MADV_DONTNEED);
    auto reference_result = dml::testing::ReferenceImplementation(reference_workload);
    ASSERT_EQ(actual_result, reference_result);
    ASSERT_EQ(actual_workload, reference_workload);
#endif

}

INSTANTIATE_TEST_SUITE_P(block_on_fault,
                         compare_pattern_page_fault,
                         ::testing::Combine(::testing::ValuesIn(variable::block_on_fault)));
#endif

INSTANTIATE_TEST_SUITE_P(transfer_size,
                         compare_pattern,
                         ::testing::Combine(::testing::ValuesIn(variable::transfer_size),
                                            ::testing::Values(constant::src_alignment),
                                            ::testing::ValuesIn(variable::expect),
                                            ::testing::ValuesIn(variable::mismatch)));

INSTANTIATE_TEST_SUITE_P(alignment,
                         compare_pattern,
                         ::testing::Combine(::testing::Values(constant::transfer_size),
                                            ::testing::ValuesIn(variable::src_alignment),
                                            ::testing::ValuesIn(variable::expect),
                                            ::testing::ValuesIn(variable::mismatch)));
