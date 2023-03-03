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

TEST(dmlhl_sequence, batch_overflow)
{
    constexpr auto size = 16u;
    constexpr auto seed = 777u;

    auto mem_move        = dml::testing::mem_move(seed, size);
    auto fill            = dml::testing::fill(seed, size);
    auto dualcast        = dml::testing::dualcast(seed, size);
    auto compare         = dml::testing::compare_equal(seed, size);
    auto compare_pattern = dml::testing::compare_pattern_equal(seed, size);
    auto create_delta    = dml::testing::delta(seed, size);
    auto apply_delta     = dml::testing::delta(seed, size);
    auto crc             = dml::testing::crc(seed, size);
    auto copy_crc        = dml::testing::copy_crc(seed, size);
    auto cache_flush     = dml::testing::cache_flush(seed, size);

    auto sequence = dml::sequence(0, std::allocator<int>());

    ASSERT_EQ(sequence.add(dml::mem_move, dml::make_view(mem_move.src), dml::make_view(mem_move.dst)), dml::status_code::batch_overflow);
    ASSERT_EQ(sequence.add(dml::fill, fill.pattern, dml::make_view(fill.dst)), dml::status_code::batch_overflow);
    ASSERT_EQ(sequence.add(dml::dualcast,
                           dml::make_view(dualcast.src),
                           dml::make_view(dualcast.dst1, dualcast.length),
                           dml::make_view(dualcast.dst2, dualcast.length)),
              dml::status_code::batch_overflow);
    ASSERT_EQ(sequence.add(dml::compare, dml::make_view(compare.src1), dml::make_view(compare.src2)), dml::status_code::batch_overflow);
    ASSERT_EQ(sequence.add(dml::compare_pattern, compare_pattern.pattern, dml::make_view(compare_pattern.src)),
              dml::status_code::batch_overflow);
    ASSERT_EQ(sequence.add(dml::create_delta,
                           dml::make_view(create_delta.src1),
                           dml::make_view(create_delta.src2),
                           dml::make_view(create_delta.delta_record)),
              dml::status_code::batch_overflow);

    ASSERT_EQ(sequence.add(dml::apply_delta,
                           dml::make_view(apply_delta.delta_record),
                           dml::make_view(apply_delta.src1),
                           { {}, dml::comparison_result::not_equal, {}, {} }),
              dml::status_code::batch_overflow);
    ASSERT_EQ(sequence.add(dml::crc, dml::make_view(crc.src), crc.crc_seed), dml::status_code::batch_overflow);
    ASSERT_EQ(sequence.add(dml::copy_crc, dml::make_view(copy_crc.src), dml::make_view(copy_crc.dst), copy_crc.crc_seed),
              dml::status_code::batch_overflow);
    ASSERT_EQ(sequence.add(dml::cache_flush, dml::make_view(cache_flush.dst)), dml::status_code::batch_overflow);
}

TEST(dmlhl_sequence, mem_move_src_null)
{
    constexpr auto size = 16u;
    constexpr auto seed = 777u;

    auto test_data = dml::testing::mem_move(seed, size);
    auto sequence  = dml::sequence(1, std::allocator<int>());

    auto status = sequence.add(dml::mem_move, dml::make_view<uint8_t>(nullptr, size), dml::make_view(test_data.dst));

    ASSERT_EQ(status, dml::status_code::nullptr_error);
}

TEST(dmlhl_sequence, mem_move_dst_null)
{
    constexpr auto size = 16u;
    constexpr auto seed = 777u;

    auto test_data = dml::testing::mem_move(seed, size);
    auto sequence  = dml::sequence(1, std::allocator<int>());

    auto status = sequence.add(dml::mem_move, dml::make_view(test_data.src), dml::make_view<uint8_t>(nullptr, size));

    ASSERT_EQ(status, dml::status_code::nullptr_error);
}

TEST(dmlhl_sequence, mem_move_inconsistent_size)
{
    constexpr auto size = 16u;
    constexpr auto seed = 777u;

    auto test_data = dml::testing::mem_move(seed, size);
    auto sequence  = dml::sequence(1, std::allocator<int>());

    auto status = sequence.add(dml::mem_move, dml::make_view(test_data.src.data(), test_data.length - 1u), dml::make_view(test_data.dst));

    ASSERT_EQ(status, dml::status_code::inconsistent_size);
}

