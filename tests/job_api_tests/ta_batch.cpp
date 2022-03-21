/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains algorithmic tests for DML_OP_BATCH operation
 * @date 4/17/2020
 *
 */
#include "t_common.hpp"
#include "t_delta_record_feature_defines.hpp"
#include "t_random_generator.hpp"
#include "t_random_parameters.hpp"
#include "ref_crc.hpp"

namespace dml
{
    /** Default POLYNOMIAL for crc operation */
    constexpr auto DEFAULT_POLYNOMIAL = 0x8BB7u;

    /** Enables All F Detection */
    constexpr auto DIF_F_ALL_FLAG = DML_DIF_FLAG_SRC_F_DETECT_ALL | DML_DIF_FLAG_SRC_F_ENABLE_ERROR;

    /** Returns random size */
    static auto get_random_size(const uint32_t seed)
    {
        return dml::test::random_t<uint32_t>(
                   {TEST_MIN_MEMORY_BLOCK_SIZE, TEST_MAX_MEMORY_BLOCK_SIZE}, seed)
            .get_next();
    }

    /** Fills memory region with random values */
    static auto fill_with_random(uint8_t *const ptr, const uint32_t size, const uint32_t seed)
        -> void
    {
        std::generate_n(ptr, size, dml::test::random_t<uint8_t>(seed));
    }

    /** Adds a new batch job to reference job */
    static auto extend_ref_job(dml::test::reference_job_t &ref_job)
    {
        const auto prev_size = ref_job.destination_first.size();
        const auto new_size  = prev_size + sizeof(dml::test::reference_job_t);
        ref_job.destination_first.resize(new_size);

        auto ptr = ref_job.destination_first.data();
        new (ptr + prev_size) dml::test::reference_job_t{};

        const auto last_index   = prev_size / sizeof(dml::test::reference_job_t);
        const auto last_job_ptr = reinterpret_cast<dml::test::reference_job_t *>(ptr + prev_size);
        return std::tuple(last_index, last_job_ptr);
    }

    /** Adds a NOP operation to batch */
    static auto append_nop(const uint32_t              seed,
                           dml::test::job_t &          lib_job,
                           dml::test::reference_job_t &ref_job) -> void
    {
        static_cast<void>(seed);
        const auto [last_index, _] = extend_ref_job(ref_job);

        auto status = dml_batch_set_nop_by_index(&*lib_job, last_index, 0u);
        EXPECT_EQ(DML_STATUS_OK, status);
    }

    /** Adds a MEM_MOVE operation to batch */
    static auto append_mem_move(const uint32_t              seed,
                                dml::test::job_t &          lib_job,
                                dml::test::reference_job_t &ref_job) -> void
    {
        const auto [last_index, last_job_ptr] = extend_ref_job(ref_job);
        auto size                             = get_random_size(seed);

        auto source_ptr = dml::test::global_allocator::allocate_ptr(size);
        fill_with_random(source_ptr, size, seed);

        auto destination_ptr = dml::test::global_allocator::allocate_ptr(size);
        fill_with_random(destination_ptr, size, seed);

        auto status = dml_batch_set_mem_move_by_index(
            &*lib_job, last_index, source_ptr, destination_ptr, size, 0u);
        EXPECT_EQ(DML_STATUS_OK, status);

        last_job_ptr->destination_first.assign(source_ptr, source_ptr + size);
    }

    /** Adds a FILL operation to batch */
    static auto append_fill(const uint32_t              seed,
                            dml::test::job_t &          lib_job,
                            dml::test::reference_job_t &ref_job) -> void
    {
        const auto [last_index, last_job_ptr] = extend_ref_job(ref_job);
        auto size                             = get_random_size(seed);

        auto pattern_ptr = dml::test::global_allocator::allocate_ptr(sizeof(pattern_t));
        fill_with_random(pattern_ptr, sizeof(pattern_t), seed);

        auto destination_ptr = dml::test::global_allocator::allocate_ptr(size);
        fill_with_random(destination_ptr, size, seed);

        auto status = dml_batch_set_fill_by_index(
            &*lib_job, last_index, pattern_ptr, destination_ptr, size, 0u);
        EXPECT_EQ(DML_STATUS_OK, status);

        for (auto index = 0u; index < size; ++index)
        {
            last_job_ptr->destination_first.push_back(pattern_ptr[index % sizeof(pattern_t)]);
        }
    }

