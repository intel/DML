/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains algorithmic tests for DML_OP_DIF_UPDATE operation
 * @date 5/14/2020
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


    /** Custom initializer */
    template<dml_dif_block_size_t block_size_index, uint32_t block_size, uint32_t flags>
    static inline auto own_create_jobs(dml::test::job_t &lib_job, dml::test::reference_job_t &ref_job) -> void
    {
        // Variables
        const auto seed          = test_system::get_seed();
        auto random_length       = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_value        = dml::test::random_t<uint8_t>(seed);
        auto random_tag_seed     = dml::test::random_t<uint16_t>(seed);
        auto random_ref_tag_seed = dml::test::random_t<uint32_t>(seed);
        const auto block_count   = random_length.get_next();
        const auto step          = (block_size + 8u);
        const auto length        = (step * block_count);


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length    = length;
        lib_job->operation             = DML_OP_DIF_UPDATE;

        // DIF config initialization
        lib_job->dif_config.source_application_tag_seed      = random_tag_seed.get_next();
        lib_job->dif_config.destination_application_tag_seed = random_tag_seed.get_next();
        lib_job->dif_config.source_application_tag_mask      = random_tag_seed.get_next();
        lib_job->dif_config.destination_application_tag_mask = random_tag_seed.get_next();
        lib_job->dif_config.source_reference_tag_seed        = random_ref_tag_seed.get_next();
        lib_job->dif_config.destination_reference_tag_seed   = random_ref_tag_seed.get_next();
        lib_job->dif_config.block_size                       = static_cast<dml_dif_block_size_t>(block_size_index);
        lib_job->dif_config.flags                            = flags;

        // DIF variables
        constexpr auto crc_seed                 = static_cast<uint16_t>((DML_DIF_FLAG_INVERT_CRC_SEED & flags) ? (0xFFFFu) : (0u));
        constexpr auto invert_flag              = (flags & DML_DIF_FLAG_INVERT_CRC_RESULT) ? (true) : (false);
        constexpr auto src_ref_tag_update_value = static_cast<uint8_t>((flags & DML_DIF_FLAG_SRC_FIX_REF_TAG) ? (0u) : (1u));
        constexpr auto dst_ref_tag_update_value = static_cast<uint8_t>((flags & DML_DIF_FLAG_DST_FIX_REF_TAG) ? (0u) : (1u));
        constexpr auto src_app_tag_update_value = static_cast<uint8_t>((flags & DML_DIF_FLAG_SRC_INC_APP_TAG) ? (1u) : (0u));
        constexpr auto dst_app_tag_update_value = static_cast<uint8_t>((flags & DML_DIF_FLAG_DST_INC_APP_TAG) ? (1u) : (0u));
        const auto src_application_tag_mask     = static_cast<uint16_t>(~(lib_job->dif_config.source_application_tag_mask));
        const auto dst_application_tag_mask     = static_cast<uint16_t>(~(lib_job->dif_config.destination_application_tag_mask));

        constexpr auto update_guard   = (flags & DML_DIF_FLAG_DST_PASS_GUARD)   ? (false) : (true);
        constexpr auto update_app_tag = (flags & DML_DIF_FLAG_DST_PASS_APP_TAG) ? (false) : (true);
        constexpr auto update_ref_tag = (flags & DML_DIF_FLAG_DST_PASS_REF_TAG) ? (false) : (true);


        // Vectors initialization
        std::generate(lib_job->source_first_ptr,
                      lib_job->source_first_ptr +
                      lib_job->source_length,
                      random_value);


        // Reference job initialization
        ref_job.destination_first = dml::test::global_allocator::allocate(lib_job->source_first_ptr,
                                                                          lib_job->source_first_ptr +
                                                                          lib_job->source_length);


        auto source_current_ptr      = &lib_job->source_first_ptr[0];
        auto destination_current_ptr = &ref_job.destination_first[0];

        auto src_application_tag = lib_job->dif_config.source_application_tag_seed;
        auto dst_application_tag = lib_job->dif_config.destination_application_tag_seed;
        auto src_reference_tag   = lib_job->dif_config.source_reference_tag_seed;
        auto dst_reference_tag   = lib_job->dif_config.destination_reference_tag_seed;


        for (auto i = 0u; i < block_count; ++i)
        {
            auto current_crc = test::crc_16u(source_current_ptr, block_size, crc_seed, DEFAULT_POLYNOMIAL);

            auto source_dif_ptr      = (source_current_ptr      + block_size);
            auto destination_dif_ptr = (destination_current_ptr + block_size);

            *reinterpret_cast<uint16_t *>(&source_dif_ptr[0]) = dml::reference::reverse_bytes<uint16_t>((invert_flag) ?
                                                                                                            (~current_crc) :
                                                                                                            (current_crc));
            *reinterpret_cast<uint16_t *>(&source_dif_ptr[2]) = dml::reference::reverse_bytes<uint16_t>(src_application_tag & src_application_tag_mask);
            *reinterpret_cast<uint32_t *>(&source_dif_ptr[4]) = dml::reference::reverse_bytes<uint32_t>(src_reference_tag);


            dml::core::dispatch::mem_move(source_current_ptr, destination_current_ptr, step);

            if constexpr (update_guard)
            {
                uint16_t guard = dml::reference::reverse_bytes<uint16_t>((invert_flag) ?
                                                                             (~current_crc) :
                                                                             (current_crc));
                *reinterpret_cast<uint16_t *>(&destination_dif_ptr[0]) = guard;
            }

            if constexpr (update_app_tag)
            {
                uint16_t updated_application_tag = dml::reference::reverse_bytes<uint16_t>(dst_application_tag & dst_application_tag_mask);
                *reinterpret_cast<uint16_t *>(&destination_dif_ptr[2]) = updated_application_tag;
            }

            if constexpr (update_ref_tag)
            {
                uint32_t updated_reference_tag = dml::reference::reverse_bytes<uint32_t>(dst_reference_tag);
                *reinterpret_cast<uint32_t *>(&destination_dif_ptr[4]) = updated_reference_tag;
            }


            src_application_tag     += src_app_tag_update_value;
            dst_application_tag     += dst_app_tag_update_value;
            src_reference_tag       += src_ref_tag_update_value;
            dst_reference_tag       += dst_ref_tag_update_value;
            source_current_ptr      += step;
            destination_current_ptr += step;
        }
    }