TEST(dmlhl_sequence, mem_move_bad_size)
{
    constexpr auto size = 16u;
    constexpr auto seed = 777u;

    auto test_data = dml::testing::mem_move(seed, size);
    auto sequence  = dml::sequence(1, std::allocator<int>());

    auto status = sequence.add(dml::mem_move, dml::make_view(test_data.src.data(), 0u), dml::make_view(test_data.dst.data(), 0u));

    ASSERT_EQ(status, dml::status_code::bad_size);
}

TEST(dmlhl_sequence, fill_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::fill(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::fill, test_data.pattern, dml::make_view<std::uint8_t>(nullptr, test_data.length));

    ASSERT_EQ(status, dml::status_code::nullptr_error);
}

TEST(dmlhl_sequence, fill_bad_size)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::fill(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::fill, test_data.pattern, dml::make_view(test_data.dst.data(), 0u));

    ASSERT_EQ(status, dml::status_code::bad_size);
}

TEST(dmlhl_sequence, dualcast_src_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::dualcast(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::dualcast,
                               dml::make_view<uint8_t>(nullptr, test_data.length),
                               dml::make_view(test_data.dst1, test_data.length),
                               dml::make_view(test_data.dst2, test_data.length));

    ASSERT_EQ(status, dml::status_code::nullptr_error);
}

TEST(dmlhl_sequence, dualcast_dst1_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::dualcast(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::dualcast,
                               dml::make_view(test_data.src),
                               dml::make_view<uint8_t>(nullptr, test_data.length),
                               dml::make_view(test_data.dst2, test_data.length));

    ASSERT_EQ(status, dml::status_code::nullptr_error);
}

TEST(dmlhl_sequence, dualcast_dst2_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::dualcast(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::dualcast,
                               dml::make_view(test_data.src),
                               dml::make_view(test_data.dst1, test_data.length),
                               dml::make_view<uint8_t>(nullptr, test_data.length));

    ASSERT_EQ(status, dml::status_code::nullptr_error);
}

TEST(dmlhl_sequence, dualcast_inconsistent_size_1)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::dualcast(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::dualcast,
                               dml::make_view(test_data.src),
                               dml::make_view(test_data.dst1, test_data.length - 1u),
                               dml::make_view(test_data.dst2, test_data.length));

    ASSERT_EQ(status, dml::status_code::inconsistent_size);
}

TEST(dmlhl_sequence, dualcast_inconsistent_size_2)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::dualcast(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::dualcast,
                               dml::make_view(test_data.src),
                               dml::make_view(test_data.dst1, test_data.length),
                               dml::make_view(test_data.dst2, test_data.length - 1u));

    ASSERT_EQ(status, dml::status_code::inconsistent_size);
}

TEST(dmlhl_sequence, dualcast_bad_size)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::dualcast(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::dualcast,
                               dml::make_view(test_data.src.data(), 0u),
                               dml::make_view(test_data.dst1, 0u),
                               dml::make_view(test_data.dst2, 0u));

    ASSERT_EQ(status, dml::status_code::bad_size);
}

TEST(dmlhl_sequence, dualcast_bad_padding)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::dualcast(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::dualcast,
                               dml::make_view(test_data.src),
                               dml::make_view(test_data.dst1, test_data.length),
                               dml::make_view(test_data.dst2 - 1u, test_data.length));

    ASSERT_EQ(status, dml::status_code::dualcast_bad_padding);
}

TEST(dmlhl_sequence, create_delta_src1_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::create_delta,
                               dml::make_view<uint8_t>(nullptr, test_data.length),
                               dml::make_view(test_data.src2),
                               dml::make_view(test_data.delta_record));

    ASSERT_EQ(status, dml::status_code::nullptr_error);
}

TEST(dmlhl_sequence, create_delta_src2_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::create_delta,
                               dml::make_view(test_data.src1),
                               dml::make_view<uint8_t>(nullptr, test_data.length),
                               dml::make_view(test_data.delta_record));

    ASSERT_EQ(status, dml::status_code::nullptr_error);
}

TEST(dmlhl_sequence, create_delta_delta_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::create_delta,
                               dml::make_view(test_data.src1),
                               dml::make_view(test_data.src2),
                               dml::make_view<uint8_t>(nullptr, static_cast<dml::size_t>(test_data.delta_record.size())));

    ASSERT_EQ(status, dml::status_code::nullptr_error);
}

