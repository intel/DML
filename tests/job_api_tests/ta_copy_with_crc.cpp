/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains algorithmic tests for DML_OP_COPY_CRC operation
 * @date 5/7/2020
 *
 */
#include "t_common.hpp"
#include "t_random_generator.hpp"
#include "t_random_parameters.hpp"
#include "ref_crc.hpp"


namespace dml
{
    /** Default range for tests */
    constexpr dml::test::range_t<uint32_t> DEFAULT_RANGE = {TEST_MIN_MEMORY_BLOCK_SIZE, TEST_MAX_MEMORY_BLOCK_SIZE};


    /**
     * @brief Tests the operation without flags
     */
    DML_JOB_API_TEST_GENERATOR(dml_copy_crc, ta_no_flags)
    { 
        // Variables
        const auto seed     = test_system::get_seed();
        auto random_length  = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_value   = dml::test::random_t<uint8_t>(seed);
        const auto crc_seed = 0u;
        const auto length   = random_length.get_next();
        const auto flags    = 0u;        


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length;
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length    = length;
        lib_job->crc_checksum_ptr      = 
            reinterpret_cast<uint32_t *>(dml::test::global_allocator::allocate_ptr(sizeof(uint32_t)));
        lib_job->operation             = DML_OP_COPY_CRC;
        lib_job->flags                 = flags;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr, 
                      lib_job->source_first_ptr + 
                      lib_job->source_length, 
                      random_value);
        
        const auto crc_reference_value = dml::reference::calculate_crc<uint32_t, flags>(lib_job->source_first_ptr,
                                                                                        lib_job->source_first_ptr + 
                                                                                        lib_job->source_length, 
                                                                                        crc_seed);


