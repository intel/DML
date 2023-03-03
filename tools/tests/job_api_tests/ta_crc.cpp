/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains algorithmic tests for DML_OP_CRC operation
 * @date 5/7/2020
 *
 */
#include "t_common.hpp"
#include "t_random_generator.hpp"
#include "t_random_parameters.hpp"
#include "ref_crc.hpp"
#include "t_utility_functions.hpp"
#include "t_test_case_generator.hpp"


namespace dml
{
    /** Default range for tests */
    constexpr dml::test::range_t<uint32_t> DEFAULT_RANGE = {TEST_MIN_MEMORY_BLOCK_SIZE, TEST_MAX_MEMORY_BLOCK_SIZE};

    /**
     * @brief Tests the operation without any flags
     */
    DML_JOB_API_TEST(crc, no_flags)
    {
        // Create library and reference jobs
        auto lib_job = dml::test::job_t(dml::test::variables_t::path);
        auto ref_job = dml::test::reference_job_t();

        // Check if library job was actually allocated
        ASSERT_TRUE(lib_job);

        // Variables
        const auto seed     = test_system::get_seed();
        const auto crc_seed = 0u;

        auto test_cases = dml::test::test_case_generator<uint32_t>(dml::test::get_default_test_lengths);

        for (auto test_case: test_cases)
        {
            auto random_value   = dml::test::random_t<uint8_t>(seed);
            const auto flags    = 0u;
            std::vector<uint8_t> source(test_case, 0);
            uint32_t             crc_value = 0;
            // Library job initialization
            lib_job->source_first_ptr = source.data();
            lib_job->source_length    = test_case;
            lib_job->crc_checksum_ptr = &crc_value;
            lib_job->operation        = DML_OP_CRC;
            lib_job->flags            = flags;

            // Vectors initialization
            std::generate(source.begin(),
                          source.end(),
                          random_value);

            const auto crc_reference_value = dml::reference::calculate_crc<uint32_t, flags>(lib_job->source_first_ptr,
                                                                                            lib_job->source_first_ptr +
                                                                                            lib_job->source_length,
                                                                                            crc_seed);

            // Reference job initialization
            ref_job.crc_checksum_ptr = std::make_unique<uint32_t>(crc_reference_value);
            ref_job.status           = DML_STATUS_OK;

            auto library_status = lib_job.run();
            auto reference_status = ref_job.status;

            EXPECT_EQ(library_status, reference_status) << test_cases.info(test_case);

            EXPECT_EQ(lib_job, ref_job) << test_cases.info(test_case);
        }
    }

    /**
     * @brief Tests the operation with DML_FLAG_CRC_READ_SEED flag
     */
    DML_JOB_API_TEST(crc, read_seed_flag)
    {
        // Create library and reference jobs
        auto lib_job = dml::test::job_t(dml::test::variables_t::path);
        auto ref_job = dml::test::reference_job_t();

        // Check if library job was actually allocated
        ASSERT_TRUE(lib_job);

        // Variables
        const auto seed     = test_system::get_seed();
        auto random_seed    = dml::test::random_t<uint32_t>(seed);
        const auto crc_seed = random_seed.get_next();
        const auto flags    = DML_FLAG_CRC_READ_SEED;

        auto test_cases = dml::test::test_case_generator<uint32_t>(dml::test::get_default_test_lengths);

        for (auto test_case: test_cases)
        {
            auto random_value   = dml::test::random_t<uint8_t>(seed);
            std::vector<uint8_t> source(test_case, 0);
            uint32_t             crc_value = crc_seed;
            // Library job initialization
            lib_job->source_first_ptr = source.data();
            lib_job->source_length    = test_case;
            lib_job->crc_checksum_ptr = &crc_value;
            lib_job->operation        = DML_OP_CRC;
            lib_job->flags            = flags;

            // Vectors initialization
            std::generate(source.begin(),
                          source.end(),
                          random_value);

            const auto crc_reference_value = dml::reference::calculate_crc<uint32_t, flags>(lib_job->source_first_ptr,
                                                                                            lib_job->source_first_ptr +
                                                                                            lib_job->source_length,
                                                                                            crc_seed);

            // Reference job initialization
            ref_job.crc_checksum_ptr = std::make_unique<uint32_t>(crc_reference_value);
            ref_job.status           = DML_STATUS_OK;

            auto library_status = lib_job.run();
            auto reference_status = ref_job.status;

            EXPECT_EQ(library_status, reference_status) << test_cases.info(test_case);

            EXPECT_EQ(lib_job, ref_job) << test_cases.info(test_case);
        }
    }