TEST(dmlhl_sequence, create_delta_inconsistent_size)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::create_delta,
                               dml::make_view(test_data.src1),
                               dml::make_view(test_data.src2.data(), test_data.length - 1u),
                               dml::make_view(test_data.delta_record));

    ASSERT_EQ(status, dml::status_code::inconsistent_size);
}

TEST(dmlhl_sequence, create_delta_bad_size_zero)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::create_delta,
                               dml::make_view(test_data.src1.data(), 0u),
                               dml::make_view(test_data.src2.data(), 0u),
                               dml::make_view(test_data.delta_record));

    ASSERT_EQ(status, dml::status_code::bad_size);
}

TEST(dmlhl_sequence, create_delta_bad_size_not_8_divided)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::create_delta,
                               dml::make_view(test_data.src1.data(), test_data.length - 1u),
                               dml::make_view(test_data.src2.data(), test_data.length - 1u),
                               dml::make_view(test_data.delta_record));

    ASSERT_EQ(status, dml::status_code::bad_size);
}

TEST(dmlhl_sequence, create_delta_delta_bad_size_less_than_80)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::create_delta,
                               dml::make_view(test_data.src1),
                               dml::make_view(test_data.src2),
                               dml::make_view(test_data.delta_record.data(), 79u));

    ASSERT_EQ(status, dml::status_code::delta_bad_size);
}

TEST(dmlhl_sequence, create_delta_delta_bad_size_not_10_divided)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::create_delta,
                               dml::make_view(test_data.src1),
                               dml::make_view(test_data.src2),
                               dml::make_view(test_data.delta_record.data(), test_data.length + 1u));

    ASSERT_EQ(status, dml::status_code::delta_bad_size);
}

TEST(dmlhl_sequence, apply_delta_src_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto create_result = dml::create_delta_result{ dml::status_code::ok, dml::comparison_result::not_equal, 0u, 10u };

    auto status = sequence.add(dml::apply_delta,
                               dml::make_view(test_data.delta_record),
                               dml::make_view<uint8_t>(nullptr, test_data.length),
                               create_result);

    ASSERT_EQ(status, dml::status_code::nullptr_error);
}

TEST(dmlhl_sequence, apply_delta_delta_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto create_result = dml::create_delta_result{ dml::status_code::ok, dml::comparison_result::not_equal, 0u, 10u };

    auto status = sequence.add(dml::apply_delta,
                               dml::make_view<uint8_t>(nullptr, static_cast<dml::size_t>(test_data.delta_record.size())),
                               dml::make_view(test_data.src1),
                               create_result);

    ASSERT_EQ(status, dml::status_code::nullptr_error);
}

TEST(dmlhl_sequence, apply_delta_bad_size_src)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto create_result = dml::create_delta_result{ dml::status_code::ok, dml::comparison_result::not_equal, 0u, 10u };

    auto status =
        sequence.add(dml::apply_delta, dml::make_view(test_data.delta_record), dml::make_view(test_data.src1.data(), 0u), create_result);

    ASSERT_EQ(status, dml::status_code::bad_size);
}

TEST(dmlhl_sequence, apply_delta_delta_bad_size_0)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto create_result = dml::create_delta_result{ dml::status_code::ok, dml::comparison_result::not_equal, 0u, 0u };

    auto status = sequence.add(dml::apply_delta, dml::make_view(test_data.delta_record), dml::make_view(test_data.src1), create_result);

    ASSERT_EQ(status, dml::status_code::bad_size);
}

TEST(dmlhl_sequence, apply_delta_delta_bad_size_not_10_divided)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto create_result = dml::create_delta_result{ dml::status_code::ok, dml::comparison_result::not_equal, 0u, 9u };

    auto status = sequence.add(dml::apply_delta, dml::make_view(test_data.delta_record), dml::make_view(test_data.src1), create_result);

    ASSERT_EQ(status, dml::status_code::delta_bad_size);
}

TEST(dmlhl_sequence, apply_delta_delta_empty)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::delta(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto create_result = dml::create_delta_result{ dml::status_code::ok, dml::comparison_result::equal, 0u, 10u };

    auto status = sequence.add(dml::apply_delta, dml::make_view(test_data.delta_record), dml::make_view(test_data.src1), create_result);

    ASSERT_EQ(status, dml::status_code::delta_delta_empty);
}

