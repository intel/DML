/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains algorithmic tests for DML_OP_DIF_CHECK operation
 * @date 5/21/2020
 *
 */
#include "t_common.hpp"
#include "t_random_generator.hpp"
#include "t_random_parameters.hpp"

#include "ref_value_reverse.hpp"

#include <optimization_dispatcher.hpp>

namespace dml
{

    /** Default range for tests */
    constexpr dml::test::range_t<uint32_t> DEFAULT_RANGE = {TEST_MIN_MEMORY_BLOCK_SIZE, TEST_MAX_MEMORY_BLOCK_SIZE};

    /** Default POLYNOMIAL for crc operation */
    constexpr auto DEFAULT_POLYNOMIAL  = 0x8BB7u;

    /** Block size is 512 bytes */
    constexpr auto DIF_BLOCK_SIZE_512  = 512u;

    /** Block size is 520 bytes */
    constexpr auto DIF_BLOCK_SIZE_520  = 520u;

    /** Block size is 4096 bytes */
    constexpr auto DIF_BLOCK_SIZE_4096 = 4096u;

    /** Block size is 4104 bytes */
    constexpr auto DIF_BLOCK_SIZE_4104 = 4104u;

    /** Enables All F Detection */
    constexpr auto DIF_F_ALL_FLAG = DML_DIF_FLAG_SRC_F_DETECT_ALL | DML_DIF_FLAG_SRC_F_ENABLE_ERROR;


