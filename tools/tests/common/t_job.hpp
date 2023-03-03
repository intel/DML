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
#ifndef DML_TEST_JOB
#define DML_TEST_JOB


#include "t_reference_job.hpp"
#include "gtest/gtest.h"
#include "dml/dmldefs.h"
#include <cstring>
#include "dml/dml.h"


namespace dml::test
{

    /**
     * @brief Class that wraps @ref dml_job_t
     */
    class job_t
    {
    public:
        /**
         * @brief Default constructor that performs @ref dml_job_t initialization
         */
        job_t(const dml_path_t path = DML_PATH_SW);

        /**
         * @brief Copy constructor
         */
        job_t(const job_t &other_job) noexcept;

        /**
         * @brief Move constructor
         */
        job_t(job_t &&other_job) noexcept;

        /**
         * @brief Destructs the job
         */
        ~job_t() noexcept;

        /**
         * @brief Calls @ref dml_execute_job
         */
        auto run() noexcept -> dml_status_t;

        /**
         * @brief Gives access to members in @ref dml_job_t
         */
        [[nodiscard]]
        auto operator->() noexcept -> dml_job_t *;

        /**
         * @brief Gives access to members in @ref dml_job_t
         */
        [[nodiscard]]
        auto operator->() const noexcept -> const dml_job_t *;

        /**
         * @brief Gives access to @ref dml_job_t
         */
        [[nodiscard]]
        auto operator*() noexcept -> dml_job_t &;

        /**
         * @brief Gives access to @ref dml_job_t
         */
        [[nodiscard]]
        auto operator*() const noexcept -> const dml_job_t &;

        /**
         * @brief Assign operator
         */
        auto operator=(const job_t &other_job) noexcept -> job_t &;

        /**
         * @brief Move operator
         */
        auto operator=(job_t &&other_job) noexcept -> job_t &;

        /**
         * @brief Returns status of the @ref dml_job_t initialization
         */
        operator bool() const noexcept;

        /**
         * @brief Compares result with reference
         */
        auto operator==(const dml::test::reference_job_t &ref_job) const noexcept -> ::testing::AssertionResult;

    private:
        /**
         * @brief Member pointer to @ref dml_job_t
         */
        std::unique_ptr<uint8_t []> m_job_ptr;

        /**
         * @brief Number of bytes for allocation
         */
        uint32_t m_job_size = 0u;

        /**
         * @brief Status of the initialization
         */
        dml_status_t m_status;
    };

}


#endif // DML_TEST_JOB
