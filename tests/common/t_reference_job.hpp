/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @file
 * @brief
 * @date 4/17/2020
 */
#ifndef DML_TEST_REFERENCE_JOB
#define DML_TEST_REFERENCE_JOB


#include "t_memory_pool.hpp"
#include "dml/dmldefs.h"
#include "vector"
#include "memory"


namespace dml::test
{

    /**
     * @brief Performs special fields to compare tests results with
     */
    struct reference_job_t
    {

        /**
         * @brief Using type for a vector
         */
        using vector_type_t = dml::test::global_allocator::inner_buffer_t;

        /**
         * @brief Using type for a crc pointer
         */
        using crc_type_t    = std::unique_ptr<uint32_t>;



        /**
         * @brief First destination vector to compare with
         */
        vector_type_t     destination_first;

        /**
         * @brief Second destination vector to compare with
         */
        vector_type_t     destination_second;

        /**
         * @brief Pointer to crc checksum to compare with
         */
        crc_type_t        crc_checksum_ptr = nullptr;

        /**
         * @brief Count of successfully processed bytes
         */
        uint32_t          offset           = 0u;

        /**
         * @brief Result field for some operations
         */
        dml_meta_result_t result           = 0u;

        /**
         * @brief Expected status of an operation
         */
        dml_status_t      status           = DML_STATUS_OK;

    };

}



#endif // DML_TEST_REFERENCE_JOB
