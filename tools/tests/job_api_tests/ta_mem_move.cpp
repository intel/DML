/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains algorithmic tests for DML_OP_MEM_MOVE operation
 * @date 4/17/2020
 *
 */
#include "t_common.hpp"
#include "t_random_generator.hpp"
#include "t_random_parameters.hpp"
#include "t_utility_functions.hpp"
#include "t_test_case_generator.hpp"


namespace dml {

    /** Default range for tests */
    constexpr dml::test::range_t<uint32_t> DEFAULT_RANGE = {TEST_MIN_MEMORY_BLOCK_SIZE, TEST_MAX_MEMORY_BLOCK_SIZE};

    struct mem_move_test_case {
        uint32_t source_length = 1;
        uint32_t source_alignment = 0;
        uint32_t destination_alignment = 0;

        bool operator==(const mem_move_test_case &other) {
            return this->source_length == other.source_length &&
                   this->source_alignment == other.source_alignment &&
                   this->destination_alignment == other.destination_alignment;
        }
    };

    auto& operator<<(std::ostream &os, const mem_move_test_case &test_case) {
        os << "source length = " << test_case.source_length <<
           "\nsource alignment = " << test_case.source_alignment <<
           "\n destination alignment = " << test_case.destination_alignment << "\n";

        return os;
    }

    DML_JOB_API_TEST(mem_move, no_overlapping_unaligned_destination) {
        // Create library and reference jobs
        auto lib_job = dml::test::job_t(dml::test::variables_t::path);
        auto ref_job = dml::test::reference_job_t();

        // Check if library job was actually allocated
        ASSERT_TRUE(lib_job);

        auto mem_move_test_generator = []() -> auto {
            auto test_lengths = dml::test::get_default_test_lengths();
            std::vector<mem_move_test_case> result;

            for (auto length: test_lengths) {
                mem_move_test_case current_test_case;
                current_test_case.source_alignment = 0;
                current_test_case.source_length = length;
                for (uint32_t i = 1; i <= 64; i++) {
                    current_test_case.destination_alignment = i;
                    result.push_back(current_test_case);
                }
            }

            return result;
        };

        const auto seed = test_system::get_seed();
        auto random_value = dml::test::random_t<uint8_t>(seed);

        auto test_cases = dml::test::test_case_generator<mem_move_test_case>(mem_move_test_generator);

        for (auto &test_case : test_cases) {
            std::vector<uint8_t> source(test_case.source_length + 128, 0);
            std::vector<uint8_t> destination(test_case.source_length + 128, 0);

            auto *src_ptr = source.data();
            auto *aligned_src_ptr = (uint8_t *) (((uint64_t) src_ptr & ~63llu) + 64u +
                                                 (uint64_t) test_case.source_alignment);

            lib_job->source_first_ptr = aligned_src_ptr;
            lib_job->source_length = test_case.source_length;

            auto *dst_ptr = destination.data();
            auto *aligned_dst_ptr = (uint8_t *) (((uint64_t) dst_ptr & ~63llu) + 64u +
                                                 (uint64_t) test_case.destination_alignment);

            lib_job->destination_first_ptr = aligned_dst_ptr;
            lib_job->destination_length = test_case.source_length;

            lib_job->flags = DML_FLAG_COPY_ONLY;
            lib_job->operation = DML_OP_MEM_MOVE;

            // Vectors initialization
            std::generate(lib_job->source_first_ptr,
                          lib_job->source_first_ptr + lib_job->source_length,
                          random_value);

            // Reference job initialization
            ref_job.destination_first = std::vector<uint8_t>(lib_job->source_first_ptr,
                                                             lib_job->source_first_ptr + lib_job->source_length);
            ref_job.status = DML_STATUS_OK;

            auto library_status = lib_job.run();
            auto reference_status = DML_STATUS_OK;


            EXPECT_EQ(library_status, reference_status) << test_cases.info(test_case);
            EXPECT_EQ(lib_job, ref_job) << test_cases.info(test_case);
        }


    }

