/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <gtest/gtest.h>

#include <utils/apply_delta/actual.hpp>
#include <utils/apply_delta/reference.hpp>
#include <utils/apply_delta/workload_builder.hpp>

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
        static constexpr delta_size_e delta_size[] = {delta_size_e::min, delta_size_e::medium, delta_size_e::max};
    };
}// namespace

using types = std::tuple<std::uint32_t, delta_size_e>;

class apply_delta: public ::testing::TestWithParam<types>
{
};

TEST_P(apply_delta, success)
{
    auto [transfer_size, delta_size] = GetParam();

    auto workload_builder = dml::testing::WorkloadBuilder<dml::testing::ApplyDeltaOperation>()
                                .set_transfer_size(transfer_size)
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

TEST(apply_delta, page_fault_read)
{
    const auto page_size = getpagesize();
    const auto multiplier = 4u;
    const auto fault_page = 0u; // TODO: Add generator for delta

    auto workload_builder = dml::testing::WorkloadBuilder<dml::testing::ApplyDeltaOperation>()
                                .set_transfer_size(page_size * multiplier)
                                .set_alignment(page_size)
                                .set_delta_size(delta_size_e::max);

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

TEST(apply_delta, page_fault_write)
{
    const auto page_size = getpagesize();
    const auto multiplier = 4u;
    const auto fault_page = 2u;

    auto workload_builder = dml::testing::WorkloadBuilder<dml::testing::ApplyDeltaOperation>()
                                .set_transfer_size(page_size * multiplier)
                                .set_alignment(page_size)
                                .set_delta_size(delta_size_e::max);

    auto actual_workload    = workload_builder.build();
    madvise(actual_workload.get_dst().data() + page_size * fault_page, page_size, MADV_DONTNEED);
    auto actual_result    = dml::testing::ActualImplementation(actual_workload);

#if defined (HW_PATH)
    ASSERT_EQ(actual_result, dml::testing::StatusCode::PageFault);
#endif

#if defined (AUTO_PATH)
    auto reference_workload = workload_builder.build();
    madvise(reference_workload.get_dst().data() + page_size * fault_page, page_size, MADV_DONTNEED);
    auto reference_result = dml::testing::ReferenceImplementation(reference_workload);
    ASSERT_EQ(actual_result, reference_result);
    ASSERT_EQ(actual_workload, reference_workload);
#endif

}
#endif

INSTANTIATE_TEST_SUITE_P(transfer_size,
                         apply_delta,
                         ::testing::Combine(::testing::ValuesIn(variable::transfer_size),
                                            ::testing::ValuesIn(variable::delta_size)));
