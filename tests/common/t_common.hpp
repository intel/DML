/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @file
 * @brief
 * @date 2/7/2020
 */

#if defined(_WIN32)
#undef _HAS_STD_BYTE

//Fix for std::byte
#define _HAS_STD_BYTE 0

#endif  // _HAS_STD_BYTE

#ifndef DML_T_COMMON_HPP__
#define DML_T_COMMON_HPP__

// Fix for std::tuple and some implicit type casts
#pragma warning(disable : 4244 4100 4127 4670 4673)

#include <algorithm>
#include <array>

#include "gtest/gtest.h"
#include "iostream"
#include "t_defines.hpp"
#include "t_job.hpp"
#include "t_memory_pool.hpp"
#include "t_reference_job.hpp"

namespace dml::test
{

    /**
     * @brief Struct that holds global variables for testing
     */
    struct variables_t
    {
        // This structure prevents zeroing
        // variables when they are passed
        // to log

        /**
         * @brief Seed for testing
         */
        static inline auto seed = uint32_t(0u);

        /**
         * @brief Execution path
         */
        static inline auto path = DML_PATH_SW;

        /**
         * @brief Test case id, default value -1 means this parameter was not specified
         *        and all tests cases should be executed
         */
        static inline int32_t test_case_id = -1;
    };

    static inline uint16_t calculate_crc_16u(uint16_t crc_value, const uint8_t data, const uint16_t polynomial)
    {
        const size_t   byte_width     = 8;
        const size_t   crc_bit_count  = sizeof(crc_value) * byte_width;
        const size_t   crc_byte_shift = crc_bit_count - byte_width;
        const uint16_t high_bit_mask  = 1 << (crc_bit_count - 1);

        crc_value ^= (data << crc_byte_shift);

        for (size_t bit = 0u; bit < byte_width; ++bit)
        {
            crc_value = (crc_value & high_bit_mask) ? ((crc_value << 1) ^ polynomial) : (crc_value << 1);
        }

        return crc_value;
    }

    static inline uint16_t crc_16u(const uint8_t *src, const uint16_t transfer_size, uint16_t crc_value, const uint16_t polynomial)
    {
        for (size_t byte = 0; byte < transfer_size; ++byte)
        {
            crc_value = calculate_crc_16u(crc_value, src[byte], polynomial);
        }

        return crc_value;
    }
}  // namespace dml::test

/** Console indent for logging */
#define OWN_TEST_LOG_INDENT ("[          ] ")

/** Console loging macro */
#define DML_TEST_LOG(...) std::cout << OWN_TEST_LOG_INDENT << __VA_ARGS__ << std::endl;

/**
 * @brief Common core test register
 */
#define CORE_TEST_REGISTER(function_name, test_name) \
    TEST(function_name, test_name)                   \
    {                                                \
        test_name();                                 \
    }

/**
 * @brief Job API test register
 */
#define DML_JOB_API_TEST(operation, test) TEST(ta##_c_api_##operation, test)

/**
 * @brief Wrapper for job filler function
 */
#define DML_JOB_API_TEST_GENERATOR(function_name, generator_name) \
    auto function_name##_##generator_name##_job_filler(dml::test::job_t &lib_job, dml::test::reference_job_t &ref_job)->void

/**
 * @brief Common job-api test register
 */
#define DML_JOB_API_TEST_REGISTER(function_name, generator_name)                                                  \
    using function_name = ::testing::Test;                                                                        \
    TEST_F(function_name, generator_name)                                                                         \
    {                                                                                                             \
        try                                                                                                       \
        {                                                                                                         \
            auto lib_job = dml::test::job_t(dml::test::variables_t::path);                                        \
            auto ref_job = dml::test::reference_job_t();                                                          \
                                                                                                                  \
            if (lib_job)                                                                                          \
            {                                                                                                     \
                function_name##_##generator_name##_job_filler(lib_job, ref_job);                                  \
                                                                                                                  \
                auto library_status   = lib_job.run();                                                            \
                auto reference_status = ref_job.status;                                                           \
                                                                                                                  \
                EXPECT_EQ(library_status, reference_status);                                                      \
                                                                                                                  \
                EXPECT_TRUE(lib_job == ref_job);                                                                  \
                if (lib_job->operation == DML_OP_BATCH)                                                           \
                {                                                                                                 \
                    auto ptr  = reinterpret_cast<dml::test::reference_job_t *>(ref_job.destination_first.data()); \
                    auto size = ref_job.destination_first.size() / sizeof(dml::test::reference_job_t);            \
                    std::destroy_n(ptr, size);                                                                    \
                }                                                                                                 \
                dml::test::global_allocator::deallocate_all();                                                    \
            }                                                                                                     \
        }                                                                                                         \
        catch (std::runtime_error & e)                                                                            \
        {                                                                                                         \
            std::cout << e.what() << '\n';                                                                        \
            exit(1);                                                                                              \
        }                                                                                                         \
    }

/**
 * @brief Common unit test generator
 */
#define DML_UNIT_TEST_GENERATOR(function_name, generator_name) auto function_name##_##generator_name##_unit_test()->void

/**
 * @brief Common unit test register
 */
#define DML_UNIT_TEST_REGISTER(function_name, generator_name) \
    using function_name = ::testing::Test;                    \
    TEST_F(function_name, generator_name)                     \
    {                                                         \
        function_name##_##generator_name##_unit_test();       \
        dml::test::global_allocator::deallocate_all();        \
    }

#endif  //DML_T_COMMON_HPP__
