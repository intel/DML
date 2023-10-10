/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include "system_error"

#include "t_job.hpp"


namespace dml::test
{
    /**
     * @brief Masks a value to get the closest multiple of 8
     */
    constexpr uint32_t OWN_MULTIPLY_8_MASK = ~7u;

    /**
     * @brief The size of an internal batch descriptor
     */
    constexpr auto OWN_BATCH_TASK_SIZE = 120u;


    dml::test::job_t::job_t(const dml_path_t path)
    {
        m_status = DML_STATUS_OK;

        auto get_job_size_status = dml_get_job_size(path, &m_job_size);

        if (DML_STATUS_OK != get_job_size_status)
        {
            EXPECT_EQ(DML_STATUS_OK, get_job_size_status);
            m_status = get_job_size_status;
        }
        else
        {
            m_job_ptr = std::make_unique<uint8_t []>(static_cast<size_t>(m_job_size));

            auto actual_job_ptr  = reinterpret_cast<dml_job_t *>(m_job_ptr.get());
            auto init_job_status = dml_init_job(path,
                                                actual_job_ptr);

            if (DML_STATUS_OK != init_job_status)
            {
                m_status = init_job_status;

                std::string message = "System error: Hardware initialization failed!\n \t Status code: "
                                      + std::to_string(init_job_status);

                // Need to fail the tests and to provide user with error message
                // Cannot use FAIL() operator inside the constructor this is workaround
                EXPECT_EQ(1, 0) << message;
            }
        }
    }


    dml::test::job_t::job_t(const job_t &other_job) noexcept
        :m_status(DML_STATUS_OK)
    {
        (*this) = other_job;
    }


    dml::test::job_t::job_t(job_t &&other_job) noexcept
        :m_status(DML_STATUS_OK)
    {
        (*this) = std::move(other_job);
    }


    dml::test::job_t::~job_t() noexcept(false)
    {
        auto actual_job_ptr        = reinterpret_cast<dml_job_t *>(m_job_ptr.get());
        const auto finalize_status = dml_finalize_job(actual_job_ptr);
        EXPECT_EQ(DML_STATUS_OK, finalize_status);
    }


    auto dml::test::job_t::run() noexcept -> dml_status_t
    {
        return dml_execute_job(reinterpret_cast<dml_job_t *>(m_job_ptr.get()), DML_WAIT_MODE_BUSY_POLL);
    }


    auto dml::test::job_t::operator->() noexcept -> dml_job_t *
    {
        return reinterpret_cast<dml_job_t *>(m_job_ptr.get());
    }


    auto dml::test::job_t::operator->() const noexcept -> const dml_job_t *
    {
        return reinterpret_cast<const dml_job_t *>(m_job_ptr.get());
    }


    auto dml::test::job_t::operator*() noexcept -> dml_job_t &
    {
        return (*reinterpret_cast<dml_job_t *>(m_job_ptr.get()));
    }


    auto dml::test::job_t::operator*() const noexcept -> const dml_job_t &
    {
        return (*reinterpret_cast<const dml_job_t *>(m_job_ptr.get()));
    }


    auto dml::test::job_t::operator=(const job_t &other_job) -> job_t &
    {
        m_job_size = other_job.m_job_size;

        m_job_ptr = std::make_unique<uint8_t []>(static_cast<size_t>(m_job_size));
        EXPECT_NE(nullptr, m_job_ptr);

        std::copy_n(other_job.m_job_ptr.get(), m_job_size, m_job_ptr.get());

        return (*this);
    }


    auto dml::test::job_t::operator=(job_t &&other_job) noexcept -> job_t &
    {
        m_job_size = other_job.m_job_size;

        std::swap(m_job_ptr, other_job.m_job_ptr);

        return (*this);
    }


    dml::test::job_t::operator bool() const noexcept
    {
        return (DML_STATUS_OK == m_status);
    }