    /**
     * @brief Tests the operation with DML_FLAG_CRC_BYPASS_REFLECTION flag
     */
    DML_JOB_API_TEST(crc, bypass_reflection_flag)
    {
        // Create library and reference jobs
        auto lib_job = dml::test::job_t(dml::test::variables_t::path);
        auto ref_job = dml::test::reference_job_t();

        // Check if library job was actually allocated
        ASSERT_TRUE(lib_job);

        // Variables
        const auto seed     = test_system::get_seed();
        auto random_seed    = dml::test::random_t<uint32_t>(seed);
        const auto crc_seed = random_seed.get_next();
        const auto flags    = DML_FLAG_CRC_READ_SEED | DML_FLAG_CRC_BYPASS_REFLECTION;

        auto test_cases = dml::test::test_case_generator<uint32_t>(dml::test::get_default_test_lengths);

        for (auto test_case: test_cases)
        {
            auto random_value   = dml::test::random_t<uint8_t>(seed);
            std::vector<uint8_t> source(test_case, 0);
            uint32_t             crc_value = crc_seed;
            // Library job initialization
            lib_job->source_first_ptr = source.data();
            lib_job->source_length    = test_case;
            lib_job->crc_checksum_ptr = &crc_value;
            lib_job->operation        = DML_OP_CRC;
            lib_job->flags            = flags;

            // Vectors initialization
            std::generate(source.begin(),
                          source.end(),
                          random_value);

            const auto crc_reference_value = dml::reference::calculate_crc<uint32_t, flags>(lib_job->source_first_ptr,
                                                                                            lib_job->source_first_ptr +
                                                                                            lib_job->source_length,
                                                                                            crc_seed);

            // Reference job initialization
            ref_job.crc_checksum_ptr = std::make_unique<uint32_t>(crc_reference_value);
            ref_job.status           = DML_STATUS_OK;

            auto library_status = lib_job.run();
            auto reference_status = ref_job.status;

            EXPECT_EQ(library_status, reference_status) << test_cases.info(test_case);

            EXPECT_EQ(lib_job, ref_job) << test_cases.info(test_case);
        }
    }

    /**
     * @brief Tests the operation with DML_FLAG_CRC_BYPASS_DATA_REFLECTION flag
     */
    DML_JOB_API_TEST(crc, bypass_data_reflection_flag)
    {
        // Create library and reference jobs
        auto lib_job = dml::test::job_t(dml::test::variables_t::path);
        auto ref_job = dml::test::reference_job_t();

        // Check if library job was actually allocated
        ASSERT_TRUE(lib_job);

        // Variables
        const auto seed     = test_system::get_seed();
        auto random_seed    = dml::test::random_t<uint32_t>(seed);
        const auto crc_seed = random_seed.get_next();
        const auto flags    = DML_FLAG_CRC_READ_SEED | DML_FLAG_CRC_BYPASS_DATA_REFLECTION;

        auto test_cases = dml::test::test_case_generator<uint32_t>(dml::test::get_default_test_lengths);

        for (auto test_case: test_cases)
        {
            auto random_value   = dml::test::random_t<uint8_t>(seed);
            std::vector<uint8_t> source(test_case, 0);
            uint32_t             crc_value = crc_seed;
            // Library job initialization
            lib_job->source_first_ptr = source.data();
            lib_job->source_length    = test_case;
            lib_job->crc_checksum_ptr = &crc_value;
            lib_job->operation        = DML_OP_CRC;
            lib_job->flags            = flags;

            // Vectors initialization
            std::generate(source.begin(),
                          source.end(),
                          random_value);

            const auto crc_reference_value = dml::reference::calculate_crc<uint32_t, flags>(lib_job->source_first_ptr,
                                                                                            lib_job->source_first_ptr +
                                                                                            lib_job->source_length,
                                                                                            crc_seed);

            // Reference job initialization
            ref_job.crc_checksum_ptr = std::make_unique<uint32_t>(crc_reference_value);
            ref_job.status           = DML_STATUS_OK;

            auto library_status = lib_job.run();
            auto reference_status = ref_job.status;

            EXPECT_EQ(library_status, reference_status) << test_cases.info(test_case);

            EXPECT_EQ(lib_job, ref_job) << test_cases.info(test_case);
        }
    }

}
