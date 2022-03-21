/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains algorithmic tests for DML_OP_DELTA_APPLY operation
 * @date 4/17/2020
 *
 */
#include "dml/dml.h"
#include "t_common.hpp"
#include "t_random_generator.hpp"
#include "t_delta_record_feature_defines.hpp"


namespace dml
{

    /**
     * @brief Tests the operation with random parameters
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_apply, ta_random_parameters)
    {
        // Variables
        const auto seed           = test_system::get_seed();
        auto random_regions       = dml::test::random_t<uint32_t>(REGIONS_COUNT, seed);
        auto random_value         = dml::test::random_t<uint8_t>(seed);
        const auto regions_count  = random_regions.get_next();
        const auto length         = (regions_count * DELTA_DATA_FIELD_SIZE);
        const auto max_size       = (regions_count * DELTA_NOTE_BYTE_SIZE);
        const auto mismatch_count = 1u + (random_regions.get_next() % regions_count);
        auto mismatch_indices     = std::vector<uint32_t>();
        auto &mismatch_buffer     = dml::test::global_allocator::allocate();


        // Library job initialization
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length    = length;
        lib_job->operation             = DML_OP_DELTA_APPLY;


        // Vectors initialization
        std::generate(lib_job->destination_first_ptr,
                      lib_job->destination_first_ptr + lib_job->destination_length,
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
        ref_job.destination_first = dml::test::global_allocator::allocate(
                lib_job->destination_first_ptr,
                lib_job->destination_first_ptr +
                lib_job->destination_length);


        // Add mismatches
        for (const auto &index : mismatch_indices)
        {
            auto delta_data_field         = std::array<uint8_t, 10u>();
            const auto mismatch_region    = (index * DELTA_DATA_FIELD_SIZE);
            const auto inner_index        = (random_regions.get_next() % DELTA_DATA_FIELD_SIZE);
            const auto mismatch_position  = (mismatch_region + inner_index);
            const auto reference_position = &lib_job->destination_first_ptr[mismatch_region];
            const auto region_index       = *reinterpret_cast<const uint16_t *>(&index);
            const auto reference_value    = *reinterpret_cast<const uint64_t *>(reference_position);

            *reinterpret_cast<uint16_t *>(&delta_data_field[0]) = region_index;
            *reinterpret_cast<uint64_t *>(&delta_data_field[2]) = reference_value;

            // Put reference element to the destination vector
            mismatch_buffer.insert(mismatch_buffer.end(),
                                   delta_data_field.begin(),
                                   delta_data_field.end());

            // Corrupt element in mask
            lib_job->destination_first_ptr[mismatch_position] =
                    ~lib_job->destination_first_ptr[mismatch_position];
        }


        // Set library job buffers
        lib_job->source_first_ptr = mismatch_buffer.data();
        lib_job->source_length    = static_cast<uint32_t>(mismatch_buffer.size());
    }


    /**
     * @brief Tests the operation with random parameters
     *        after DML_DELTA_CREATE operation
     */
    DML_JOB_API_TEST_GENERATOR(dml_delta_apply, ta_random_parameters_after_create)
    {
        // Variables
        const auto seed           = test_system::get_seed();
        auto random_regions       = dml::test::random_t<uint32_t>(REGIONS_COUNT, seed);
        auto random_value         = dml::test::random_t<uint8_t>(seed);
        const auto regions_count  = random_regions.get_next();
        const auto length         = (regions_count * DELTA_DATA_FIELD_SIZE);
        const auto max_size       = (regions_count * DELTA_NOTE_BYTE_SIZE);
        const auto mismatch_count = 1u + (random_regions.get_next() % regions_count);
        auto mismatch_indices     = std::vector<uint32_t>();

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
        mismatch_indices.reserve(mismatch_count);
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
        ref_job.destination_first = dml::test::global_allocator::allocate(lib_job->source_first_ptr,
                                                                          lib_job->source_first_ptr +
                                                                          lib_job->source_length);
        // Put correct result
        // and first mismatch index
        ref_job.result = 1u;
        ref_job.offset = mismatch_indices.front();


        // Corrupt mask
        for (const auto &index : mismatch_indices)
        {
            auto delta_data_field         = std::array<uint8_t, 10u>();
            const auto mismatch_region    = (index * DELTA_DATA_FIELD_SIZE);
            const auto inner_index        = (random_regions.get_next() % DELTA_DATA_FIELD_SIZE);
            const auto mismatch_position  = (mismatch_region + inner_index);

            // Corrupt element in mask
            lib_job->source_first_ptr[mismatch_position] =
                    ~lib_job->source_first_ptr[mismatch_position];
        }


        // Intermediate calculations
        const auto create_delta_status = dml_execute_job(&(*lib_job));
        EXPECT_EQ(DML_STATUS_OK, create_delta_status);


        // Prepare for applying delta
        std::swap(lib_job->destination_first_ptr, lib_job->source_first_ptr);
        std::swap(lib_job->destination_length,    lib_job->source_length);
        lib_job->source_second_ptr = nullptr;
        lib_job->operation         = DML_OP_DELTA_APPLY;
    }


    // Tests registration
    DML_JOB_API_TEST_REGISTER(dml_delta_apply, ta_random_parameters);
    DML_JOB_API_TEST_REGISTER(dml_delta_apply, ta_random_parameters_after_create);

}