    auto dml::test::job_t::operator==(
        const dml::test::reference_job_t &ref_job) const noexcept -> ::testing::AssertionResult
    {
        const auto actual_job_ptr = reinterpret_cast<dml_job_t *>(m_job_ptr.get());
        auto compare_result       = ::testing::AssertionSuccess();

        if ((DML_OP_BATCH == actual_job_ptr->operation) && (!ref_job.destination_first.empty()))
        {
            uint32_t index = 0;
            while (true)
            {
                dml_status_t task_status;

                auto status = dml_batch_get_status(actual_job_ptr, index, &task_status);
                if (status != DML_STATUS_OK)
                {
                    break;
                }

                if (task_status != DML_STATUS_OK)
                {
                    compare_result = ::testing::AssertionFailure();
                }
                index++;
            }
        }
        else
        {
            if (!ref_job.destination_first.empty())
            {
                for (auto i = 0u; i < actual_job_ptr->destination_length; ++i)
                {
                    if (actual_job_ptr->destination_first_ptr[i] != ref_job.destination_first[i])
                    {
                        std::cout << std::endl << "destination_first fields are not equal:" << std::endl;

                        std::cout << std::endl
                                << "library_job->destination_first_ptr[" << i << "]"
                                << " = " << static_cast<uint32_t>(actual_job_ptr->destination_first_ptr[i])
                                << std::endl
                                << "reference_job->destination_first_ptr[" << i << "]"
                                << " = " << static_cast<uint32_t>(ref_job.destination_first[i])
                                << std::endl << std::endl;

                        compare_result = ::testing::AssertionFailure();
                        break;
                    }
                }
            }

            if (!ref_job.destination_second.empty())
            {
                for (auto i = 0u; i < actual_job_ptr->destination_length; ++i)
                {
                    if (actual_job_ptr->destination_second_ptr[i] != ref_job.destination_second[i])
                    {
                        std::cout << std::endl << "destination_second fields are not equal:" << std::endl;

                        std::cout << std::endl
                                << "library_job->destination_second_ptr[" << i << "]"
                                << " = " << static_cast<uint32_t>(actual_job_ptr->destination_second_ptr[i])
                                << std::endl
                                << "reference_job->destination_second_ptr[" << i << "]"
                                << " = " << static_cast<uint32_t>(ref_job.destination_second[i])
                                << std::endl << std::endl;

                        compare_result = ::testing::AssertionFailure();
                        break;
                    }
                }
            }

            if (nullptr != ref_job.crc_checksum_ptr)
            {
                if ((*actual_job_ptr->crc_checksum_ptr) != (*ref_job.crc_checksum_ptr))
                {
                    std::cout << std::endl << "crc_checksum fields are not equal" << std::endl;

                    std::cout << std::endl
                            << "library_job->crc_checksum_ptr"
                            << " = " << static_cast<uint32_t>(*actual_job_ptr->crc_checksum_ptr)
                            << std::endl
                            << "reference_job->crc_checksum_ptr"
                            << " = " << static_cast<uint32_t>(*ref_job.crc_checksum_ptr)
                            << std::endl << std::endl;

                    compare_result = ::testing::AssertionFailure();
                }
            }

            switch (actual_job_ptr->operation)
            {
                case DML_OP_COMPARE_PATTERN:
                case DML_OP_DELTA_CREATE:
                    if ((actual_job_ptr->offset & OWN_MULTIPLY_8_MASK) != (ref_job.offset & OWN_MULTIPLY_8_MASK))
                    {
                        std::cout << std::endl << "offset is not in the pattern range" << std::endl;

                        std::cout << std::endl
                                << "library_job->offset"
                                << " = " << static_cast<uint32_t>(actual_job_ptr->offset & OWN_MULTIPLY_8_MASK)
                                << std::endl
                                << "reference_job->offset"
                                << " = " << static_cast<uint32_t>(ref_job.offset & OWN_MULTIPLY_8_MASK)
                                << std::endl << std::endl;

                        compare_result = ::testing::AssertionFailure();
                    }
                    break;

                default:
                    if (actual_job_ptr->offset != ref_job.offset)
                    {
                        std::cout << std::endl << "offset fields are not equal" << std::endl;

                        std::cout << std::endl
                                << "library_job->offset"
                                << " = " << static_cast<uint32_t>(actual_job_ptr->offset)
                                << std::endl
                                << "reference_job->offset"
                                << " = " << static_cast<uint32_t>(ref_job.offset)
                                << std::endl << std::endl;

                        compare_result = ::testing::AssertionFailure();
                    }
            }

            if (actual_job_ptr->result != ref_job.result)
            {
                std::cout << std::endl << "result fields are not equal" << std::endl;

                std::cout << std::endl
                        << "library_job->result"
                        << " = " << static_cast<uint32_t>(actual_job_ptr->result)
                        << std::endl
                        << "reference_job->result"
                        << " = " << static_cast<uint32_t>(ref_job.result)
                        << std::endl << std::endl;

                compare_result = ::testing::AssertionFailure();
            }
        }

        return compare_result;
    }
}
