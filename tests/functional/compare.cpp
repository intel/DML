/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <gtest/gtest.h>

#include <utils/compare/actual.hpp>
#include <utils/compare/reference.hpp>
#include <utils/compare/workload_builder.hpp>

using dml::testing::expect_e;
using dml::testing::mismatch_e;

namespace// anonymous
{
    struct constant
    {
        static constexpr std::uint32_t transfer_size = 1024u;
        static constexpr std::uint32_t src1_alignment = 1u;
        static constexpr std::uint32_t src2_alignment = 1u;
    };

    struct variable
    {
        static constexpr std::uint32_t transfer_size[] = {1, 3, 8, 15, 64, 255, 1024};
        static constexpr std::uint32_t src1_alignment[] = {1, 2, 4, 8};
        static constexpr std::uint32_t src2_alignment[] = {1, 2, 4, 8};
        static constexpr expect_e expect[] = {expect_e::none, expect_e::equal, expect_e::not_equal};
        static constexpr mismatch_e mismatch[] = {mismatch_e::none, mismatch_e::first, mismatch_e::middle,
                                                  mismatch_e::last};
    };
}// namespace

using types = std::tuple<std::uint32_t, std::uint32_t, std::uint32_t, expect_e, mismatch_e>;

class compare: public ::testing::TestWithParam<types>
{
};

TEST_P(compare, success)
{
    auto [transfer_size, src1_alignment, src2_alignment, expect, mismatch] = GetParam();

    auto workload_builder = dml::testing::WorkloadBuilder<dml::testing::CompareOperation>()
                                .set_transfer_size(transfer_size)
                                .set_src1_alignment(src1_alignment)
                                .set_src2_alignment(src2_alignment)
                                .set_expected_result(expect)
                                .set_mismatch(mismatch);

    auto actual_workload    = workload_builder.build();
    auto reference_workload = workload_builder.build();

    auto actual_result    = dml::testing::ActualImplementation(actual_workload);
    auto reference_result = dml::testing::ReferenceImplementation(reference_workload);

    ASSERT_EQ(actual_result, reference_result);
    ASSERT_EQ(actual_workload, reference_workload);
}

#if !defined(SW_PATH) && defined(OS_UNIX)
#include <sys/mman.h>
#include <unistd.h>

TEST(compare, page_fault_read_first)
{
    const auto page_size = getpagesize();
    const auto multiplier = 4u;
    const auto fault_page = 2u;

    auto workload_builder = dml::testing::WorkloadBuilder<dml::testing::CompareOperation>()
                                .set_transfer_size(page_size * multiplier)
                                .set_src1_alignment(page_size)
                                .set_src2_alignment(page_size)
                                .set_mismatch(mismatch_e::none);

    auto actual_workload    = workload_builder.build();
    madvise(actual_workload.get_src1().data() + page_size * fault_page, page_size, MADV_DONTNEED);
    auto actual_result    = dml::testing::ActualImplementation(actual_workload);

#if defined (HW_PATH)
    ASSERT_EQ(actual_result, dml::testing::StatusCode::PageFault);
#endif

#if defined (AUTO_PATH)
    auto reference_workload = workload_builder.build();
    madvise(reference_workload.get_src1().data() + page_size * fault_page, page_size, MADV_DONTNEED);
    auto reference_result = dml::testing::ReferenceImplementation(reference_workload);
    ASSERT_EQ(actual_result, reference_result);
    ASSERT_EQ(actual_workload, reference_workload);
#endif

}

TEST(compare, page_fault_read_second)
{
    const auto page_size = getpagesize();
    const auto multiplier = 4u;
    const auto fault_page = 2u;

    auto workload_builder = dml::testing::WorkloadBuilder<dml::testing::CompareOperation>()
                                .set_transfer_size(page_size * multiplier)
                                .set_src1_alignment(page_size)
                                .set_src2_alignment(page_size)
                                .set_mismatch(mismatch_e::none);

    auto actual_workload    = workload_builder.build();
    madvise(actual_workload.get_src2().data() + page_size * fault_page, page_size, MADV_DONTNEED);
    auto actual_result    = dml::testing::ActualImplementation(actual_workload);

#if defined (HW_PATH)
    ASSERT_EQ(actual_result, dml::testing::StatusCode::PageFault);
#endif

#if defined (AUTO_PATH)
    auto reference_workload = workload_builder.build();
    madvise(reference_workload.get_src2().data() + page_size * fault_page, page_size, MADV_DONTNEED);
    auto reference_result = dml::testing::ReferenceImplementation(reference_workload);
    ASSERT_EQ(actual_result, reference_result);
    ASSERT_EQ(actual_workload, reference_workload);
#endif

}
#endif

INSTANTIATE_TEST_SUITE_P(transfer_size,
                         compare,
                         ::testing::Combine(::testing::ValuesIn(variable::transfer_size),
                                            ::testing::Values(constant::src1_alignment),
                                            ::testing::Values(constant::src2_alignment),
                                            ::testing::ValuesIn(variable::expect),
                                            ::testing::ValuesIn(variable::mismatch)));

INSTANTIATE_TEST_SUITE_P(alignment,
                         compare,
                         ::testing::Combine(::testing::Values(constant::transfer_size),
                                            ::testing::ValuesIn(variable::src1_alignment),
                                            ::testing::ValuesIn(variable::src2_alignment),
                                            ::testing::ValuesIn(variable::expect),
                                            ::testing::ValuesIn(variable::mismatch)));