    DML_JOB_API_TEST(mem_move, no_overlapping_unaligned_source) {
        // Create library and reference jobs
        auto lib_job = dml::test::job_t(dml::test::variables_t::path);
        auto ref_job = dml::test::reference_job_t();

        // Check if library job was actually allocated
        ASSERT_TRUE(lib_job);

        auto mem_move_test_generator = []() -> auto {
            auto test_lengths = dml::test::get_default_test_lengths();
            std::vector<mem_move_test_case> result;

            for (auto length: test_lengths) {
                mem_move_test_case current_test_case;
                current_test_case.destination_alignment = 0;
                current_test_case.source_length = length;
                for (uint32_t i = 1; i <= 64; i++) {
                    current_test_case.source_alignment = i;
                    result.push_back(current_test_case);
                }
            }

            return result;
        };

        const auto seed = test_system::get_seed();
        auto random_value = dml::test::random_t<uint8_t>(seed);

        auto test_cases = dml::test::test_case_generator<mem_move_test_case>(mem_move_test_generator);

        for (auto &test_case : test_cases) {
            std::vector<uint8_t> source(test_case.source_length + 128, 0);
            std::vector<uint8_t> destination(test_case.source_length + 128, 0);

            auto *src_ptr = source.data();
            auto *aligned_src_ptr = (uint8_t *) (((uint64_t) src_ptr & ~63llu) + 64u +
                                                 (uint64_t) test_case.source_alignment);

            lib_job->source_first_ptr = aligned_src_ptr;
            lib_job->source_length = test_case.source_length;

            auto *dst_ptr = destination.data();
            auto *aligned_dst_ptr = (uint8_t *) (((uint64_t) dst_ptr & ~63llu) + 64u +
                                                 (uint64_t) test_case.destination_alignment);

            lib_job->destination_first_ptr = aligned_dst_ptr;
            lib_job->destination_length = test_case.source_length;

            lib_job->flags = DML_FLAG_COPY_ONLY;
            lib_job->operation = DML_OP_MEM_MOVE;

            // Vectors initialization
            std::generate(lib_job->source_first_ptr,
                          lib_job->source_first_ptr + lib_job->source_length,
                          random_value);

            // Reference job initialization
            ref_job.destination_first = std::vector<uint8_t>(lib_job->source_first_ptr,
                                                             lib_job->source_first_ptr + lib_job->source_length);
            ref_job.status = DML_STATUS_OK;

            auto library_status = lib_job.run();
            auto reference_status = DML_STATUS_OK;


            EXPECT_EQ(library_status, reference_status) << test_cases.info(test_case);
            EXPECT_EQ(lib_job, ref_job) << test_cases.info(test_case);
        }

    }