    /** Adds a COMPARE operation to batch */
    static auto append_compare(const uint32_t              seed,
                               dml::test::job_t &          lib_job,
                               dml::test::reference_job_t &ref_job) -> void
    {
        const auto [last_index, last_job_ptr] = extend_ref_job(ref_job);
        auto size                             = get_random_size(seed);

        auto source_first_ptr = dml::test::global_allocator::allocate_ptr(size);
        fill_with_random(source_first_ptr, size, seed);

        auto source_second_ptr = dml::test::global_allocator::allocate_ptr(size);
        fill_with_random(source_second_ptr, size, seed);

        auto status = dml_batch_set_compare_by_index(
            &*lib_job, last_index, source_first_ptr, source_second_ptr, size, 1u, 0u);
        EXPECT_EQ(DML_STATUS_OK, status);
    }

    /** Adds a COMPARE_PATTERN operation to batch */
    static auto append_compare_pattern(const uint32_t              seed,
                                       dml::test::job_t &          lib_job,
                                       dml::test::reference_job_t &ref_job) -> void
    {
        const auto [last_index, last_job_ptr] = extend_ref_job(ref_job);
        auto size                             = get_random_size(seed);

        auto source_first_ptr = dml::test::global_allocator::allocate_ptr(size);
        fill_with_random(source_first_ptr, size, seed);

        auto pattern_ptr = dml::test::global_allocator::allocate_ptr(sizeof(pattern_t));
        fill_with_random(pattern_ptr, sizeof(pattern_t), seed);

        auto status = dml_batch_set_compare_pattern_by_index(
            &*lib_job, last_index, source_first_ptr, pattern_ptr, size, 0u, 0u);
        EXPECT_EQ(DML_STATUS_OK, status);

        last_job_ptr->offset = 8u;
    }

    /** Adds a DELTA operation to batch */
    static auto append_delta_create(const uint32_t              seed,
                                    dml::test::job_t &          lib_job,
                                    dml::test::reference_job_t &ref_job) -> void
    {
        // Add CREATE
        const auto [last_index, last_job_ptr] = extend_ref_job(ref_job);

        const auto size     = get_random_size(seed) / DELTA_DATA_FIELD_SIZE;
        const auto src_size = size * DELTA_DATA_FIELD_SIZE;
        const auto dst_size = size * DELTA_NOTE_BYTE_SIZE;

        auto source_first_ptr = dml::test::global_allocator::allocate_ptr(src_size);
        fill_with_random(source_first_ptr, src_size, seed);

        auto source_second_ptr = dml::test::global_allocator::allocate_ptr(src_size);
        fill_with_random(source_second_ptr, src_size, seed + 1);

        auto destination_ptr = dml::test::global_allocator::allocate_ptr(dst_size);
        fill_with_random(destination_ptr, dst_size, seed + 2);

        auto status = dml_batch_set_delta_create_by_index(&*lib_job,
                                                          last_index,
                                                          source_first_ptr,
                                                          source_second_ptr,
                                                          src_size,
                                                          destination_ptr,
                                                          dst_size,
                                                          0u,
                                                          0u);
        EXPECT_EQ(DML_STATUS_OK, status);
    }

