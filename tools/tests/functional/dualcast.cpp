/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <gtest/gtest.h>

#include <utils/dualcast/actual.hpp>
#include <utils/dualcast/reference.hpp>
#include <utils/dualcast/workload_builder.hpp>

using dml::testing::block_on_fault_e;

namespace// anonymous
{
    struct constant
    {
        static constexpr std::uint32_t transfer_size = 1024;
        static constexpr std::uint32_t src_alignment = 1u;
        static constexpr std::uint32_t dst_alignment = 1u;
    };

    struct variable
    {
        static constexpr std::uint32_t transfer_size[] = { 1, 3, 8, 15, 64, 255, 1024, 8096 };
        static constexpr std::uint32_t src_alignment[] = {1, 2, 4, 8};
        static constexpr std::uint32_t dst_alignment[] = {1, 2, 4, 8};
        static constexpr block_on_fault_e block_on_fault[] = {block_on_fault_e::dont_block, block_on_fault_e::block};
    };
}// namespace

using types = std::tuple<std::uint32_t, std::uint32_t, std::uint32_t>;

class dualcast: public ::testing::TestWithParam<types>
{
};

TEST_P(dualcast, success)
{
    auto [transfer_size, src_alignment, dst_alignment] = GetParam();

    auto workload_builder = dml::testing::WorkloadBuilder<dml::testing::DualcastOperation>()
                                .set_transfer_size(transfer_size)
                                .set_src_alignment(src_alignment)
                                .set_dst_alignment(dst_alignment);

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

using page_fault_types = std::tuple<block_on_fault_e>;

class dualcast_page_fault: public ::testing::TestWithParam<page_fault_types>
{
};

TEST_P(dualcast_page_fault, read)
{
    auto [block_on_fault] = GetParam();

    const auto page_size = getpagesize();
    const auto multiplier = 4u;
    const auto fault_page = 2u;

    auto workload_builder = dml::testing::WorkloadBuilder<dml::testing::DualcastOperation>()
                                .set_transfer_size(page_size * multiplier)
                                .set_src_alignment(page_size)
                                .set_dst_alignment(page_size)
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

TEST_P(dualcast_page_fault, write_first)
{
    auto [block_on_fault] = GetParam();

    const auto page_size = getpagesize();
    const auto multiplier = 4u;
    const auto fault_page = 2u;

    auto workload_builder = dml::testing::WorkloadBuilder<dml::testing::DualcastOperation>()
                                .set_transfer_size(page_size * multiplier)
                                .set_src_alignment(page_size)
                                .set_dst_alignment(page_size)
                                .set_block_on_fault(block_on_fault);

    auto actual_workload    = workload_builder.build();
    madvise(actual_workload.get_dst1().data() + page_size * fault_page, page_size, MADV_DONTNEED);
    auto actual_result    = dml::testing::ActualImplementation(actual_workload);

#if defined (HW_PATH)
    if(block_on_fault == block_on_fault_e::block){
        auto reference_workload = workload_builder.build();
        madvise(reference_workload.get_dst1().data() + page_size * fault_page, page_size, MADV_DONTNEED);
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
    madvise(reference_workload.get_dst1().data() + page_size * fault_page, page_size, MADV_DONTNEED);
    auto reference_result = dml::testing::ReferenceImplementation(reference_workload);
    ASSERT_EQ(actual_result, reference_result);
    ASSERT_EQ(actual_workload, reference_workload);
#endif

}

TEST_P(dualcast_page_fault, write_second)
{
    auto [block_on_fault] = GetParam();

    const auto page_size = getpagesize();
    const auto multiplier = 4u;
    const auto fault_page = 2u;

    auto workload_builder = dml::testing::WorkloadBuilder<dml::testing::DualcastOperation>()
                                .set_transfer_size(page_size * multiplier)
                                .set_src_alignment(page_size)
                                .set_dst_alignment(page_size)
                                .set_block_on_fault(block_on_fault);

    auto actual_workload    = workload_builder.build();
    madvise(actual_workload.get_dst2().data() + page_size * fault_page, page_size, MADV_DONTNEED);
    auto actual_result    = dml::testing::ActualImplementation(actual_workload);

#if defined (HW_PATH)
    if(block_on_fault == block_on_fault_e::block){
        auto reference_workload = workload_builder.build();
        madvise(reference_workload.get_dst2().data() + page_size * fault_page, page_size, MADV_DONTNEED);
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
    madvise(reference_workload.get_dst2().data() + page_size * fault_page, page_size, MADV_DONTNEED);
    auto reference_result = dml::testing::ReferenceImplementation(reference_workload);
    ASSERT_EQ(actual_result, reference_result);
    ASSERT_EQ(actual_workload, reference_workload);
#endif

}

INSTANTIATE_TEST_SUITE_P(block_on_fault,
                         dualcast_page_fault,
                         ::testing::Combine(::testing::ValuesIn(variable::block_on_fault)));
#endif

INSTANTIATE_TEST_SUITE_P(transfer_size,
                         dualcast,
                         ::testing::Combine(::testing::ValuesIn(variable::transfer_size),
                                            ::testing::Values(constant::src_alignment),
                                            ::testing::Values(constant::dst_alignment)));

INSTANTIATE_TEST_SUITE_P(alignment,
                         dualcast,
                         ::testing::Combine(::testing::Values(constant::transfer_size),
                                            ::testing::ValuesIn(variable::src_alignment),
                                            ::testing::ValuesIn(variable::dst_alignment)));
