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
#include <optimization_dispatcher.hpp>

#include "ref_crc.hpp"
#include "t_common.hpp"
#include "t_random_generator.hpp"
#include "t_random_parameters.hpp"

namespace dml
{
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

    /** Default range for tests */
    constexpr dml::test::range_t<uint32_t> DEFAULT_RANGE = { TEST_MIN_MEMORY_BLOCK_SIZE, TEST_MAX_MEMORY_BLOCK_SIZE };

    /** Enables All F Detection */
    constexpr auto DIF_F_ALL_FLAG = DML_DIF_FLAG_SRC_F_DETECT_ALL | DML_DIF_FLAG_SRC_F_ENABLE_ERROR;

    /** Default POLYNOMIAL for crc operation */
    constexpr auto DEFAULT_POLYNOMIAL = 0x8BB7u;

    /** Block sizes for the DIF operations */
    constexpr auto BLOCK_SIZES = std::array<uint32_t, 4u>{ 512u, 520u, 4096u, 4104u };

    /** Fills the batch with the DML_OP_DIF_CHECK operation */
    static inline auto init_dif_check_operation(dml::test::job_t &lib_job, uint32_t index, dml::test::reference_job_t &ref_job) -> void
    {
        // Variables
        static const auto seed                = test_system::get_seed();
        static auto       random_length       = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        static auto       random_value        = dml::test::random_t<uint8_t>(seed);
        static auto       random_tag_seed     = dml::test::random_t<uint16_t>(seed);
        static auto       random_ref_tag_seed = dml::test::random_t<uint32_t>(seed);
        static auto       random_flag         = dml::test::random_t<uint32_t>({ 0u, 1u }, seed);
        const auto        block_count         = random_length.get_next();
        const auto        block_size_index    = random_length.get_next() % BLOCK_SIZES.size();
        const auto        block_size          = BLOCK_SIZES[block_size_index];
        const auto        source_length       = ((block_size + 8u) * block_count);
        const auto        destination_length  = (block_size * block_count);
        const auto        dif_error_expected  = 0u;
        auto              flags               = uint32_t(0u);
        auto dif_config_ptr = reinterpret_cast<dml_dif_config_t *>(dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t)));

        // Library job initialization
        auto source_first_ptr = dml::test::global_allocator::allocate_ptr(source_length);

        // Flags generation
        if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_INVERT_CRC_SEED;
        }
        else if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_INVERT_CRC_RESULT;
        }
        else if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_SRC_FIX_REF_TAG;
        }
        else if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_SRC_INC_APP_TAG;
        }
        else if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_SRC_F_DETECT_TAGS;
        }
        else if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_SRC_F_DETECT_APP_TAG;
        }
        else if (random_flag.get_next())
        {
            flags |= DIF_F_ALL_FLAG;
        }

        // DIF config initialization
        dif_config_ptr->source_application_tag_seed = random_tag_seed.get_next();
        dif_config_ptr->source_application_tag_mask = random_tag_seed.get_next();
        dif_config_ptr->source_reference_tag_seed   = random_ref_tag_seed.get_next();
        dif_config_ptr->block_size                  = static_cast<dml_dif_block_size_t>(block_size_index);
        dif_config_ptr->flags                       = flags;

        if (0u == dif_config_ptr->source_application_tag_mask)
        {
            dif_config_ptr->source_application_tag_mask = ~dif_config_ptr->source_application_tag_mask;
        }

        // DIF variables
        const auto crc_seed             = static_cast<uint16_t>((DML_DIF_FLAG_INVERT_CRC_SEED & flags) ? 0xFFFFu : 0u);
        const auto invert_flag          = (flags & DML_DIF_FLAG_INVERT_CRC_RESULT) ? 1u : 0u;
        const auto ref_tag_update_value = static_cast<uint8_t>((flags & DML_DIF_FLAG_SRC_FIX_REF_TAG) ? 0u : 1u);
        const auto app_tag_update_value = static_cast<uint8_t>((flags & DML_DIF_FLAG_SRC_INC_APP_TAG) ? 1u : 0u);
        const auto application_tag_mask = static_cast<uint16_t>(~(dif_config_ptr->source_application_tag_mask));

        // Buffer initialization
        auto &memory_buffer = dml::test::global_allocator::allocate(destination_length);

        // Reference vectors initialization
        std::generate(memory_buffer.begin(), memory_buffer.end(), random_value);

        auto       source_current_ptr      = &memory_buffer[0];
        auto       destination_current_ptr = &source_first_ptr[0];
        const auto destination_end_ptr     = &source_first_ptr[source_length - 1u];

        auto application_tag = static_cast<uint16_t>(dif_config_ptr->source_application_tag_seed);
        auto reference_tag   = static_cast<uint32_t>(dif_config_ptr->source_reference_tag_seed);

        const auto [_, temp_buffer_ptr] = extend_ref_job(ref_job);

        while (destination_current_ptr < destination_end_ptr)
        {
            dml::core::dispatch::mem_move(source_current_ptr, destination_current_ptr, block_size);

            auto current_crc = test::crc_16u(destination_current_ptr, block_size, crc_seed, DEFAULT_POLYNOMIAL);

            auto dif_ptr = (destination_current_ptr + block_size);

            *reinterpret_cast<uint16_t *>(&dif_ptr[0]) =
                dml::reference::reverse_bytes<uint16_t>((invert_flag) ? ~current_crc : current_crc);
            *reinterpret_cast<uint16_t *>(&dif_ptr[2]) = dml::reference::reverse_bytes<uint16_t>(application_tag & application_tag_mask);
            *reinterpret_cast<uint32_t *>(&dif_ptr[4]) = dml::reference::reverse_bytes<uint32_t>(reference_tag);

            application_tag += app_tag_update_value;
            reference_tag += ref_tag_update_value;
            source_current_ptr += block_size;
            destination_current_ptr += (block_size + 8u);
        }

        // Apply corruption
        if (0u != dif_error_expected || (DML_DIF_FLAG_SRC_F_DETECT_TAGS & flags) || (DML_DIF_FLAG_SRC_F_DETECT_APP_TAG & flags))
        {
            const auto random_block    = (random_length.get_next() % block_count);
            const auto actual_block    = ((8u + block_size) * random_block);
            const auto actual_position = (actual_block + block_size);

            temp_buffer_ptr->result |= dif_error_expected;
            temp_buffer_ptr->offset = dif_error_expected ? actual_block : 0u;
            temp_buffer_ptr->status = dif_error_expected ? DML_STATUS_DIF_CHECK_ERROR : DML_STATUS_OK;

            if (static_cast<bool>(DIF_F_ALL_FLAG == (DIF_F_ALL_FLAG & flags)))
            {
                *reinterpret_cast<uint64_t *>(&source_first_ptr[actual_position]) = 0xFFFFFFFFFFFFFFFFull;
            }

            if (static_cast<bool>(DML_DIF_CHECK_GUARD_MISMATCH & dif_error_expected))
            {
                *reinterpret_cast<uint16_t *>(&source_first_ptr[actual_position]) =
                    ~(*reinterpret_cast<uint16_t *>(&source_first_ptr[actual_position]));
            }

            if (static_cast<bool>(DML_DIF_CHECK_APPLICATION_TAG_MISMATCH & dif_error_expected))
            {
                *reinterpret_cast<uint16_t *>(&source_first_ptr[actual_position + 2u]) =
                    ~(*reinterpret_cast<uint16_t *>(&source_first_ptr[actual_position + 2u]));
            }

            if (static_cast<bool>(DML_DIF_CHECK_REFERENCE_TAG_MISMATCH & dif_error_expected))
            {
                *reinterpret_cast<uint32_t *>(&source_first_ptr[actual_position + 4u]) =
                    ~(*reinterpret_cast<uint32_t *>(&source_first_ptr[actual_position + 4u]));
            }

            if (static_cast<bool>(DML_DIF_FLAG_SRC_F_DETECT_APP_TAG & flags))
            {
                *reinterpret_cast<uint16_t *>(&source_first_ptr[actual_position + 2u]) = 0xFFFFu;
            }

            if (static_cast<bool>(DML_DIF_FLAG_SRC_F_DETECT_TAGS & flags))
            {
                *reinterpret_cast<uint16_t *>(&source_first_ptr[actual_position + 2u]) = 0xFFFFu;
                *reinterpret_cast<uint32_t *>(&source_first_ptr[actual_position + 4u]) = 0xFFFFFFFFu;
            }

            if (static_cast<bool>((DML_DIF_FLAG_SRC_F_DETECT_TAGS | DML_DIF_FLAG_SRC_F_DETECT_APP_TAG) & flags))
            {
                temp_buffer_ptr->status = DML_STATUS_OK;
                temp_buffer_ptr->result = 0u;
                temp_buffer_ptr->offset = 0u;
            }
        }

        auto status = dml_batch_set_dif_check_by_index(&*lib_job, index, &source_first_ptr[0], source_length, dif_config_ptr, 0u);
        EXPECT_EQ(DML_STATUS_OK, status);
    }

    /** Fills the batch with the DML_OP_DIF_INSERT operation */
    static inline auto init_dif_insert_operation(dml::test::job_t &lib_job, uint32_t index, dml::test::reference_job_t &ref_job) -> void
    {
        // Variables
        static const auto seed                = test_system::get_seed();
        static auto       random_length       = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        static auto       random_value        = dml::test::random_t<uint8_t>(seed);
        static auto       random_tag_seed     = dml::test::random_t<uint16_t>(seed);
        static auto       random_ref_tag_seed = dml::test::random_t<uint32_t>(seed);
        static auto       random_flag         = dml::test::random_t<uint32_t>({ 0u, 1u }, seed);
        const auto        block_count         = random_length.get_next();
        const auto        block_size_index    = random_length.get_next() % BLOCK_SIZES.size();
        const auto        block_size          = BLOCK_SIZES[block_size_index];
        const auto        source_length       = (block_size * block_count);
        const auto        destination_length  = ((block_size + 8u) * block_count);
        auto              flags               = uint32_t(0u);
        auto dif_config_ptr = reinterpret_cast<dml_dif_config_t *>(dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t)));

        // Flags generation
        if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_INVERT_CRC_SEED;
        }
        if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_INVERT_CRC_RESULT;
        }
        if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_DST_FIX_REF_TAG;
        }
        if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_DST_INC_APP_TAG;
        }

        // Library job initialization
        auto source_first_ptr      = dml::test::global_allocator::allocate_ptr(source_length);
        auto destination_first_ptr = dml::test::global_allocator::allocate_ptr(destination_length);

        // DIF config initialization
        dif_config_ptr->destination_application_tag_seed = random_tag_seed.get_next();
        dif_config_ptr->destination_application_tag_mask = random_tag_seed.get_next();
        dif_config_ptr->destination_reference_tag_seed   = random_ref_tag_seed.get_next();
        dif_config_ptr->block_size                       = static_cast<dml_dif_block_size_t>(block_size_index);
        dif_config_ptr->flags                            = flags;

        // DIF variables
        const auto crc_seed             = static_cast<uint16_t>((DML_DIF_FLAG_INVERT_CRC_SEED & flags) ? (0xFFFFu) : (0u));
        const auto invert_flag          = (flags & DML_DIF_FLAG_INVERT_CRC_RESULT) ? (true) : (false);
        const auto ref_tag_update_value = static_cast<uint8_t>((flags & DML_DIF_FLAG_DST_FIX_REF_TAG) ? (0u) : (1u));
        const auto app_tag_update_value = static_cast<uint8_t>((flags & DML_DIF_FLAG_DST_INC_APP_TAG) ? (1u) : (0u));
        const auto application_tag_mask = static_cast<uint16_t>(~(dif_config_ptr->destination_application_tag_mask));

        // Vectors initialization
        std::generate(source_first_ptr, source_first_ptr + source_length, random_value);

        const auto [_, temp_buffer_ptr] = extend_ref_job(ref_job);

        temp_buffer_ptr->destination_first = dml::test::global_allocator::allocate(destination_length);

        // Reference vectors initialization
        auto       source_current_ptr      = source_first_ptr;
        auto       destination_current_ptr = &temp_buffer_ptr->destination_first.front();
        const auto destination_end_ptr     = &temp_buffer_ptr->destination_first.back();

        auto application_tag = static_cast<uint16_t>(dif_config_ptr->destination_application_tag_seed);
        auto reference_tag   = static_cast<uint32_t>(dif_config_ptr->destination_reference_tag_seed);

        while (destination_current_ptr < destination_end_ptr)
        {
            dml::core::dispatch::mem_move(source_current_ptr, destination_current_ptr, block_size);

            auto current_crc = test::crc_16u(destination_current_ptr, block_size, crc_seed, 0x8BB7u);

            auto dif_ptr = (destination_current_ptr + block_size);

            *reinterpret_cast<uint16_t *>(&dif_ptr[0]) =
                dml::reference::reverse_bytes<uint16_t>((invert_flag) ? ~current_crc : current_crc);
            *reinterpret_cast<uint16_t *>(&dif_ptr[2]) = dml::reference::reverse_bytes<uint16_t>(application_tag & application_tag_mask);
            *reinterpret_cast<uint32_t *>(&dif_ptr[4]) = dml::reference::reverse_bytes(reference_tag);

            application_tag += app_tag_update_value;
            reference_tag += ref_tag_update_value;
            source_current_ptr += block_size;
            destination_current_ptr += (block_size + 8u);
        }

        auto status = dml_batch_set_dif_insert_by_index(&*lib_job,
                                                        index,
                                                        source_first_ptr,
                                                        source_length,
                                                        dif_config_ptr,
                                                        destination_first_ptr,
                                                        destination_length,
                                                        0u);
        EXPECT_EQ(DML_STATUS_OK, status);
    }

    /** Fills the batch with the DML_OP_DIF_STRIP operation */
    static inline auto init_dif_strip_operation(dml::test::job_t &lib_job, uint32_t index, dml::test::reference_job_t &ref_job) -> void
    {
        // Variables
        static const auto seed                = test_system::get_seed();
        static auto       random_length       = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        static auto       random_value        = dml::test::random_t<uint8_t>(seed);
        static auto       random_tag_seed     = dml::test::random_t<uint16_t>(seed);
        static auto       random_ref_tag_seed = dml::test::random_t<uint32_t>(seed);
        static auto       random_flag         = dml::test::random_t<uint32_t>({ 0u, 1u }, seed);
        const auto        block_count         = random_length.get_next();
        const auto        block_size_index    = random_length.get_next() % BLOCK_SIZES.size();
        const auto        block_size          = BLOCK_SIZES[block_size_index];
        const auto        source_length       = ((block_size + 8u) * block_count);
        const auto        destination_length  = (block_size * block_count);
        auto              flags               = uint32_t(0u);
        auto dif_config_ptr = reinterpret_cast<dml_dif_config_t *>(dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t)));

        // Flags generation
        if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_INVERT_CRC_SEED;
        }
        if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_INVERT_CRC_RESULT;
        }
        if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_SRC_FIX_REF_TAG;
        }
        if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_SRC_INC_APP_TAG;
        }

        // Library job initialization
        auto source_first_ptr      = dml::test::global_allocator::allocate_ptr(source_length);
        // Source size is intentionally used for destination buffer so than they not be adjacent. See DML_STATUS_DIF_STRIP_ADJACENT_ERROR.
        auto destination_first_ptr = dml::test::global_allocator::allocate_ptr(source_length);

        // DIF config initialization
        dif_config_ptr->source_application_tag_seed = random_tag_seed.get_next();
        dif_config_ptr->source_application_tag_mask = random_tag_seed.get_next();
        dif_config_ptr->source_reference_tag_seed   = random_ref_tag_seed.get_next();
        dif_config_ptr->block_size                  = static_cast<dml_dif_block_size_t>(block_size_index);
        dif_config_ptr->flags                       = flags;

        // DIF variables
        const auto crc_seed             = static_cast<uint16_t>((DML_DIF_FLAG_INVERT_CRC_SEED & flags) ? (0xFFFFu) : (0u));
        const auto invert_flag          = (flags & DML_DIF_FLAG_INVERT_CRC_RESULT) ? (true) : (false);
        const auto ref_tag_update_value = static_cast<uint8_t>((flags & DML_DIF_FLAG_SRC_FIX_REF_TAG) ? (0u) : (1u));
        const auto app_tag_update_value = static_cast<uint8_t>((flags & DML_DIF_FLAG_SRC_INC_APP_TAG) ? (1u) : (0u));
        const auto application_tag_mask = static_cast<uint16_t>(~(dif_config_ptr->source_application_tag_mask));

        // Reference job initialization
        const auto [_, temp_buffer_ptr]    = extend_ref_job(ref_job);
        temp_buffer_ptr->destination_first = dml::test::global_allocator::allocate(destination_length);

        // Reference vectors initialization
        std::generate(temp_buffer_ptr->destination_first.begin(), temp_buffer_ptr->destination_first.end(), random_value);

        auto       source_current_ptr      = &temp_buffer_ptr->destination_first.front();
        auto       destination_current_ptr = &source_first_ptr[0];
        const auto destination_end_ptr     = &source_first_ptr[source_length - 1u];

        auto application_tag = static_cast<uint16_t>(dif_config_ptr->source_application_tag_seed);
        auto reference_tag   = static_cast<uint32_t>(dif_config_ptr->source_reference_tag_seed);

        while (destination_current_ptr < destination_end_ptr)
        {
            dml::core::dispatch::mem_move(source_current_ptr, destination_current_ptr, block_size);

            auto current_crc = test::crc_16u(destination_current_ptr, block_size, crc_seed, DEFAULT_POLYNOMIAL);

            auto dif_ptr = (destination_current_ptr + block_size);

            *reinterpret_cast<uint16_t *>(&dif_ptr[0]) =
                dml::reference::reverse_bytes<uint16_t>((invert_flag) ? ~current_crc : current_crc);
            *reinterpret_cast<uint16_t *>(&dif_ptr[2]) = dml::reference::reverse_bytes<uint16_t>(application_tag & application_tag_mask);
            *reinterpret_cast<uint32_t *>(&dif_ptr[4]) = dml::reference::reverse_bytes<uint32_t>(reference_tag);

            application_tag += app_tag_update_value;
            reference_tag += ref_tag_update_value;
            source_current_ptr += block_size;
            destination_current_ptr += (block_size + 8u);
        }

        auto status = dml_batch_set_dif_strip_by_index(&*lib_job,
                                                       index,
                                                       source_first_ptr,
                                                       source_length,
                                                       dif_config_ptr,
                                                       destination_first_ptr,
                                                       destination_length,
                                                       0u);
        EXPECT_EQ(DML_STATUS_OK, status);
    }

    /** Fills the batch with the DML_OP_DIF_STRIP operation */
    static inline auto init_dif_update_operation(dml::test::job_t &lib_job, uint32_t index, dml::test::reference_job_t &ref_job) -> void
    {
        // Variables
        static const auto seed                = test_system::get_seed();
        static auto       random_length       = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        static auto       random_value        = dml::test::random_t<uint8_t>(seed);
        static auto       random_tag_seed     = dml::test::random_t<uint16_t>(seed);
        static auto       random_ref_tag_seed = dml::test::random_t<uint32_t>(seed);
        static auto       random_flag         = dml::test::random_t<uint32_t>({ 0u, 1u }, seed);
        const auto        block_count         = random_length.get_next();
        const auto        block_size_index    = random_length.get_next() % BLOCK_SIZES.size();
        const auto        block_size          = BLOCK_SIZES[block_size_index];
        const auto        step                = (block_size + 8u);
        const auto        length              = (step * block_count);
        auto              flags               = uint32_t(0u);
        auto dif_config_ptr = reinterpret_cast<dml_dif_config_t *>(dml::test::global_allocator::allocate_ptr(sizeof(dml_dif_config_t)));

        // Flags generation
        if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_INVERT_CRC_SEED;
        }
        if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_INVERT_CRC_RESULT;
        }
        if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_SRC_FIX_REF_TAG;
        }
        if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_DST_FIX_REF_TAG;
        }
        if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_SRC_INC_APP_TAG;
        }
        if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_DST_INC_APP_TAG;
        }
        if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_DST_PASS_GUARD;
        }
        if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_DST_PASS_APP_TAG;
        }
        if (random_flag.get_next())
        {
            flags |= DML_DIF_FLAG_DST_PASS_REF_TAG;
        }

        // Library job initialization
        auto source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        auto destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        auto operation             = DML_OP_DIF_UPDATE;

        // DIF config initialization
        dif_config_ptr->source_application_tag_seed      = random_tag_seed.get_next();
        dif_config_ptr->destination_application_tag_seed = random_tag_seed.get_next();
        dif_config_ptr->source_application_tag_mask      = random_tag_seed.get_next();
        dif_config_ptr->destination_application_tag_mask = random_tag_seed.get_next();
        dif_config_ptr->source_reference_tag_seed        = random_ref_tag_seed.get_next();
        dif_config_ptr->destination_reference_tag_seed   = random_ref_tag_seed.get_next();
        dif_config_ptr->block_size                       = static_cast<dml_dif_block_size_t>(block_size_index);
        dif_config_ptr->flags                            = flags;

        // DIF variables
        const auto crc_seed                 = static_cast<uint16_t>((DML_DIF_FLAG_INVERT_CRC_SEED & flags) ? (0xFFFFu) : (0u));
        const auto invert_flag              = (flags & DML_DIF_FLAG_INVERT_CRC_RESULT) ? (true) : (false);
        const auto src_ref_tag_update_value = static_cast<uint8_t>((flags & DML_DIF_FLAG_SRC_FIX_REF_TAG) ? (0u) : (1u));
        const auto dst_ref_tag_update_value = static_cast<uint8_t>((flags & DML_DIF_FLAG_DST_FIX_REF_TAG) ? (0u) : (1u));
        const auto src_app_tag_update_value = static_cast<uint8_t>((flags & DML_DIF_FLAG_SRC_INC_APP_TAG) ? (1u) : (0u));
        const auto dst_app_tag_update_value = static_cast<uint8_t>((flags & DML_DIF_FLAG_DST_INC_APP_TAG) ? (1u) : (0u));
        const auto src_application_tag_mask = static_cast<uint16_t>(~(dif_config_ptr->source_application_tag_mask));
        const auto dst_application_tag_mask = static_cast<uint16_t>(~(dif_config_ptr->destination_application_tag_mask));

        const auto update_guard   = (flags & DML_DIF_FLAG_DST_PASS_GUARD) ? (false) : (true);
        const auto update_app_tag = (flags & DML_DIF_FLAG_DST_PASS_APP_TAG) ? (false) : (true);
        const auto update_ref_tag = (flags & DML_DIF_FLAG_DST_PASS_REF_TAG) ? (false) : (true);

        // Vectors initialization
        std::generate(&source_first_ptr[0], &source_first_ptr[length], random_value);

        const auto [_, temp_buffer_ptr]    = extend_ref_job(ref_job);
        temp_buffer_ptr->destination_first = dml::test::global_allocator::allocate(&source_first_ptr[0], &source_first_ptr[length]);

        auto source_current_ptr      = &source_first_ptr[0];
        auto destination_current_ptr = &temp_buffer_ptr->destination_first[0];

        auto src_application_tag = dif_config_ptr->source_application_tag_seed;
        auto dst_application_tag = dif_config_ptr->destination_application_tag_seed;
        auto src_reference_tag   = dif_config_ptr->source_reference_tag_seed;
        auto dst_reference_tag   = dif_config_ptr->destination_reference_tag_seed;

        for (auto i = 0u; i < block_count; ++i)
        {
            auto current_crc = test::crc_16u(source_current_ptr, block_size, crc_seed, DEFAULT_POLYNOMIAL);

            auto source_dif_ptr      = (source_current_ptr + block_size);
            auto destination_dif_ptr = (destination_current_ptr + block_size);

            *reinterpret_cast<uint16_t *>(&source_dif_ptr[0]) =
                dml::reference::reverse_bytes<uint16_t>((invert_flag) ? (~current_crc) : (current_crc));
            *reinterpret_cast<uint16_t *>(&source_dif_ptr[2]) =
                dml::reference::reverse_bytes<uint16_t>(src_application_tag & src_application_tag_mask);
            *reinterpret_cast<uint32_t *>(&source_dif_ptr[4]) = dml::reference::reverse_bytes<uint32_t>(src_reference_tag);

            dml::core::dispatch::mem_move(source_current_ptr, destination_current_ptr, step);

            if (update_guard)
            {
                uint16_t guard = dml::reference::reverse_bytes<uint16_t>((invert_flag) ? (~current_crc) : (current_crc));
                *reinterpret_cast<uint16_t *>(&destination_dif_ptr[0]) = guard;
            }

            if (update_app_tag)
            {
                uint16_t updated_application_tag = dml::reference::reverse_bytes<uint16_t>(dst_application_tag & dst_application_tag_mask);
                *reinterpret_cast<uint16_t *>(&destination_dif_ptr[2]) = updated_application_tag;
            }

            if (update_ref_tag)
            {
                uint32_t updated_reference_tag                         = dml::reference::reverse_bytes<uint32_t>(dst_reference_tag);
                *reinterpret_cast<uint32_t *>(&destination_dif_ptr[4]) = updated_reference_tag;
            }

            src_application_tag += src_app_tag_update_value;
            dst_application_tag += dst_app_tag_update_value;
            src_reference_tag += src_ref_tag_update_value;
            dst_reference_tag += dst_ref_tag_update_value;
            source_current_ptr += step;
            destination_current_ptr += step;
        }

        auto status = dml_batch_set_dif_update_by_index(&*lib_job,
                                                        index,
                                                        source_first_ptr,
                                                        length,
                                                        dif_config_ptr,
                                                        destination_first_ptr,
                                                        length,
                                                        0u);
        EXPECT_EQ(DML_STATUS_OK, status);
    }

    /**
     * @brief Tests the operation with random internal operations
     */
    DML_JOB_API_TEST_GENERATOR(dml_batch, ta_dif_operations)
    {
        auto operations_count = 4u;
        auto byte_length      = 0u;
        auto index            = uint32_t{ 0 };

        // Batch initialization
        auto status = dml_get_batch_size(&*lib_job, operations_count, &byte_length);
        EXPECT_EQ(DML_STATUS_OK, status);

        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(byte_length);
        lib_job->destination_length    = byte_length;
        lib_job->operation             = DML_OP_BATCH;

        init_dif_check_operation(lib_job, index, ref_job);
        index++;

        init_dif_insert_operation(lib_job, index, ref_job);
        index++;

        init_dif_strip_operation(lib_job, index, ref_job);
        index++;

        init_dif_update_operation(lib_job, index, ref_job);
    }

    // Test registers
    DML_JOB_API_TEST_REGISTER(dml_batch, ta_dif_operations);

}  // namespace dml