    /** Adds a DELTA operation to batch */
    static auto append_delta_apply(const uint32_t seed, dml::test::job_t &lib_job, dml::test::reference_job_t &ref_job)
        -> void
    {
        // Algorithm is copied from ta_apply_delta.cpp
        const auto [last_index, last_job_ptr] = extend_ref_job(ref_job);

        auto random_regions       = dml::test::random_t<uint32_t>(REGIONS_COUNT, seed);
        auto random_value         = dml::test::random_t<uint8_t>(seed);
        const auto regions_count  = random_regions.get_next();
        const auto length         = (regions_count * DELTA_DATA_FIELD_SIZE);
        const auto mismatch_count = 1u + (random_regions.get_next() % regions_count);
        auto mismatch_indices     = std::vector<uint32_t>();
        auto &mismatch_buffer     = dml::test::global_allocator::allocate();


        // Library job initialization
        auto destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        auto destination_length    = length;

        // Vectors initialization
        std::generate(destination_first_ptr,
            destination_first_ptr + destination_length,
            random_value);


        // Generate mismatches
        mismatch_indices.reserve(mismatch_count);
        mismatch_buffer.reserve(mismatch_count * DELTA_NOTE_BYTE_SIZE);

        for (auto i = 0u; i < mismatch_count; ++i)
        {
            const auto mismatch_region = (random_regions.get_next() % regions_count);

            if (mismatch_indices.end() == std::find(mismatch_indices.begin(),
                mismatch_indices.end(),
                mismatch_region))
            {
                mismatch_indices.push_back(mismatch_region);
            }
        }

        // We have to sort indices to form
        // correct reference stream
        std::sort(mismatch_indices.begin(), mismatch_indices.end());


        // Reference job initialization
        last_job_ptr->destination_first = dml::test::global_allocator::allocate(
            destination_first_ptr,
            destination_first_ptr +
            destination_length);


        // Add mismatches
        for (const auto &index : mismatch_indices)
        {
            auto delta_data_field         = std::array<uint8_t, 10u>();
            const auto mismatch_region    = (index * DELTA_DATA_FIELD_SIZE);
            const auto inner_index        = (random_regions.get_next() % DELTA_DATA_FIELD_SIZE);
            const auto mismatch_position  = (mismatch_region + inner_index);
            const auto reference_position = &destination_first_ptr[mismatch_region];
            const auto region_index       = *reinterpret_cast<const uint16_t *>(&index);
            const auto reference_value    = *reinterpret_cast<const uint64_t *>(reference_position);

            *reinterpret_cast<uint16_t *>(&delta_data_field[0]) = region_index;
            *reinterpret_cast<uint64_t *>(&delta_data_field[2]) = reference_value;

            // Put reference element to the destination vector
            mismatch_buffer.insert(mismatch_buffer.end(),
                delta_data_field.begin(),
                delta_data_field.end());

            // Corrupt element in mask
            destination_first_ptr[mismatch_position] =
                ~destination_first_ptr[mismatch_position];
        }

        // Set library job buffers
        auto source_first_ptr = mismatch_buffer.data();
        auto source_length    = static_cast<uint32_t>(mismatch_buffer.size());

        auto status = dml_batch_set_delta_apply_by_index(
            &*lib_job, last_index, source_first_ptr, source_length, destination_first_ptr, destination_length, 0u);
        EXPECT_EQ(DML_STATUS_OK, status);
    }

    /** Adds a DUALCAST operation to batch */
    static auto append_dualcast(const uint32_t              seed,
                                dml::test::job_t &          lib_job,
                                dml::test::reference_job_t &ref_job) -> void
    {
        constexpr auto alignment              = 0x1000u;
        const auto [last_index, last_job_ptr] = extend_ref_job(ref_job);
        auto size                             = get_random_size(seed);
        auto need_to_align                    = alignment + (size / alignment) * alignment;
        auto buffer_size                      = size * 2u + need_to_align;

        auto source_ptr = dml::test::global_allocator::allocate_ptr(size);
        fill_with_random(source_ptr, size, seed);

        auto destination_ptr = dml::test::global_allocator::allocate_ptr(buffer_size);
        fill_with_random(destination_ptr, buffer_size, seed);

        auto status = dml_batch_set_dualcast_by_index(&*lib_job,
                                                      last_index,
                                                      source_ptr,
                                                      destination_ptr,
                                                      destination_ptr + need_to_align,
                                                      size,
                                                      0u);
        EXPECT_EQ(DML_STATUS_OK, status);

        last_job_ptr->destination_first.assign(source_ptr, source_ptr + size);
        last_job_ptr->destination_second.assign(source_ptr, source_ptr + size);
    }

