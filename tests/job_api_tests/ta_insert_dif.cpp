/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains algorithmic tests for DML_OP_DIF_INSERT operation
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


    /** Custom initializer */
    template<dml_dif_block_size_t block_size_index, uint32_t block_size, uint32_t flags>
    static inline auto own_create_jobs(dml::test::job_t &lib_job, dml::test::reference_job_t &ref_job) -> void
    {
        // Variables
        const auto seed               = test_system::get_seed();
        auto random_length            = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_value             = dml::test::random_t<uint8_t>(seed);
        auto random_tag_seed          = dml::test::random_t<uint16_t>(seed);
        auto random_ref_tag_seed      = dml::test::random_t<uint32_t>(seed);
        const auto block_count        = random_length.get_next();
        const auto source_length      = (block_size * block_count);
        const auto destination_length = ((block_size + 8u) * block_count);


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(source_length);
        lib_job->source_length         = source_length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(destination_length);
        lib_job->destination_length    = destination_length;
        lib_job->operation             = DML_OP_DIF_INSERT;

        // DIF config initialization
        lib_job->dif_config.destination_application_tag_seed = random_tag_seed.get_next();
        lib_job->dif_config.destination_application_tag_mask = random_tag_seed.get_next();
        lib_job->dif_config.destination_reference_tag_seed   = random_ref_tag_seed.get_next();
        lib_job->dif_config.block_size                       = static_cast<dml_dif_block_size_t>(block_size_index);
        lib_job->dif_config.flags                            = flags;

        // DIF variables
        constexpr auto crc_seed             = static_cast<uint16_t>((DML_DIF_FLAG_INVERT_CRC_SEED & flags) ? (0xFFFFu) : (0u));
        constexpr auto invert_flag          = (flags & DML_DIF_FLAG_INVERT_CRC_RESULT) ? (true) : (false);
        constexpr auto ref_tag_update_value = static_cast<uint8_t>((flags & DML_DIF_FLAG_DST_FIX_REF_TAG) ? (0u) : (1u));
        constexpr auto app_tag_update_value = static_cast<uint8_t>((flags & DML_DIF_FLAG_DST_INC_APP_TAG) ? (1u) : (0u));
        const auto application_tag_mask = static_cast<uint16_t>(~(lib_job->dif_config.destination_application_tag_mask));


        // Vectors initialization
        std::generate(lib_job->source_first_ptr,
                      lib_job->source_first_ptr +
                      lib_job->source_length,
                      random_value);


        // Reference job initialization
        ref_job.destination_first = dml::test::global_allocator::allocate(destination_length);


        // Reference vectors initialization
        auto source_current_ptr        = lib_job->source_first_ptr;
        auto destination_current_ptr   = &ref_job.destination_first.front();
        const auto destination_end_ptr = &ref_job.destination_first.back();

        auto application_tag = static_cast<uint16_t>(lib_job->dif_config.destination_application_tag_seed);
        auto reference_tag   = static_cast<uint32_t>(lib_job->dif_config.destination_reference_tag_seed);


        while (destination_current_ptr < destination_end_ptr)
        {
            dml::core::dispatch::mem_move(source_current_ptr, destination_current_ptr, block_size);

            auto current_crc = test::crc_16u(destination_current_ptr, block_size, crc_seed, 0x8BB7u);

            auto dif_ptr = (destination_current_ptr + block_size);

            *reinterpret_cast<uint16_t *>(&dif_ptr[0]) = dml::reference::reverse_bytes<uint16_t>((invert_flag) ?
                                                                                                  ~current_crc :
                                                                                                  current_crc);
            *reinterpret_cast<uint16_t *>(&dif_ptr[2]) = dml::reference::reverse_bytes<uint16_t>(application_tag
                                                                                                  & application_tag_mask);
            *reinterpret_cast<uint32_t *>(&dif_ptr[4]) = dml::reference::reverse_bytes(reference_tag);

            application_tag         += app_tag_update_value;
            reference_tag           += ref_tag_update_value;
            source_current_ptr      += block_size;
            destination_current_ptr += (block_size + 8u);
        }
    }


/** Custom test register **/
#define OWN_SUBMIT_DIF_INSERT_TEST(name, flags) \
        DML_JOB_API_TEST_GENERATOR(dml_dif_insert, name ##_ ##512) \
        { \
            own_create_jobs<DML_DIF_BLOCK_SIZE_512, 512u, flags>(lib_job, ref_job); \
        } \
        DML_JOB_API_TEST_GENERATOR(dml_dif_insert, name ##_ ##520) \
        { \
            own_create_jobs<DML_DIF_BLOCK_SIZE_520, 520u, flags>(lib_job, ref_job); \
        } \
        DML_JOB_API_TEST_GENERATOR(dml_dif_insert, name ##_ ##4096) \
        { \
            own_create_jobs<DML_DIF_BLOCK_SIZE_4096, 4096u, flags>(lib_job, ref_job); \
        } \
        DML_JOB_API_TEST_GENERATOR(dml_dif_insert, name ##_ ##4104) \
        { \
            own_create_jobs<DML_DIF_BLOCK_SIZE_4104, 4104u, flags>(lib_job, ref_job); \
        } \
        DML_JOB_API_TEST_REGISTER(dml_dif_insert, name ##_ ##512);  \
        DML_JOB_API_TEST_REGISTER(dml_dif_insert, name ##_ ##520);  \
        DML_JOB_API_TEST_REGISTER(dml_dif_insert, name ##_ ##4096); \
        DML_JOB_API_TEST_REGISTER(dml_dif_insert, name ##_ ##4104);


    /**
     * @brief Tests the operation without flags
     */
    OWN_SUBMIT_DIF_INSERT_TEST(ta_no_flags, 0u);

    /**
     * @brief Tests the operation with invert crc seed flag
     */
    OWN_SUBMIT_DIF_INSERT_TEST(ta_invert_crc_seed_flag, DML_DIF_FLAG_INVERT_CRC_SEED);

    /**
     * @brief Tests the operation with invert crc result flag
     */
    OWN_SUBMIT_DIF_INSERT_TEST(ta_invert_crc_result_flag, DML_DIF_FLAG_INVERT_CRC_RESULT);

    /**
     * @brief Tests the operation with fix reference tag flag
     */
    OWN_SUBMIT_DIF_INSERT_TEST(ta_fix_ref_tag_flag, DML_DIF_FLAG_DST_FIX_REF_TAG);

    /**
     * @brief Tests the operation with increment application tag flag
     */
    OWN_SUBMIT_DIF_INSERT_TEST(ta_inc_app_tag_flag, DML_DIF_FLAG_DST_INC_APP_TAG);

}