/** Custom test register **/
#define OWN_SUBMIT_DIF_UPDATE_TEST(name, flags) \
        DML_JOB_API_TEST_GENERATOR(dml_dif_update, name ##_ ##512) \
        { \
            own_create_jobs<DML_DIF_BLOCK_SIZE_512, DIF_BLOCK_SIZE_512, flags>(lib_job, ref_job); \
        } \
        DML_JOB_API_TEST_GENERATOR(dml_dif_update, name ##_ ##520) \
        { \
            own_create_jobs<DML_DIF_BLOCK_SIZE_520, DIF_BLOCK_SIZE_520, flags>(lib_job, ref_job); \
        } \
        DML_JOB_API_TEST_GENERATOR(dml_dif_update, name ##_ ##4096) \
        { \
            own_create_jobs<DML_DIF_BLOCK_SIZE_4096, DIF_BLOCK_SIZE_4096, flags>(lib_job, ref_job); \
        } \
        DML_JOB_API_TEST_GENERATOR(dml_dif_update, name ##_ ##4104) \
        { \
            own_create_jobs<DML_DIF_BLOCK_SIZE_4104, DIF_BLOCK_SIZE_4104, flags>(lib_job, ref_job); \
        } \
        DML_JOB_API_TEST_REGISTER(dml_dif_update, name ##_ ##512);  \
        DML_JOB_API_TEST_REGISTER(dml_dif_update, name ##_ ##520);  \
        DML_JOB_API_TEST_REGISTER(dml_dif_update, name ##_ ##4096); \
        DML_JOB_API_TEST_REGISTER(dml_dif_update, name ##_ ##4104);


    /**
     * @brief Tests the operation without flags
     */
    OWN_SUBMIT_DIF_UPDATE_TEST(ta_no_flags, 0u);

    /**
     * @brief Tests the operation with DML_DIF_FLAG_DST_FIX_REF_TAG flag
     */
    OWN_SUBMIT_DIF_UPDATE_TEST(ta_fix_ref_tag_flag, DML_DIF_FLAG_DST_FIX_REF_TAG);

    /**
     * @brief Tests the operation with DML_DIF_FLAG_DST_INC_APP_TAG flag
     */
    OWN_SUBMIT_DIF_UPDATE_TEST(ta_inc_app_tag_flag, DML_DIF_FLAG_DST_INC_APP_TAG);

    /**
     * @brief Tests the operation with DML_DIF_FLAG_INVERT_CRC_SEED flag
     */
    OWN_SUBMIT_DIF_UPDATE_TEST(ta_invert_crc_seed_flag, DML_DIF_FLAG_INVERT_CRC_SEED);

    /**
     * @brief Tests the operation with DML_DIF_FLAG_INVERT_CRC_RESULT flag
     */
    OWN_SUBMIT_DIF_UPDATE_TEST(ta_invert_crc_result_flag, DML_DIF_FLAG_INVERT_CRC_RESULT);

    /**
     * @brief Tests the operation with DML_DIF_FLAG_DST_PASS_GUARD flag
     */
    OWN_SUBMIT_DIF_UPDATE_TEST(ta_invert_guard_pass_flag, DML_DIF_FLAG_DST_PASS_GUARD);

    /**
     * @brief Tests the operation with DML_DIF_FLAG_DST_PASS_APP_TAG flag
     */
    OWN_SUBMIT_DIF_UPDATE_TEST(ta_invert_app_tag_pass_flag, DML_DIF_FLAG_DST_PASS_APP_TAG);

    /**
     * @brief Tests the operation with DML_DIF_FLAG_DST_PASS_REF_TAG flag
     */
    OWN_SUBMIT_DIF_UPDATE_TEST(ta_invert_ref_tag_pass_flag, DML_DIF_FLAG_DST_PASS_REF_TAG);

}