        // Reference job initialization
        ref_job.destination_first = dml::test::global_allocator::allocate(lib_job->source_first_ptr,
                                                                          lib_job->source_first_ptr + 
                                                                          lib_job->source_length);
        ref_job.crc_checksum_ptr  = std::make_unique<uint32_t>(crc_reference_value); 
        ref_job.status            = DML_STATUS_OK;
    }


    /**
     * @brief Tests the operation with DML_FLAG_CRC_READ_SEED flag
     */
    DML_JOB_API_TEST_GENERATOR(dml_copy_crc, ta_read_seed_flag)
    { 
        // Variables
        const auto seed     = test_system::get_seed();
        auto random_length  = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_seed    = dml::test::random_t<uint32_t>(seed);
        auto random_value   = dml::test::random_t<uint8_t>(seed);
        const auto crc_seed = random_seed.get_next();
        const auto length   = random_length.get_next();
        const auto flags    = DML_FLAG_CRC_READ_SEED;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length; 
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length    = length;
        lib_job->crc_checksum_ptr      = 
            reinterpret_cast<uint32_t *>(dml::test::global_allocator::allocate_ptr(sizeof(uint32_t)));
        lib_job->operation             = DML_OP_COPY_CRC;
        lib_job->flags                 = flags; 
        
        (*lib_job->crc_checksum_ptr)   = crc_seed;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr, 
                      lib_job->source_first_ptr + 
                      lib_job->source_length, 
                      random_value);
        
        const auto crc_reference_value = dml::reference::calculate_crc<uint32_t, flags>(lib_job->source_first_ptr,
                                                                                        lib_job->source_first_ptr + 
                                                                                        lib_job->source_length, 
                                                                                        crc_seed);
        

        // Reference job initialization
        ref_job.destination_first = dml::test::global_allocator::allocate(lib_job->source_first_ptr,
                                                                          lib_job->source_first_ptr + 
                                                                          lib_job->source_length);
        ref_job.crc_checksum_ptr  = std::make_unique<uint32_t>(crc_reference_value); 
        ref_job.status            = DML_STATUS_OK;
    }
    

    /**
     * @brief Tests the operation with DML_FLAG_CRC_BYPASS_REFLECTION flag
     */
    DML_JOB_API_TEST_GENERATOR(dml_copy_crc, ta_bypass_reflection)
    { 
        // Variables
        const auto seed     = test_system::get_seed();
        auto random_length  = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_seed    = dml::test::random_t<uint32_t>(seed);
        auto random_value   = dml::test::random_t<uint8_t>(seed);
        const auto crc_seed = random_seed.get_next();
        const auto length   = random_length.get_next();
        const auto flags    = DML_FLAG_CRC_READ_SEED | DML_FLAG_CRC_BYPASS_REFLECTION;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length; 
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length    = length;
        lib_job->crc_checksum_ptr      = 
            reinterpret_cast<uint32_t *>(dml::test::global_allocator::allocate_ptr(sizeof(uint32_t)));
        lib_job->operation             = DML_OP_COPY_CRC;
        lib_job->flags                 = flags; 
        
        (*lib_job->crc_checksum_ptr)   = crc_seed;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr, 
                      lib_job->source_first_ptr + 
                      lib_job->source_length, 
                      random_value);
        
        const auto crc_reference_value = dml::reference::calculate_crc<uint32_t, flags>(lib_job->source_first_ptr,
                                                                                        lib_job->source_first_ptr + 
                                                                                        lib_job->source_length, 
                                                                                        crc_seed);
        

        // Reference job initialization
        ref_job.destination_first = dml::test::global_allocator::allocate(lib_job->source_first_ptr,
                                                                          lib_job->source_first_ptr + 
                                                                          lib_job->source_length);
        ref_job.crc_checksum_ptr  = std::make_unique<uint32_t>(crc_reference_value); 
        ref_job.status            = DML_STATUS_OK;
    }


    /**
     * @brief Tests the operation with DML_FLAG_CRC_BYPASS_DATA_REFLECTION flag
     */
    DML_JOB_API_TEST_GENERATOR(dml_copy_crc, ta_bypass_data_reflection)
    { 
        // Variables
        const auto seed     = test_system::get_seed();
        auto random_length  = dml::test::random_t<uint32_t>(DEFAULT_RANGE, seed);
        auto random_seed    = dml::test::random_t<uint32_t>(seed);
        auto random_value   = dml::test::random_t<uint8_t>(seed);
        const auto crc_seed = random_seed.get_next();
        const auto length   = random_length.get_next();
        const auto flags    = DML_FLAG_CRC_READ_SEED | DML_FLAG_CRC_BYPASS_DATA_REFLECTION;


        // Library job initialization
        lib_job->source_first_ptr      = dml::test::global_allocator::allocate_ptr(length);
        lib_job->source_length         = length; 
        lib_job->destination_first_ptr = dml::test::global_allocator::allocate_ptr(length);
        lib_job->destination_length    = length;
        lib_job->crc_checksum_ptr      = 
            reinterpret_cast<uint32_t *>(dml::test::global_allocator::allocate_ptr(sizeof(uint32_t)));
        lib_job->operation             = DML_OP_COPY_CRC;
        lib_job->flags                 = flags; 
        
        (*lib_job->crc_checksum_ptr)   = crc_seed;


        // Vectors initialization
        std::generate(lib_job->source_first_ptr, 
                      lib_job->source_first_ptr + 
                      lib_job->source_length, 
                      random_value);
        
        const auto crc_reference_value = dml::reference::calculate_crc<uint32_t, flags>(lib_job->source_first_ptr,
                                                                                        lib_job->source_first_ptr + 
                                                                                        lib_job->source_length, 
                                                                                        crc_seed);
        

        // Reference job initialization
        ref_job.destination_first = dml::test::global_allocator::allocate(lib_job->source_first_ptr,
                                                                          lib_job->source_first_ptr + 
                                                                          lib_job->source_length);
        ref_job.crc_checksum_ptr  = std::make_unique<uint32_t>(crc_reference_value); 
        ref_job.status            = DML_STATUS_OK;
    }


    // Test registers
    DML_JOB_API_TEST_REGISTER(dml_copy_crc, ta_no_flags);
    DML_JOB_API_TEST_REGISTER(dml_copy_crc, ta_read_seed_flag);
    DML_JOB_API_TEST_REGISTER(dml_copy_crc, ta_bypass_reflection);
    DML_JOB_API_TEST_REGISTER(dml_copy_crc, ta_bypass_data_reflection);

}