    /** Adds a CRC operation to batch */
    static auto append_crc(const uint32_t              seed,
                           dml::test::job_t &          lib_job,
                           dml::test::reference_job_t &ref_job) -> void
    {
        const auto [last_index, last_job_ptr] = extend_ref_job(ref_job);
        auto size                             = get_random_size(seed);

        auto source_ptr = dml::test::global_allocator::allocate_ptr(size);
        fill_with_random(source_ptr, size, seed);

        auto crc_ptr = dml::test::global_allocator::allocate_ptr(sizeof(uint32_t));
        *crc_ptr = 0;

        auto status = dml_batch_set_crc_by_index(
            &*lib_job, last_index, source_ptr, size, reinterpret_cast<uint32_t *>(crc_ptr), 0u);
        EXPECT_EQ(DML_STATUS_OK, status);

        last_job_ptr->destination_first.assign(source_ptr, source_ptr + size);
        last_job_ptr->crc_checksum_ptr = std::make_unique<uint32_t>(
            dml::reference::calculate_crc<uint32_t, 0u>(source_ptr, source_ptr + size, (*crc_ptr)));
    }

    /** Adds a CRC_COPY operation to batch */
    static auto append_crc_copy(const uint32_t              seed,
                                dml::test::job_t &          lib_job,
                                dml::test::reference_job_t &ref_job) -> void
    {
        const auto [last_index, last_job_ptr] = extend_ref_job(ref_job);
        auto size                             = get_random_size(seed);

        auto source_ptr = dml::test::global_allocator::allocate_ptr(size);
        fill_with_random(source_ptr, size, seed);

        auto destination_ptr = dml::test::global_allocator::allocate_ptr(size);
        fill_with_random(destination_ptr, size, seed);

        auto crc_ptr = dml::test::global_allocator::allocate_ptr(sizeof(uint32_t));
        *crc_ptr = 0;

        auto status = dml_batch_set_copy_crc_by_index(&*lib_job,
                                                      last_index,
                                                      source_ptr,
                                                      size,
                                                      reinterpret_cast<uint32_t *>(crc_ptr),
                                                      destination_ptr,
                                                      0u);
        EXPECT_EQ(DML_STATUS_OK, status);

        last_job_ptr->crc_checksum_ptr = std::make_unique<uint32_t>(
            dml::reference::calculate_crc<uint32_t, 0u>(source_ptr, source_ptr + size, (*crc_ptr)));
    }

    /** Adds a CACHE_FLUSH operation to batch */
    static auto append_flush(const uint32_t              seed,
                            dml::test::job_t &          lib_job,
                            dml::test::reference_job_t &ref_job) -> void
    {
        const auto [last_index, last_job_ptr] = extend_ref_job(ref_job);
        auto size                             = get_random_size(seed);

        auto destination_ptr = dml::test::global_allocator::allocate_ptr(size);
        fill_with_random(destination_ptr, size, seed);

        auto status = dml_batch_set_cache_flush_by_index(
            &*lib_job, last_index, destination_ptr, size, 0u);
        EXPECT_EQ(DML_STATUS_OK, status);
    }

    /**
     * @brief Tests the operation with all operations
     */
    DML_JOB_API_TEST_GENERATOR(dml_batch, ta_all_operations)
    {
        constexpr auto operation_count = 11u;
        auto           batch_size      = 0u;

        auto status = dml_get_batch_size(&*lib_job, operation_count, &batch_size);
        EXPECT_EQ(DML_STATUS_OK, status);

        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(batch_size);
        lib_job->destination_length    = batch_size;
        lib_job->operation             = DML_OP_BATCH;

        const auto seed = test_system::get_seed();
        append_nop(seed, lib_job, ref_job);
        append_mem_move(seed, lib_job, ref_job);
        append_fill(seed, lib_job, ref_job);
        append_compare(seed, lib_job, ref_job);
        append_compare_pattern(seed, lib_job, ref_job);
        append_delta_create(seed, lib_job, ref_job);
        append_delta_apply(seed, lib_job, ref_job);
        append_dualcast(seed, lib_job, ref_job);
        append_crc(seed, lib_job, ref_job);
        append_crc_copy(seed, lib_job, ref_job);
        append_flush(seed, lib_job, ref_job);
    }

    // Test registers
    DML_JOB_API_TEST_REGISTER(dml_batch, ta_all_operations);

}  // namespace dml