TEST(dmlhl_sequence, crc_src_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::crc(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::crc, dml::make_view<uint8_t>(nullptr, test_data.length), test_data.crc_seed);

    ASSERT_EQ(status, dml::status_code::nullptr_error);
}

TEST(dmlhl_sequence, crc_bad_size)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::crc(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::crc, dml::make_view<uint8_t>(test_data.src.data(), 0u), test_data.crc_seed);

    ASSERT_EQ(status, dml::status_code::bad_size);
}

TEST(dmlhl_sequence, copy_crc_src_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::copy_crc(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status =
        sequence.add(dml::copy_crc, dml::make_view<uint8_t>(nullptr, test_data.length), dml::make_view(test_data.dst), test_data.crc_seed);

    ASSERT_EQ(status, dml::status_code::nullptr_error);
}

TEST(dmlhl_sequence, copy_crc_dst_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::copy_crc(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status =
        sequence.add(dml::copy_crc, dml::make_view(test_data.src), dml::make_view<uint8_t>(nullptr, test_data.length), test_data.crc_seed);

    ASSERT_EQ(status, dml::status_code::nullptr_error);
}

TEST(dmlhl_sequence, copy_crc_inconsistent_size)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::copy_crc(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::copy_crc,
                               dml::make_view(test_data.src),
                               dml::make_view<uint8_t>(test_data.dst.data(), test_data.length - 1u),
                               test_data.crc_seed);

    ASSERT_EQ(status, dml::status_code::inconsistent_size);
}

TEST(dmlhl_sequence, copy_crc_bad_size)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::copy_crc(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status =
        sequence.add(dml::copy_crc, dml::make_view(test_data.src.data(), 0u), dml::make_view(test_data.dst.data(), 0u), test_data.crc_seed);

    ASSERT_EQ(status, dml::status_code::bad_size);
}

TEST(dmlhl_sequence, compare_src1_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_equal(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::compare, dml::make_view<uint8_t>(nullptr, test_data.length), dml::make_view(test_data.src2));

    ASSERT_EQ(status, dml::status_code::nullptr_error);
}

TEST(dmlhl_sequence, compare_src2_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_equal(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::compare, dml::make_view(test_data.src1), dml::make_view<uint8_t>(nullptr, test_data.length));

    ASSERT_EQ(status, dml::status_code::nullptr_error);
}

TEST(dmlhl_sequence, compare_inconsistent_size)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_equal(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::compare, dml::make_view(test_data.src1), dml::make_view(test_data.src2.data(), test_data.length - 1u));

    ASSERT_EQ(status, dml::status_code::inconsistent_size);
}

TEST(dmlhl_sequence, compare_bad_size)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_equal(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::compare, dml::make_view(test_data.src1.data(), 0u), dml::make_view(test_data.src2.data(), 0u));

    ASSERT_EQ(status, dml::status_code::bad_size);
}

TEST(dmlhl_sequence, compare_pattern_src_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_pattern_equal(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::compare_pattern, test_data.pattern, dml::make_view<uint8_t>(nullptr, test_data.length));

    ASSERT_EQ(status, dml::status_code::nullptr_error);
}

TEST(dmlhl_sequence, compare_pattern_bad_size)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::compare_pattern_equal(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::compare_pattern, test_data.pattern, dml::make_view(test_data.src.data(), 0u));

    ASSERT_EQ(status, dml::status_code::bad_size);
}

TEST(dmlhl_sequence, cache_flush_dst_null)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::cache_flush(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::cache_flush, dml::make_view<uint8_t>(nullptr, test_data.length));

    ASSERT_EQ(status, dml::status_code::nullptr_error);
}

TEST(dmlhl_sequence, cache_flush_bad_size)
{
    constexpr auto length = 16u;
    constexpr auto seed   = 777u;

    auto test_data = dml::testing::cache_flush(seed, length);
    auto sequence  = dml::sequence(1u, std::allocator<int>());

    auto status = sequence.add(dml::cache_flush, dml::make_view(test_data.dst.data(), 0u));

    ASSERT_EQ(status, dml::status_code::bad_size);
}

TEST(dmlhl_sequence, bad_alloc)
{
    ASSERT_ANY_THROW(auto sequence = dml::sequence(1u, exception_allocator<int>()));
}
