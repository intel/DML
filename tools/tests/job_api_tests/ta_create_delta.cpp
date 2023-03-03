/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains algorithmic tests for DML_OP_DELTA_CREATE operation
 * @date 4/17/2020
 *
 */
#include "t_common.hpp"
#include "t_random_generator.hpp"
#include "t_delta_record_feature_defines.hpp"


namespace dml
{

    /**
     * @brief Tests the operation with equal vectors
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, ta_equal_vectors)
    {
        // Variables
        const auto seed          = test_system::get_seed();
        auto random_regions      = dml::test::random_t<uint32_t>(REGIONS_COUNT, seed);
        auto random_value        = dml::test::random_t<uint8_t>(seed);
        const auto regions_count = random_regions.get_next();
        const auto length        = regions_count * DELTA_DATA_FIELD_SIZE;
        const auto max_size      = regions_count * DELTA_NOTE_BYTE_SIZE;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr,
                      lib_job->source_first_ptr + lib_job->source_length,
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr,
                                        lib_job->source_first_ptr + lib_job->source_length);
        ref_job.result = 0u;
    }


    /**
     * @brief Tests the operation with not equal vectors
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, ta_not_equal_vectors)
    {
        // Variables
        const auto seed            = test_system::get_seed();
        auto random_regions        = dml::test::random_t<uint32_t>(REGIONS_COUNT, seed);
        auto random_value          = dml::test::random_t<uint8_t>(seed);
        const auto regions_count   = random_regions.get_next();
        const auto length          = (regions_count * DELTA_DATA_FIELD_SIZE);
        const auto max_size        = (regions_count * DELTA_NOTE_BYTE_SIZE);
        auto random_mismatch_count = dml::test::random_t<uint32_t>({1u, regions_count - 1u}, seed);
        const auto mismatch_count  = random_mismatch_count.get_next();
        auto mismatch_indices      = std::vector<uint32_t>();


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr,
                      lib_job->source_first_ptr + lib_job->source_length,
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr,
                                        lib_job->source_first_ptr + lib_job->source_length);


        // Generate mismatches
        auto random_mismatch = dml::test::random_t<uint32_t>({0u, regions_count - 1u}, seed);
        mismatch_indices.reserve(mismatch_count);

        for (auto i = 0u; i < mismatch_count;)
        {
            const auto mismatch_region = random_mismatch.get_next();

            if (mismatch_indices.end() == std::find(mismatch_indices.begin(),
                                                    mismatch_indices.end(),
                                                    mismatch_region))
            {
                mismatch_indices.push_back(mismatch_region);
                ++i;
            }
        }

        // We have to sort indices to form
        // correct reference stream
        std::sort(mismatch_indices.begin(), mismatch_indices.end());


        // Reference job initialization
        ref_job.destination_first.reserve(mismatch_indices.size() * DELTA_NOTE_BYTE_SIZE);

        // Put correct result
        // and first mismatch index
        ref_job.result = 1u;
        ref_job.offset = mismatch_indices.front();


        // Reference vector initialization
        for (const auto &index : mismatch_indices)
        {
            auto delta_data_field         = std::array<uint8_t, 10u>();
            const auto mismatch_region    = (index * DELTA_DATA_FIELD_SIZE);
            const auto inner_index        = (random_regions.get_next() % DELTA_DATA_FIELD_SIZE);
            const auto mismatch_position  = (mismatch_region + inner_index);
            const auto reference_position = &lib_job->source_second_ptr[mismatch_region];
            const auto region_index       = *reinterpret_cast<const uint16_t *>(&index);
            const auto reference_value    = *reinterpret_cast<const uint64_t *>(reference_position);

            *reinterpret_cast<uint16_t *>(&delta_data_field[0]) = region_index;
            *reinterpret_cast<uint64_t *>(&delta_data_field[2]) = reference_value;

            // Put reference element to the destination vector
            ref_job.destination_first.insert(ref_job.destination_first.end(),
                                             delta_data_field.begin(),
                                             delta_data_field.end());

            // Corrupt element in mask
            lib_job->source_first_ptr[mismatch_position] =
                ~lib_job->source_first_ptr[mismatch_position];
        }
    }


    /**
     * @brief Tests the operation with equal vectors, expected equal
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, ta_equal_vectors_expected_equal)
    {
        // Variables
        const auto seed          = test_system::get_seed();
        auto random_regions      = dml::test::random_t<uint32_t>(REGIONS_COUNT, seed);
        auto random_value        = dml::test::random_t<uint8_t>(seed);
        const auto regions_count = random_regions.get_next();
        const auto length        = regions_count * DELTA_DATA_FIELD_SIZE;
        const auto max_size      = regions_count * DELTA_NOTE_BYTE_SIZE;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;
        lib_job->flags                 = DML_FLAG_CHECK_RESULT;
        lib_job->expected_result       = DML_FLAG_EXPECT_EQUAL;


        ref_job.result = 0u;

        // Vectors initialization
        std::generate(lib_job->source_first_ptr,
                      lib_job->source_first_ptr + lib_job->source_length,
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr,
                                        lib_job->source_first_ptr + lib_job->source_length);

    }


    /**
     * @brief Tests the operation with equal vectors, expected not equal
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, ta_equal_vectors_expected_not_equal)
    {
        // Variables
        const auto seed          = test_system::get_seed();
        auto random_regions      = dml::test::random_t<uint32_t>(REGIONS_COUNT, seed);
        auto random_value        = dml::test::random_t<uint8_t>(seed);
        const auto regions_count = random_regions.get_next();
        const auto length        = regions_count * DELTA_DATA_FIELD_SIZE;
        const auto max_size      = regions_count * DELTA_NOTE_BYTE_SIZE;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;
        lib_job->flags                 = DML_FLAG_CHECK_RESULT;
        lib_job->expected_result       = DML_FLAG_EXPECT_NOT_EQUAL;

        ref_job.result = 0;

        // Vectors initialization
        std::generate(lib_job->source_first_ptr,
                      lib_job->source_first_ptr + lib_job->source_length,
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr,
                                        lib_job->source_first_ptr + lib_job->source_length);

        // Reference job initialization
        ref_job.status = DML_STATUS_FALSE_PREDICATE_OK;
    }


    /**
     * @brief Tests the operation with equal vectors, expected overflow
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, ta_equal_vectors_expected_overflow)
    {
        // Variables
        const auto seed          = test_system::get_seed();
        auto random_regions      = dml::test::random_t<uint32_t>(REGIONS_COUNT, seed);
        auto random_value        = dml::test::random_t<uint8_t>(seed);
        const auto regions_count = random_regions.get_next();
        const auto length        = regions_count * DELTA_DATA_FIELD_SIZE;
        const auto max_size      = regions_count * DELTA_NOTE_BYTE_SIZE;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;
        lib_job->flags                 = DML_FLAG_CHECK_RESULT;
        lib_job->expected_result       = DML_FLAG_EXPECT_OVERFLOW;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr,
                      lib_job->source_first_ptr + lib_job->source_length,
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr,
                                        lib_job->source_first_ptr + lib_job->source_length);

        // Reference job initialization
        ref_job.status = DML_STATUS_FALSE_PREDICATE_OK;
        ref_job.result = 0u;
    }


    /**
     * @brief Tests the operation with bad check result flag
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, ta_equal_vectors_bad_check_result)
    {
        // Variables
        const auto seed          = test_system::get_seed();
        auto random_regions      = dml::test::random_t<uint32_t>(REGIONS_COUNT, seed);
        auto random_result       = dml::test::random_t<uint32_t>(seed);
        auto random_value        = dml::test::random_t<uint8_t>(seed);
        const auto regions_count = random_regions.get_next();
        const auto length        = regions_count * DELTA_DATA_FIELD_SIZE;
        const auto max_size      = regions_count * DELTA_NOTE_BYTE_SIZE;
        const auto all_results   = DML_FLAG_EXPECT_EQUAL |
                                   DML_FLAG_EXPECT_NOT_EQUAL |
                                   DML_FLAG_EXPECT_OVERFLOW;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;
        lib_job->flags                 = DML_FLAG_CHECK_RESULT;
        lib_job->expected_result       = random_result.get_next() & ~all_results;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr,
                      lib_job->source_first_ptr + lib_job->source_length,
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr,
                                        lib_job->source_first_ptr + lib_job->source_length);

        // Reference job initialization
        ref_job.status = DML_STATUS_FALSE_PREDICATE_OK;
        ref_job.result = 0;
    }


    /**
     * @brief Tests the operation with not equal vectors, expected equal
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, ta_not_equal_vectors_expected_equal)
    {
        // Variables
        const auto seed            = test_system::get_seed();
        auto random_regions        = dml::test::random_t<uint32_t>(REGIONS_COUNT, seed);
        auto random_value          = dml::test::random_t<uint8_t>(seed);
        const auto regions_count   = random_regions.get_next();
        const auto length          = (regions_count * DELTA_DATA_FIELD_SIZE);
        const auto max_size        = (regions_count * DELTA_NOTE_BYTE_SIZE);
        auto random_mismatch_count = dml::test::random_t<uint32_t>({1u, regions_count - 1u}, seed);
        const auto mismatch_count  = random_mismatch_count.get_next();
        auto mismatch_indices      = std::vector<uint32_t>();


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;
        lib_job->flags                 = DML_FLAG_CHECK_RESULT;
        lib_job->expected_result       = DML_FLAG_EXPECT_EQUAL;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr,
                      lib_job->source_first_ptr + lib_job->source_length,
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr,
                                        lib_job->source_first_ptr + lib_job->source_length);


        // Generate mismatches
        auto random_mismatch = dml::test::random_t<uint32_t>({0u, regions_count - 1u}, seed);
        mismatch_indices.reserve(mismatch_count);

        for (auto i = 0u; i < mismatch_count;)
        {
            const auto mismatch_region = random_mismatch.get_next();

            if (mismatch_indices.end() == std::find(mismatch_indices.begin(),
                                                    mismatch_indices.end(),
                                                    mismatch_region))
            {
                mismatch_indices.push_back(mismatch_region);
                ++i;
            }
        }

        // We have to sort indices to form
        // correct reference stream
        std::sort(mismatch_indices.begin(), mismatch_indices.end());


        // Reference job initialization
        ref_job.status = DML_STATUS_FALSE_PREDICATE_OK;
        ref_job.destination_first.reserve(mismatch_indices.size() * DELTA_NOTE_BYTE_SIZE);

        // Put correct result
        // and first mismatch index
        ref_job.result = 1u;
        ref_job.offset = mismatch_indices.front();


        // Reference vector initialization
        for (const auto &index : mismatch_indices)
        {
            auto delta_data_field         = std::array<uint8_t, 10u>();
            const auto mismatch_region    = (index * DELTA_DATA_FIELD_SIZE);
            const auto inner_index        = (random_regions.get_next() % DELTA_DATA_FIELD_SIZE);
            const auto mismatch_position  = (mismatch_region + inner_index);
            const auto reference_position = &lib_job->source_second_ptr[mismatch_region];
            const auto region_index       = *reinterpret_cast<const uint16_t *>(&index);
            const auto reference_value    = *reinterpret_cast<const uint64_t *>(reference_position);

            *reinterpret_cast<uint16_t *>(&delta_data_field[0]) = region_index;
            *reinterpret_cast<uint64_t *>(&delta_data_field[2]) = reference_value;

            // Put reference element to the destination vector
            ref_job.destination_first.insert(ref_job.destination_first.end(),
                                             delta_data_field.begin(),
                                             delta_data_field.end());

            // Corrupt element in mask
            lib_job->source_first_ptr[mismatch_position] =
                ~lib_job->source_first_ptr[mismatch_position];
        }
    }


    /**
     * @brief Tests the operation with not equal vectors, expected not equal
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, ta_not_equal_vectors_expected_not_equal)
    {
        // Variables
        const auto seed            = test_system::get_seed();
        auto random_regions        = dml::test::random_t<uint32_t>(REGIONS_COUNT, seed);
        auto random_value          = dml::test::random_t<uint8_t>(seed);
        const auto regions_count   = random_regions.get_next();
        const auto length          = (regions_count * DELTA_DATA_FIELD_SIZE);
        const auto max_size        = (regions_count * DELTA_NOTE_BYTE_SIZE);
        auto random_mismatch_count = dml::test::random_t<uint32_t>({1u, regions_count - 1u}, seed);
        const auto mismatch_count  = random_mismatch_count.get_next();
        auto mismatch_indices      = std::vector<uint32_t>();


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;
        lib_job->flags                 = DML_FLAG_CHECK_RESULT;
        lib_job->expected_result       = DML_FLAG_EXPECT_NOT_EQUAL;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr,
                      lib_job->source_first_ptr + lib_job->source_length,
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr,
                                        lib_job->source_first_ptr + lib_job->source_length);


        // Generate mismatches
        auto random_mismatch = dml::test::random_t<uint32_t>({0u, regions_count - 1u}, seed);
        mismatch_indices.reserve(mismatch_count);

        for (auto i = 0u; i < mismatch_count;)
        {
            const auto mismatch_region = random_mismatch.get_next();

            if (mismatch_indices.end() == std::find(mismatch_indices.begin(),
                                                    mismatch_indices.end(),
                                                    mismatch_region))
            {
                mismatch_indices.push_back(mismatch_region);
                ++i;
            }
        }

        // We have to sort indices to form
        // correct reference stream
        std::sort(mismatch_indices.begin(), mismatch_indices.end());


        // Reference job initialization
        ref_job.destination_first.reserve(mismatch_indices.size() * DELTA_NOTE_BYTE_SIZE);

        // Put correct result
        // and first mismatch index
        ref_job.result = 1u;
        ref_job.offset = mismatch_indices.front();


        // Reference vector initialization
        for (const auto &index : mismatch_indices)
        {
            auto delta_data_field         = std::array<uint8_t, 10u>();
            const auto mismatch_region    = (index * DELTA_DATA_FIELD_SIZE);
            const auto inner_index        = (random_regions.get_next() % DELTA_DATA_FIELD_SIZE);
            const auto mismatch_position  = (mismatch_region + inner_index);
            const auto reference_position = &lib_job->source_second_ptr[mismatch_region];
            const auto region_index       = *reinterpret_cast<const uint16_t *>(&index);
            const auto reference_value    = *reinterpret_cast<const uint64_t *>(reference_position);

            *reinterpret_cast<uint16_t *>(&delta_data_field[0]) = region_index;
            *reinterpret_cast<uint64_t *>(&delta_data_field[2]) = reference_value;

            // Put reference element to the destination vector
            ref_job.destination_first.insert(ref_job.destination_first.end(),
                                             delta_data_field.begin(),
                                             delta_data_field.end());

            // Corrupt element in mask
            lib_job->source_first_ptr[mismatch_position] =
                ~lib_job->source_first_ptr[mismatch_position];
        }
    }


    /**
     * @brief Tests the operation with not equal vectors, expected overflow
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, ta_not_equal_vectors_expected_overflow)
    {
        // Variables
        const auto seed            = test_system::get_seed();
        auto random_regions        = dml::test::random_t<uint32_t>(REGIONS_COUNT, seed);
        auto random_value          = dml::test::random_t<uint8_t>(seed);
        const auto regions_count   = random_regions.get_next();
        const auto length          = (regions_count * DELTA_DATA_FIELD_SIZE);
        const auto max_size        = (regions_count * DELTA_NOTE_BYTE_SIZE);
        auto random_mismatch_count = dml::test::random_t<uint32_t>({1u, regions_count - 1u}, seed);
        const auto mismatch_count  = random_mismatch_count.get_next();
        auto mismatch_indices      = std::vector<uint32_t>();


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;
        lib_job->flags                 = DML_FLAG_CHECK_RESULT;
        lib_job->expected_result       = DML_FLAG_EXPECT_OVERFLOW;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr,
                      lib_job->source_first_ptr + lib_job->source_length,
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr,
                                        lib_job->source_first_ptr + lib_job->source_length);


        // Generate mismatches
        auto random_mismatch = dml::test::random_t<uint32_t>({0u, regions_count - 1u}, seed);
        mismatch_indices.reserve(mismatch_count);

        for (auto i = 0u; i < mismatch_count;)
        {
            const auto mismatch_region = random_mismatch.get_next();

            if (mismatch_indices.end() == std::find(mismatch_indices.begin(),
                                                    mismatch_indices.end(),
                                                    mismatch_region))
            {
                mismatch_indices.push_back(mismatch_region);
                ++i;
            }
        }

        // We have to sort indices to form
        // correct reference stream
        std::sort(mismatch_indices.begin(), mismatch_indices.end());


        // Reference job initialization
        ref_job.status = DML_STATUS_FALSE_PREDICATE_OK;
        ref_job.destination_first.reserve(mismatch_indices.size() * DELTA_NOTE_BYTE_SIZE);

        // Put correct result
        // and first mismatch index
        ref_job.result = 1u;
        ref_job.offset = mismatch_indices.front();


        // Reference vector initialization
        for (const auto &index : mismatch_indices)
        {
            auto delta_data_field         = std::array<uint8_t, 10u>();
            const auto mismatch_region    = (index * DELTA_DATA_FIELD_SIZE);
            const auto inner_index        = (random_regions.get_next() % DELTA_DATA_FIELD_SIZE);
            const auto mismatch_position  = (mismatch_region + inner_index);
            const auto reference_position = &lib_job->source_second_ptr[mismatch_region];
            const auto region_index       = *reinterpret_cast<const uint16_t *>(&index);
            const auto reference_value    = *reinterpret_cast<const uint64_t *>(reference_position);

            *reinterpret_cast<uint16_t *>(&delta_data_field[0]) = region_index;
            *reinterpret_cast<uint64_t *>(&delta_data_field[2]) = reference_value;

            // Put reference element to the destination vector
            ref_job.destination_first.insert(ref_job.destination_first.end(),
                                             delta_data_field.begin(),
                                             delta_data_field.end());

            // Corrupt element in mask
            lib_job->source_first_ptr[mismatch_position] =
                ~lib_job->source_first_ptr[mismatch_position];
        }
    }


    /**
     * @brief Tests the operation with not equal vectors and bad expected result flag
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, ta_not_equal_vectors_bad_check_result)
    {
        // Variables
        const auto seed            = test_system::get_seed();
        auto random_regions        = dml::test::random_t<uint32_t>(REGIONS_COUNT, seed);
        auto random_result         = dml::test::random_t<uint32_t>(seed);
        auto random_value          = dml::test::random_t<uint8_t>(seed);
        const auto regions_count   = random_regions.get_next();
        const auto length          = (regions_count * DELTA_DATA_FIELD_SIZE);
        const auto max_size        = (regions_count * DELTA_NOTE_BYTE_SIZE);
        auto random_mismatch_count = dml::test::random_t<uint32_t>({1u, regions_count - 1u}, seed);
        const auto mismatch_count  = random_mismatch_count.get_next();
        const auto all_results     = DML_FLAG_EXPECT_EQUAL |
                                     DML_FLAG_EXPECT_NOT_EQUAL |
                                     DML_FLAG_EXPECT_OVERFLOW;
        auto mismatch_indices      = std::vector<uint32_t>();


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;
        lib_job->flags                 = DML_FLAG_CHECK_RESULT;
        lib_job->expected_result       = random_result.get_next() & ~all_results;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr,
                      lib_job->source_first_ptr + lib_job->source_length,
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr,
                                        lib_job->source_first_ptr + lib_job->source_length);


        // Generate mismatches
        auto random_mismatch = dml::test::random_t<uint32_t>({0u, regions_count - 1u}, seed);
        mismatch_indices.reserve(mismatch_count);

        for (auto i = 0u; i < mismatch_count;)
        {
            const auto mismatch_region = random_mismatch.get_next();

            if (mismatch_indices.end() == std::find(mismatch_indices.begin(),
                                                    mismatch_indices.end(),
                                                    mismatch_region))
            {
                mismatch_indices.push_back(mismatch_region);
                ++i;
            }
        }

        // We have to sort indices to form
        // correct reference stream
        std::sort(mismatch_indices.begin(), mismatch_indices.end());


        // Reference job initialization
        ref_job.status = DML_STATUS_FALSE_PREDICATE_OK;
        ref_job.destination_first.reserve(mismatch_indices.size() * DELTA_NOTE_BYTE_SIZE);

        // Put correct result
        // and first mismatch index
        ref_job.result = 1u;
        ref_job.offset = mismatch_indices.front();


        // Reference vector initialization
        for (const auto &index : mismatch_indices)
        {
            auto delta_data_field         = std::array<uint8_t, 10u>();
            const auto mismatch_region    = (index * DELTA_DATA_FIELD_SIZE);
            const auto inner_index        = (random_regions.get_next() % DELTA_DATA_FIELD_SIZE);
            const auto mismatch_position  = (mismatch_region + inner_index);
            const auto reference_position = &lib_job->source_second_ptr[mismatch_region];
            const auto region_index       = *reinterpret_cast<const uint16_t *>(&index);
            const auto reference_value    = *reinterpret_cast<const uint64_t *>(reference_position);

            *reinterpret_cast<uint16_t *>(&delta_data_field[0]) = region_index;
            *reinterpret_cast<uint64_t *>(&delta_data_field[2]) = reference_value;

            // Put reference element to the destination vector
            ref_job.destination_first.insert(ref_job.destination_first.end(),
                                             delta_data_field.begin(),
                                             delta_data_field.end());

            // Corrupt element in mask
            lib_job->source_first_ptr[mismatch_position] =
                ~lib_job->source_first_ptr[mismatch_position];
        }
    }



    /**
     * @brief Tests the operation with overflowed destination vector, expected equal
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, ta_overflow_expected_equal)
    {
        // Variables
        const auto seed            = test_system::get_seed();
        auto random_regions        = dml::test::random_t<uint32_t>(REGIONS_COUNT, seed);
        auto random_value          = dml::test::random_t<uint8_t>(seed);
        const auto regions_count   = random_regions.get_next();
        const auto length          = (regions_count * DELTA_DATA_FIELD_SIZE);
        const auto max_size        = (regions_count * DELTA_NOTE_BYTE_SIZE);
        auto random_mismatch_count = dml::test::random_t<uint32_t>({16u, regions_count - 1u}, seed);
        const auto mismatch_count  = random_mismatch_count.get_next();
        auto mismatch_indices      = std::vector<uint32_t>();


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;
        lib_job->flags                 = DML_FLAG_CHECK_RESULT;
        lib_job->expected_result       = DML_FLAG_EXPECT_EQUAL;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr,
                      lib_job->source_first_ptr + lib_job->source_length,
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr,
                                        lib_job->source_first_ptr + lib_job->source_length);


        // Generate mismatches
        auto random_mismatch = dml::test::random_t<uint32_t>({0u, regions_count - 1u}, seed);
        mismatch_indices.reserve(mismatch_count);

        for (auto i = 0u; i < mismatch_count;)
        {
            const auto mismatch_region = random_mismatch.get_next();

            if (mismatch_indices.end() == std::find(mismatch_indices.begin(),
                                                    mismatch_indices.end(),
                                                    mismatch_region))
            {
                mismatch_indices.push_back(mismatch_region);
                ++i;
            }
        }

        // We have to sort indices to form
        // correct reference stream
        std::sort(mismatch_indices.begin(), mismatch_indices.end());


        // Reference job initialization
        ref_job.status = DML_STATUS_FALSE_PREDICATE_OK;
        ref_job.destination_first.reserve(mismatch_indices.size() * DELTA_NOTE_BYTE_SIZE);

        // Put correct result
        // and first mismatch index
        ref_job.result = 2u;
        ref_job.offset = mismatch_indices.front();


        // Reference vector initialization
        for (const auto &index : mismatch_indices)
        {
            auto delta_data_field         = std::array<uint8_t, 10u>();
            const auto mismatch_region    = (index * DELTA_DATA_FIELD_SIZE);
            const auto inner_index        = (random_regions.get_next() % DELTA_DATA_FIELD_SIZE);
            const auto mismatch_position  = (mismatch_region + inner_index);
            const auto reference_position = &lib_job->source_second_ptr[mismatch_region];
            const auto region_index       = *reinterpret_cast<const uint16_t *>(&index);
            const auto reference_value    = *reinterpret_cast<const uint64_t *>(reference_position);

            *reinterpret_cast<uint16_t *>(&delta_data_field[0]) = region_index;
            *reinterpret_cast<uint64_t *>(&delta_data_field[2]) = reference_value;

            // Put reference element to the destination vector
            ref_job.destination_first.insert(ref_job.destination_first.end(),
                                             delta_data_field.begin(),
                                             delta_data_field.end());

            // Corrupt element in mask
            lib_job->source_first_ptr[mismatch_position] =
                ~lib_job->source_first_ptr[mismatch_position];
        }

        // Resize vectors for overflow
        lib_job->destination_length = static_cast<uint32_t>((mismatch_indices.size() / 2u) * DELTA_NOTE_BYTE_SIZE);
        ref_job.destination_first.resize((mismatch_indices.size() / 2u) * DELTA_NOTE_BYTE_SIZE);
    }


    /**
     * @brief Tests the operation with overflowed destination vector, expected not equal
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, ta_overflow_expected_not_equal)
    {
        // Variables
        const auto seed            = test_system::get_seed();
        auto random_regions        = dml::test::random_t<uint32_t>(REGIONS_COUNT, seed);
        auto random_value          = dml::test::random_t<uint8_t>(seed);
        const auto regions_count   = random_regions.get_next();
        const auto length          = (regions_count * DELTA_DATA_FIELD_SIZE);
        const auto max_size        = (regions_count * DELTA_NOTE_BYTE_SIZE);
        auto random_mismatch_count = dml::test::random_t<uint32_t>({16u, regions_count - 1u}, seed);
        const auto mismatch_count  = random_mismatch_count.get_next();
        auto mismatch_indices      = std::vector<uint32_t>();


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;
        lib_job->flags                 = DML_FLAG_CHECK_RESULT;
        lib_job->expected_result       = DML_FLAG_EXPECT_NOT_EQUAL;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr,
                      lib_job->source_first_ptr + lib_job->source_length,
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr,
                                        lib_job->source_first_ptr + lib_job->source_length);


        // Generate mismatches
        auto random_mismatch = dml::test::random_t<uint32_t>({0u, regions_count - 1u}, seed);
        mismatch_indices.reserve(mismatch_count);

        for (auto i = 0u; i < mismatch_count;)
        {
            const auto mismatch_region = random_mismatch.get_next();

            if (mismatch_indices.end() == std::find(mismatch_indices.begin(),
                                                    mismatch_indices.end(),
                                                    mismatch_region))
            {
                mismatch_indices.push_back(mismatch_region);
                ++i;
            }
        }

        // We have to sort indices to form
        // correct reference stream
        std::sort(mismatch_indices.begin(), mismatch_indices.end());


        // Reference job initialization
        ref_job.status = DML_STATUS_FALSE_PREDICATE_OK;
        ref_job.destination_first.reserve(mismatch_indices.size() * DELTA_NOTE_BYTE_SIZE);

        // Put correct result
        // and first mismatch index
        ref_job.result = 2u;
        ref_job.offset = mismatch_indices.front();


        // Reference vector initialization
        for (const auto &index : mismatch_indices)
        {
            auto delta_data_field         = std::array<uint8_t, 10u>();
            const auto mismatch_region    = (index * DELTA_DATA_FIELD_SIZE);
            const auto inner_index        = (random_regions.get_next() % DELTA_DATA_FIELD_SIZE);
            const auto mismatch_position  = (mismatch_region + inner_index);
            const auto reference_position = &lib_job->source_second_ptr[mismatch_region];
            const auto region_index       = *reinterpret_cast<const uint16_t *>(&index);
            const auto reference_value    = *reinterpret_cast<const uint64_t *>(reference_position);

            *reinterpret_cast<uint16_t *>(&delta_data_field[0]) = region_index;
            *reinterpret_cast<uint64_t *>(&delta_data_field[2]) = reference_value;

            // Put reference element to the destination vector
            ref_job.destination_first.insert(ref_job.destination_first.end(),
                                             delta_data_field.begin(),
                                             delta_data_field.end());

            // Corrupt element in mask
            lib_job->source_first_ptr[mismatch_position] =
                ~lib_job->source_first_ptr[mismatch_position];
        }

        // Resize vectors for overflow
        lib_job->destination_length = static_cast<uint32_t>((mismatch_indices.size() / 2u) * DELTA_NOTE_BYTE_SIZE);
        ref_job.destination_first.resize((mismatch_indices.size() / 2u) * DELTA_NOTE_BYTE_SIZE);
    }


    /**
     * @brief Tests the operation with overflowed destination vector, expected overflow
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_create, ta_overflow_expected_overflow)
    {
        // Variables
        const auto seed            = test_system::get_seed();
        auto random_regions        = dml::test::random_t<uint32_t>(REGIONS_COUNT, seed);
        auto random_value          = dml::test::random_t<uint8_t>(seed);
        const auto regions_count   = random_regions.get_next();
        const auto length          = (regions_count * DELTA_DATA_FIELD_SIZE);
        const auto max_size        = (regions_count * DELTA_NOTE_BYTE_SIZE);
        auto random_mismatch_count = dml::test::random_t<uint32_t>({16u, regions_count - 1u}, seed);
        const auto mismatch_count  = random_mismatch_count.get_next();
        auto mismatch_indices      = std::vector<uint32_t>();


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(max_size);
        lib_job->destination_length    = max_size;
        lib_job->operation             = DML_OP_DELTA_CREATE;
        lib_job->flags                 = DML_FLAG_CHECK_RESULT;
        lib_job->expected_result       = DML_FLAG_EXPECT_OVERFLOW;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr,
                      lib_job->source_first_ptr + lib_job->source_length,
                      random_value);
        lib_job->source_second_ptr = dml::test::global_allocator::allocate_ptr(
                                        lib_job->source_first_ptr,
                                        lib_job->source_first_ptr + lib_job->source_length);


        // Generate mismatches
        auto random_mismatch = dml::test::random_t<uint32_t>({0u, regions_count - 1u}, seed);
        mismatch_indices.reserve(mismatch_count);

        for (auto i = 0u; i < mismatch_count;)
        {
            const auto mismatch_region = random_mismatch.get_next();

            if (mismatch_indices.end() == std::find(mismatch_indices.begin(),
                                                    mismatch_indices.end(),
                                                    mismatch_region))
            {
                mismatch_indices.push_back(mismatch_region);
                ++i;
            }
        }

        // We have to sort indices to form
        // correct reference stream
        std::sort(mismatch_indices.begin(), mismatch_indices.end());


        // Reference job initialization
        ref_job.destination_first.reserve(mismatch_indices.size() * DELTA_NOTE_BYTE_SIZE);

        // Put correct result
        // and first mismatch index
        ref_job.result = 2u;
        ref_job.offset = mismatch_indices.front();


        // Reference vector initialization
        for (const auto &index : mismatch_indices)
        {
            auto delta_data_field         = std::array<uint8_t, 10u>();
            const auto mismatch_region    = (index * DELTA_DATA_FIELD_SIZE);
            const auto inner_index        = (random_regions.get_next() % DELTA_DATA_FIELD_SIZE);
            const auto mismatch_position  = (mismatch_region + inner_index);
            const auto reference_position = &lib_job->source_second_ptr[mismatch_region];
            const auto region_index       = *reinterpret_cast<const uint16_t *>(&index);
            const auto reference_value    = *reinterpret_cast<const uint64_t *>(reference_position);

            *reinterpret_cast<uint16_t *>(&delta_data_field[0]) = region_index;
            *reinterpret_cast<uint64_t *>(&delta_data_field[2]) = reference_value;

            // Put reference element to the destination vector
            ref_job.destination_first.insert(ref_job.destination_first.end(),
                                             delta_data_field.begin(),
                                             delta_data_field.end());

            // Corrupt element in mask
            lib_job->source_first_ptr[mismatch_position] =
                ~lib_job->source_first_ptr[mismatch_position];
        }

        // Resize vectors for overflow
        lib_job->destination_length = static_cast<uint32_t>((mismatch_indices.size() / 2u) * DELTA_NOTE_BYTE_SIZE);
        ref_job.destination_first.resize((mismatch_indices.size() / 2u) * DELTA_NOTE_BYTE_SIZE);
    }


    // Tests registration
    DML_JOB_API_TEST_REGISTER(dml_delta_create, ta_equal_vectors);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, ta_not_equal_vectors);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, ta_equal_vectors_expected_equal);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, ta_equal_vectors_expected_not_equal);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, ta_equal_vectors_expected_overflow);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, ta_equal_vectors_bad_check_result);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, ta_not_equal_vectors_expected_equal);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, ta_not_equal_vectors_expected_not_equal);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, ta_not_equal_vectors_expected_overflow);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, ta_not_equal_vectors_bad_check_result);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, ta_overflow_expected_equal);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, ta_overflow_expected_not_equal);
    DML_JOB_API_TEST_REGISTER(dml_delta_create, ta_overflow_expected_overflow);

}
