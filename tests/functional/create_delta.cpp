/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <gtest/gtest.h>

#include <utils/create_delta/actual.hpp>
#include <utils/create_delta/reference.hpp>
#include <utils/create_delta/workload_builder.hpp>

using dml::testing::expect_e;
using dml::testing::mismatch_e;
using dml::testing::delta_size_e;

namespace// anonymous
{
    struct constant
    {
        static constexpr std::uint32_t transfer_size = 1024u;
    };

    struct variable
    {
        static constexpr std::uint32_t transfer_size[] = {8, 16, 64, 256, 4096, 0x80000};
        static constexpr expect_e expect[] = {expect_e::none, expect_e::equal, expect_e::not_equal,
#ifndef CPP_API// Overflow isn't implemented in C++ API
                                               expect_e::overflow
#endif
        };
        static constexpr mismatch_e mismatch[] = {mismatch_e::none, mismatch_e::first, mismatch_e::last,
                                                  mismatch_e::full};
        static constexpr delta_size_e delta_size[] = {delta_size_e::min, delta_size_e::medium, delta_size_e::max};
    };
}// namespace

static const std::uint32_t transfer_sizes[] = { 8, 16, 64, 256, 4096, 0x80000 };



using types = std::tuple<std::uint32_t, expect_e, mismatch_e, delta_size_e>;

class create_delta: public ::testing::TestWithParam<types>
{
};

TEST_P(create_delta, success)
{
    auto [transfer_size, expect, mismatch, delta_size] = GetParam();

    auto workload_builder = dml::testing::WorkloadBuilder<dml::testing::CreateDeltaOperation>()
                                .set_transfer_size(transfer_size)
                                .set_expected_result(expect)
                                .set_mismatch(mismatch)
                                .set_delta_size(delta_size);

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

TEST(create_delta, page_fault_read_first)
{
    const auto page_size = getpagesize();
    const auto multiplier = 4u;
    const auto fault_page = 2u;

    auto workload_builder = dml::testing::WorkloadBuilder<dml::testing::CreateDeltaOperation>()
                                .set_transfer_size(page_size * multiplier)
                                .set_alignment(page_size)
                                .set_delta_size(delta_size_e::max)
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

TEST(create_delta, page_fault_read_second)
{
    const auto page_size = getpagesize();
    const auto multiplier = 4u;
    const auto fault_page = 2u;

    auto workload_builder = dml::testing::WorkloadBuilder<dml::testing::CreateDeltaOperation>()
                                .set_transfer_size(page_size * multiplier)
                                .set_alignment(page_size)
                                .set_delta_size(delta_size_e::max)
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

TEST(create_delta, page_fault_write)
{
    const auto page_size = getpagesize();
    const auto multiplier = 4u;
    const auto fault_page = 2u;

    auto workload_builder = dml::testing::WorkloadBuilder<dml::testing::CreateDeltaOperation>()
                                .set_transfer_size(page_size * multiplier)
                                .set_alignment(page_size)
                                .set_delta_size(delta_size_e::max)
                                .set_mismatch(mismatch_e::full);

    auto actual_workload    = workload_builder.build();
    madvise(actual_workload.get_delta().data() + page_size * fault_page, page_size, MADV_DONTNEED);
    auto actual_result    = dml::testing::ActualImplementation(actual_workload);

#if defined (HW_PATH)
    ASSERT_EQ(actual_result, dml::testing::StatusCode::PageFault);
#endif

#if defined (AUTO_PATH)
    auto reference_workload = workload_builder.build();
    madvise(reference_workload.get_delta().data() + page_size * fault_page, page_size, MADV_DONTNEED);
    auto reference_result = dml::testing::ReferenceImplementation(reference_workload);
    ASSERT_EQ(actual_result, reference_result);
    ASSERT_EQ(actual_workload, reference_workload);
#endif

}
#endif

INSTANTIATE_TEST_SUITE_P(transfer_size,
                         create_delta,
                         ::testing::Combine(::testing::ValuesIn(variable::transfer_size),
                                            ::testing::ValuesIn(variable::expect),
                                            ::testing::ValuesIn(variable::mismatch),
                                            ::testing::ValuesIn(variable::delta_size)));