    DML_JOB_API_TEST(mem_move, no_overlapping_unaligned_source_and_destination) {
        // Create library and reference jobs
        auto lib_job = dml::test::job_t(dml::test::variables_t::path);
        auto ref_job = dml::test::reference_job_t();

        // Check if library job was actually allocated
        ASSERT_TRUE(lib_job);

        auto mem_move_test_generator = []() -> auto {
            auto test_lengths = dml::test::get_default_test_lengths();
            std::vector<mem_move_test_case> result;

            for (auto length: test_lengths) {
                mem_move_test_case current_test_case;
                current_test_case.source_length = length;
                for (uint32_t i = 1; i <= 64; i++) {
                    for (uint32_t j = 1; j <= 64; j++) {
                        current_test_case.destination_alignment = i;
                        current_test_case.source_alignment = j;
                        result.push_back(current_test_case);
                    }
                }
            }

            return result;
        };

        const auto seed = test_system::get_seed();
        auto random_value = dml::test::random_t<uint8_t>(seed);

        auto test_cases = dml::test::test_case_generator<mem_move_test_case>(mem_move_test_generator);

        for (auto &test_case : test_cases) {
            std::vector<uint8_t> source(test_case.source_length + 128, 0);
            std::vector<uint8_t> destination(test_case.source_length + 128, 0);

            auto *src_ptr = source.data();
            auto *aligned_src_ptr = (uint8_t *) (((uint64_t) src_ptr & ~63llu) + 64u +
                                                 (uint64_t) test_case.source_alignment);

            lib_job->source_first_ptr = aligned_src_ptr;
            lib_job->source_length = test_case.source_length;

            auto *dst_ptr = destination.data();
            auto *aligned_dst_ptr = (uint8_t *) (((uint64_t) dst_ptr & ~63llu) + 64u +
                                                 (uint64_t) test_case.destination_alignment);

            lib_job->destination_first_ptr = aligned_dst_ptr;
            lib_job->destination_length = test_case.source_length;

            lib_job->flags = DML_FLAG_COPY_ONLY;
            lib_job->operation = DML_OP_MEM_MOVE;

            // Vectors initialization
            std::generate(lib_job->source_first_ptr,
                          lib_job->source_first_ptr + lib_job->source_length,
                          random_value);

            // Reference job initialization
            ref_job.destination_first = std::vector<uint8_t>(lib_job->source_first_ptr,
                                                             lib_job->source_first_ptr + lib_job->source_length);
            ref_job.status = DML_STATUS_OK;

            auto library_status = lib_job.run();
            auto reference_status = DML_STATUS_OK;


            EXPECT_EQ(library_status, reference_status) << test_cases.info(test_case);
            EXPECT_EQ(lib_job, ref_job) << test_cases.info(test_case);
        }
    }

    DML_JOB_API_TEST(mem_move, overlapping_unaligned_destination) {
        // Create library and reference jobs
        auto lib_job = dml::test::job_t(dml::test::variables_t::path);
        auto ref_job = dml::test::reference_job_t();

        // Check if library job was actually allocated
        ASSERT_TRUE(lib_job);

        auto mem_move_test_generator = []() -> auto {
            auto test_lengths = dml::test::get_default_test_lengths();
            std::vector<mem_move_test_case> result;

            for (auto length: test_lengths) {
                mem_move_test_case current_test_case;
                current_test_case.source_alignment = 0;
                current_test_case.source_length = length;
                for (uint32_t i = 1; i <= 64; i++) {
                    current_test_case.destination_alignment = i;
                    result.push_back(current_test_case);
                }
            }

            return result;
        };

        const auto seed = test_system::get_seed();
        auto random_value = dml::test::random_t<uint8_t>(seed);

        auto test_cases = dml::test::test_case_generator<mem_move_test_case>(mem_move_test_generator);

        // Generate offset for destination buffer
        auto destination_offset = random_value.get_next();

        for (auto &test_case : test_cases) {
            std::vector<uint8_t> common_buffer(test_case.source_length + 128 + destination_offset, 0);

            auto *src_ptr = common_buffer.data();
            auto *aligned_src_ptr = (uint8_t *) (((uint64_t) src_ptr & ~63llu) + 64u +
                                                 (uint64_t) test_case.source_alignment);

            lib_job->source_first_ptr = aligned_src_ptr;
            lib_job->source_length = test_case.source_length;

            lib_job->destination_first_ptr = lib_job->source_first_ptr + destination_offset;
            lib_job->destination_length = test_case.source_length;

            lib_job->flags = 0;
            lib_job->operation = DML_OP_MEM_MOVE;

            // Vectors initialization
            std::generate(lib_job->source_first_ptr,
                          lib_job->source_first_ptr + lib_job->source_length,
                          random_value);

            // Reference job initialization
            ref_job.destination_first = std::vector<uint8_t>(lib_job->source_first_ptr,
                                                             lib_job->source_first_ptr + lib_job->source_length);
            ref_job.status = DML_STATUS_OK;

            auto library_status = lib_job.run();
            auto reference_status = DML_STATUS_OK;


            EXPECT_EQ(library_status, reference_status) << test_cases.info(test_case);
            EXPECT_EQ(lib_job, ref_job) << test_cases.info(test_case);
        }
    }

}