    /** Custom initializer */
    template<dml_dif_block_size_t block_size_index, uint32_t block_size, uint32_t flags, uint32_t dif_error_expected>
    static inline auto own_create_jobs(dml::test::job_t &lib_job, dml::test::reference_job_t &ref_job) -> void
    {
        // Variables
        const auto seed               = test_system::get_seed();
        auto random_length            = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_value             = dml::test::random_t<uint8_t>(seed);
        auto random_tag_seed          = dml::test::random_t<uint16_t>(seed);
        auto random_ref_tag_seed      = dml::test::random_t<uint32_t>(seed);
        const auto block_count        = random_length.get_next();
        const auto source_length      = ((block_size + 8u) * block_count);
        const auto destination_length = (block_size * block_count);


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(source_length);
        lib_job->source_length         = source_length;
        lib_job->operation             = DML_OP_DIF_CHECK;

        // DIF config initialization
        lib_job->dif_config.source_application_tag_seed = random_tag_seed.get_next();
        lib_job->dif_config.source_application_tag_mask = random_tag_seed.get_next();
        lib_job->dif_config.source_reference_tag_seed   = random_ref_tag_seed.get_next();
        lib_job->dif_config.block_size                  = static_cast<dml_dif_block_size_t>(block_size_index);
        lib_job->dif_config.flags                       = flags;

        if (0u == lib_job->dif_config.source_application_tag_mask)
        {
            lib_job->dif_config.source_application_tag_mask = ~lib_job->dif_config.source_application_tag_mask;
        }

        // DIF variables
        const auto crc_seed             = static_cast<uint16_t>((DML_DIF_FLAG_INVERT_CRC_SEED & flags) ? 0xFFFFu : 0u);
        const auto invert_flag          = (flags & DML_DIF_FLAG_INVERT_CRC_RESULT) ? 1u : 0u;
        const auto ref_tag_update_value = static_cast<uint8_t>((flags & DML_DIF_FLAG_SRC_FIX_REF_TAG) ? 0u : 1u);
        const auto app_tag_update_value = static_cast<uint8_t>((flags & DML_DIF_FLAG_SRC_INC_APP_TAG) ? 1u : 0u);
        const auto application_tag_mask = static_cast<uint16_t>(~(lib_job->dif_config.source_application_tag_mask));


        // Buffer initialization
        auto &memory_buffer = dml::test::global_allocator::allocate(destination_length);


        // Reference vectors initialization
        std::generate(memory_buffer.begin(), memory_buffer.end(), random_value);

        auto source_current_ptr        = &memory_buffer[0];
        auto destination_current_ptr   = &lib_job->source_first_ptr[0];
        const auto destination_end_ptr = &lib_job->source_first_ptr[lib_job->source_length - 1u];

        auto application_tag = static_cast<uint16_t>(lib_job->dif_config.source_application_tag_seed);
        auto reference_tag   = static_cast<uint32_t>(lib_job->dif_config.source_reference_tag_seed);


        while (destination_current_ptr < destination_end_ptr)
        {
            dml::core::dispatch::mem_move(source_current_ptr, destination_current_ptr, block_size);

            auto current_crc = test::crc_16u(destination_current_ptr, block_size, crc_seed, DEFAULT_POLYNOMIAL);

            auto dif_ptr = (destination_current_ptr + block_size);

            *reinterpret_cast<uint16_t *>(&dif_ptr[0]) = dml::reference::reverse_bytes<uint16_t>((invert_flag) ?
                                                                                                     ~current_crc :
                                                                                                     current_crc);
            *reinterpret_cast<uint16_t *>(&dif_ptr[2]) = dml::reference::reverse_bytes<uint16_t>(application_tag & application_tag_mask);
            *reinterpret_cast<uint32_t *>(&dif_ptr[4]) = dml::reference::reverse_bytes<uint32_t>(reference_tag);

            application_tag         += app_tag_update_value;
            reference_tag           += ref_tag_update_value;
            source_current_ptr      += block_size;
            destination_current_ptr += (block_size + 8u);
        }


        // Apply corruption
        if constexpr (0u != dif_error_expected
                      || (DML_DIF_FLAG_SRC_F_DETECT_TAGS & flags)
                      || (DML_DIF_FLAG_SRC_F_DETECT_APP_TAG & flags))
        {
            const auto random_block    = (random_length.get_next() % block_count);
            const auto actual_block    = ((8u + block_size) * random_block);
            const auto actual_position = (actual_block + block_size);

            ref_job.result |= dif_error_expected;
            ref_job.offset  = dif_error_expected ? actual_block : 0u;
            ref_job.status  = dif_error_expected ? DML_STATUS_DIF_CHECK_ERROR : DML_STATUS_OK;

            if constexpr (static_cast<bool>(DIF_F_ALL_FLAG == (DIF_F_ALL_FLAG & flags)))
            {
                *reinterpret_cast<uint64_t *>(&lib_job->source_first_ptr[actual_position]) = 0xFFFFFFFFFFFFFFFFull;
            }

            if constexpr (static_cast<bool>(DML_DIF_CHECK_GUARD_MISMATCH & dif_error_expected))
            {
                *reinterpret_cast<uint16_t *>(&lib_job->source_first_ptr[actual_position]) =
                    ~(*reinterpret_cast<uint16_t *>(&lib_job->source_first_ptr[actual_position]));
            }

            if constexpr (static_cast<bool>(DML_DIF_CHECK_APPLICATION_TAG_MISMATCH & dif_error_expected))
            {
                *reinterpret_cast<uint16_t *>(&lib_job->source_first_ptr[actual_position + 2u]) =
                    ~(*reinterpret_cast<uint16_t *>(&lib_job->source_first_ptr[actual_position + 2u]));
            }

            if constexpr (static_cast<bool>(DML_DIF_CHECK_REFERENCE_TAG_MISMATCH & dif_error_expected))
            {
                *reinterpret_cast<uint32_t *>(&lib_job->source_first_ptr[actual_position + 4u]) =
                    ~(*reinterpret_cast<uint32_t *>(&lib_job->source_first_ptr[actual_position + 4u]));
            }

            if constexpr (static_cast<bool>(DML_DIF_FLAG_SRC_F_DETECT_APP_TAG & flags))
            {
                *reinterpret_cast<uint16_t *>(&lib_job->source_first_ptr[actual_position + 2u]) = 0xFFFFu;
            }

            if constexpr (static_cast<bool>(DML_DIF_FLAG_SRC_F_DETECT_TAGS & flags))
            {
                *reinterpret_cast<uint16_t *>(&lib_job->source_first_ptr[actual_position + 2u]) = 0xFFFFu;
                *reinterpret_cast<uint32_t *>(&lib_job->source_first_ptr[actual_position + 4u]) = 0xFFFFFFFFu;
            }

            if constexpr (static_cast<bool>((DML_DIF_FLAG_SRC_F_DETECT_TAGS
                                             | DML_DIF_FLAG_SRC_F_DETECT_APP_TAG)
                                             & flags))
            {
                ref_job.status = DML_STATUS_OK;
                ref_job.result = 0u;
                ref_job.offset = 0u;
            }
        }
    }


/** Custom test register **/
#define OWN_SUBMIT_DIF_CHECK_TEST(name, flags, dif_error_expected) \
        DML_JOB_API_TEST_GENERATOR(dml_dif_check, name ##_ ##512) \
        { \
            own_create_jobs<DML_DIF_BLOCK_SIZE_512, DIF_BLOCK_SIZE_512, flags, dif_error_expected>(lib_job, ref_job); \
        } \
        DML_JOB_API_TEST_GENERATOR(dml_dif_check, name ##_ ##520) \
        { \
            own_create_jobs<DML_DIF_BLOCK_SIZE_520, DIF_BLOCK_SIZE_520, flags, dif_error_expected>(lib_job, ref_job); \
        } \
        DML_JOB_API_TEST_GENERATOR(dml_dif_check, name ##_ ##4096) \
        { \
            own_create_jobs<DML_DIF_BLOCK_SIZE_4096, DIF_BLOCK_SIZE_4096, flags, dif_error_expected>(lib_job, ref_job); \
        } \
        DML_JOB_API_TEST_GENERATOR(dml_dif_check, name ##_ ##4104) \
        { \
            own_create_jobs<DML_DIF_BLOCK_SIZE_4104, DIF_BLOCK_SIZE_4104, flags, dif_error_expected>(lib_job, ref_job); \
        } \
        DML_JOB_API_TEST_REGISTER(dml_dif_check, name ##_ ##512);  \
        DML_JOB_API_TEST_REGISTER(dml_dif_check, name ##_ ##520);  \
        DML_JOB_API_TEST_REGISTER(dml_dif_check, name ##_ ##4096); \
        DML_JOB_API_TEST_REGISTER(dml_dif_check, name ##_ ##4104);


    /**
     * @brief Tests the operation without flags without curruption
     */
    OWN_SUBMIT_DIF_CHECK_TEST(ta_no_flags_no_corruption, 0u, 0u);

    /**
     * @brief Tests the operation with invert crc seed flag without curruption
     */
    OWN_SUBMIT_DIF_CHECK_TEST(ta_invert_crc_seed_flag_no_corruption, DML_DIF_FLAG_INVERT_CRC_SEED, 0u);

    /**
     * @brief Tests the operation with invert crc result flag without curruption
     */
    OWN_SUBMIT_DIF_CHECK_TEST(ta_invert_crc_result_flag_no_corruption, DML_DIF_FLAG_INVERT_CRC_RESULT, 0u);

    /**
     * @brief Tests the operation with fix reference tag flag without curruption
     */
    OWN_SUBMIT_DIF_CHECK_TEST(ta_fix_ref_tag_flag_no_corruption, DML_DIF_FLAG_SRC_FIX_REF_TAG, 0u);

    /**
     * @brief Tests the operation with increment application tag flag without curruption
     */
    OWN_SUBMIT_DIF_CHECK_TEST(ta_inc_app_tag_flag_no_corruption, DML_DIF_FLAG_SRC_INC_APP_TAG, 0u);

    /**
     * @brief Tests the operation without flags with corrupted crc
     */
    OWN_SUBMIT_DIF_CHECK_TEST(ta_no_flags_crc_corrupted, 0u, DML_DIF_CHECK_GUARD_MISMATCH);

    /**
     * @brief Tests the operation without flags with corrupted application tag
     */
    OWN_SUBMIT_DIF_CHECK_TEST(ta_no_flags_app_tag_corrupted, 0u, DML_DIF_CHECK_APPLICATION_TAG_MISMATCH);

    /**
     * @brief Tests the operation without flags with corrupted reference tag
     */
    OWN_SUBMIT_DIF_CHECK_TEST(ta_no_flags_ref_tag_corrupted, 0u, DML_DIF_CHECK_REFERENCE_TAG_MISMATCH);

    /**
     * @brief Tests the operation if all bytes in the DIF fields are set to 0xFF
     */
    OWN_SUBMIT_DIF_CHECK_TEST(ta_f_all_detection,
                              DML_DIF_FLAG_SRC_F_ENABLE_ERROR | DML_DIF_FLAG_SRC_F_DETECT_ALL,
                              DML_DIF_CHECK_ALL_BITS_SET_DETECT_ERROR);

    /**
     * @brief Tests the operation if all bytes in the DIF tags are set to 0xFF
     */
    OWN_SUBMIT_DIF_CHECK_TEST(ta_f_app_tag_detection,
                              DML_DIF_FLAG_SRC_F_DETECT_APP_TAG,
                              0u);

    /**
     * @brief Tests the operation if all bytes in the DIF Application tag are set to 0xFF
     */
    OWN_SUBMIT_DIF_CHECK_TEST(ta_f_tags_detection,
                              DML_DIF_FLAG_SRC_F_DETECT_TAGS,
                              0u);

    /**
     * @brief Tests the operation if all bytes in the DIF tags are set to 0xFF
     *
     * @note Guard field is corrupted
     *
     */
    OWN_SUBMIT_DIF_CHECK_TEST(ta_f_tags_detection_with_corrupted_guard,
                              DML_DIF_FLAG_SRC_F_DETECT_TAGS,
                              DML_DIF_CHECK_GUARD_MISMATCH);

    /**
     * @brief Tests the operation if all bytes in the DIF Application tag are set to 0xFF
     *
     * @note Guard and Reference fields are corrupted
     *
     */
    OWN_SUBMIT_DIF_CHECK_TEST(ta_f_app_tag_detection_with_corrupted_guard_and_ref,
                              DML_DIF_FLAG_SRC_F_DETECT_APP_TAG,
                              DML_DIF_CHECK_GUARD_MISMATCH | DML_DIF_CHECK_REFERENCE_TAG_MISMATCH);

}
