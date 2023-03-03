/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml/dml.hpp>

#include <dml_test_utils/cache_flush.hpp>
#include <dml_test_utils/compare.hpp>
#include <dml_test_utils/compare_pattern.hpp>
#include <dml_test_utils/copy_crc.hpp>
#include <dml_test_utils/crc.hpp>
#include <dml_test_utils/delta.hpp>
#include <dml_test_utils/dualcast.hpp>
#include <dml_test_utils/fill.hpp>
#include <dml_test_utils/mem_move.hpp>

#include "gtest/gtest.h"

#include "own/path.hpp"

DML_TESTING_HL_PARAMETRIZE(dmlhl_batch);

TYPED_TEST(dmlhl_batch, all_operations)
{
    SKIP_IF_WRONG_PATH(typename TestFixture::execution_path);

    constexpr auto length = 16u;
    constexpr auto seed   = 777u;
    constexpr auto count  = 10u;

    auto mem_move        = dml::testing::mem_move(seed, length);
    auto fill            = dml::testing::fill(seed, length);
    auto dualcast        = dml::testing::dualcast(seed, length);
    auto compare         = dml::testing::compare_equal(seed, length);
    auto compare_pattern = dml::testing::compare_pattern_equal(seed, length);
    auto create_delta    = dml::testing::delta(seed, length);
    auto apply_delta     = dml::testing::delta(seed, length);
    auto crc             = dml::testing::crc(seed, length);
    auto copy_crc        = dml::testing::copy_crc(seed, length);
    auto cache_flush     = dml::testing::cache_flush(seed, length);

    auto sequence = dml::sequence(count, std::allocator<dml::byte_t>());

    ASSERT_EQ(sequence.add(dml::mem_move, dml::make_view(mem_move.src), dml::make_view(mem_move.dst)),
              dml::status_code::ok);
    ASSERT_EQ(sequence.add(dml::fill, fill.pattern, dml::make_view(fill.dst)), dml::status_code::ok);
    ASSERT_EQ(sequence.add(dml::dualcast,
                           dml::make_view(dualcast.src),
                           dml::make_view(dualcast.dst1, dualcast.length),
                           dml::make_view(dualcast.dst2, dualcast.length)),
              dml::status_code::ok);
    ASSERT_EQ(sequence.add(dml::compare, dml::make_view(compare.src1), dml::make_view(compare.src2)),
              dml::status_code::ok);
    ASSERT_EQ(sequence.add(dml::compare_pattern, compare_pattern.pattern, dml::make_view(compare_pattern.src)),
              dml::status_code::ok);
    ASSERT_EQ(sequence.add(dml::create_delta,
                           dml::make_view(create_delta.src1),
                           dml::make_view(create_delta.src2),
                           dml::make_view(create_delta.delta_record)),
              dml::status_code::ok);

    auto create_result = this->run(dml::create_delta,
                                   dml::make_view(apply_delta.src1),
                                   dml::make_view(apply_delta.src2),
                                   dml::make_view(apply_delta.delta_record));

    ASSERT_EQ(sequence.add(dml::apply_delta,
                           dml::make_view(apply_delta.delta_record),
                           dml::make_view(apply_delta.src1),
                           create_result),
              dml::status_code::ok);
    ASSERT_EQ(sequence.add(dml::crc, dml::make_view(crc.src), crc.crc_seed), dml::status_code::ok);
    ASSERT_EQ(
        sequence.add(dml::copy_crc, dml::make_view(copy_crc.src), dml::make_view(copy_crc.dst), copy_crc.crc_seed),
        dml::status_code::ok);
    ASSERT_EQ(sequence.add(dml::cache_flush, dml::make_view(cache_flush.dst)), dml::status_code::ok);

    auto result = this->run(dml::batch, sequence);

    ASSERT_EQ(result.status, dml::status_code::ok);
    ASSERT_EQ(result.operations_completed, count);

    // No support for check result of other operations
    ASSERT_TRUE(mem_move.check());
    ASSERT_TRUE(fill.check());
    ASSERT_TRUE(dualcast.check());
    ASSERT_TRUE(apply_delta.check());
    ASSERT_TRUE(copy_crc.check());
}

TYPED_TEST(dmlhl_batch, bad_length_0)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;
    constexpr auto count  = 10u;

    auto sequence = dml::sequence(count, std::allocator<dml::byte_t>());

    auto result = this->run(dml::batch, sequence);

    ASSERT_EQ(result.status, dml::status_code::bad_length);
    ASSERT_EQ(result.operations_completed, 0u);
}

TYPED_TEST(dmlhl_batch, bad_length_3)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;
    constexpr auto count  = 10u;

    auto test_data = dml::testing::mem_move(seed, length);

    auto sequence = dml::sequence(count, std::allocator<dml::byte_t>());

    sequence.add(dml::mem_move, dml::make_view(test_data.src), dml::make_view(test_data.dst));
    sequence.add(dml::mem_move, dml::make_view(test_data.src), dml::make_view(test_data.dst));
    sequence.add(dml::mem_move, dml::make_view(test_data.src), dml::make_view(test_data.dst));

    auto result = this->run(dml::batch, sequence);

    ASSERT_EQ(result.status, dml::status_code::bad_length);
    ASSERT_EQ(result.operations_completed, 0u);
}

TEST(dmlhl_batch, bad_alloc)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;
    constexpr auto count  = 4u;

    auto test_data = dml::testing::mem_move(seed, length);

    auto sequence = dml::sequence(count, std::allocator<dml::byte_t>());

    sequence.add(dml::mem_move, dml::make_view(test_data.src), dml::make_view(test_data.dst));
    sequence.add(dml::mem_move, dml::make_view(test_data.src), dml::make_view(test_data.dst));
    sequence.add(dml::mem_move, dml::make_view(test_data.src), dml::make_view(test_data.dst));
    sequence.add(dml::mem_move, dml::make_view(test_data.src), dml::make_view(test_data.dst));

    ASSERT_ANY_THROW(auto result = dml::submit<dml::software>(dml::batch,
                                                              sequence,
                                                              dml::execution_interface(
                                                                  [](auto &&task)
                                                                  {
                                                                      task();
                                                                  },
                                                                  exception_allocator<int>())));
}
