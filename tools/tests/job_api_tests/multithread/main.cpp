/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/


#define _HAS_STD_BYTE 0

#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>

#include "t_command_line_parser.hpp"
#include "t_random_generator.hpp"
#include "gtest/gtest.h"
#include "t_common.hpp"
#include "cctype"


#define OWN_NTHREADS_ARG ("--nthreads")
#define OWN_PATH_ARG     ("--path")
#define OWN_HELP_ARG     ("--help")


auto custom_mem_move(void *source_ptr, void *destination_ptr, uint32_t length) -> bool
{
    dml_job_t *job_ptr;
    uint32_t job_size;

    auto get_job_size_status = dml_get_job_size(dml::test::variables_t::path, &job_size);
    if (DML_STATUS_OK != get_job_size_status)
    {
        return false;
    }

    job_ptr = (dml_job_t *) new uint8_t[job_size];

    auto init_job_status = dml_init_job(dml::test::variables_t::path, job_ptr);
    if (DML_STATUS_OK != init_job_status)
    {
        delete[] ((uint8_t *) job_ptr);
        return false;
    }

    job_ptr->source_first_ptr      = (uint8_t *) source_ptr;
    job_ptr->source_length         = length;
    job_ptr->destination_first_ptr = (uint8_t *) destination_ptr;
    job_ptr->destination_length    = length;
    job_ptr->operation             = DML_OP_MEM_MOVE;

    auto execute_status = dml_execute_job(job_ptr, DML_WAIT_MODE_BUSY_POLL);
    if (DML_STATUS_OK != execute_status)
    {
        return false;
    }
    else
    {
        auto finalize_job_status = dml_finalize_job(job_ptr);
        if (DML_STATUS_OK != finalize_job_status)
        {
            return false;
        }
    }

    delete[] ((uint8_t *) job_ptr);
    return true;
}


auto run(uint32_t threads_count) -> uint32_t
{
    auto random_length = dml::test::random_t<uint32_t>(dml::test::variables_t::seed);
    auto random_value  = dml::test::random_t<uint8_t>(dml::test::variables_t::seed);
    auto sources       = std::vector<std::vector<uint8_t>>(threads_count);
    auto destinations  = std::vector<std::vector<uint8_t>>(threads_count);
    auto threads       = std::vector<std::thread>(threads_count);
    auto length        = random_length.get_next();
    auto test_passed   = std::atomic<bool>();

    std::cout << "length: " << length << std::endl;

    // Allocation
    std::cout << "Allocation" << std::endl;
    for (auto i = 0u; i < threads_count; ++i)
    {
        threads[i] = std::thread([&](uint32_t i)
        {
            sources[i].resize(length, 0u);
            destinations[i].resize(length, 0u);
        }, i);
    }

    for (auto &thread : threads)
    {
        thread.join();
    }


    // Initialization
    std::cout << "Initialization" << std::endl;
    for (auto i = 0u; i < threads_count; ++i)
    {
        threads[i] = std::thread([&](uint32_t i)
        {
            std::fill(sources[i].begin(), sources[i].end(), 0u);
            std::generate(destinations[i].begin(), destinations[i].end(), random_value);
        }, i);
    }

    for (auto &thread : threads)
    {
        thread.join();
    }


    // Memory move
    std::cout << "Memory move" << std::endl;
    test_passed = true;
    for (auto i = 0u; i < threads_count; ++i)
    {
        threads[i] = std::thread([&](uint32_t i)
        {
            if (!custom_mem_move(sources[i].data(), destinations[i].data(), length))
            {
                test_passed = false;
            }
        }, i);
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    if (!test_passed)
    {
        std::cout << "Library initialization failed" << std::endl;
        return 1u;
    }


    // Compare
    std::cout << "Compare" << std::endl;
    for (auto i = 0u; i < threads_count; ++i)
    {
        threads[i] = std::thread([&](uint32_t i)
        {
            for (auto j = 0u; j < length; ++j)
            {
                if (sources[i][j] != destinations[i][j])
                {
                    std::cerr << "source[" << j << "]" << " != " << "destination[" << j << "]" << " in thread " << i << std::endl;
                    std::cerr << "source[" << j << "]: " << sources[i][j] << std::endl;
                    std::cerr << "destination[" << j << "]: " << destinations[i][j] << std::endl;
                    test_passed = false;
                }
            }
        }, i);
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    if (test_passed)
    {
        std::cout << "Test passed, no errors found" << std::endl;
    }
    else
    {
        std::cout << "Test failed" << std::endl;
        return 1u;
    }


    return 0u;
}


auto main(int argc, char **argv) -> int
{
    using cmd_parser = dml::test::command_line_arguments_parser_t;

    auto threads_count = 1u;
    cmd_parser::parse(argc, argv);

    dml::test::global_allocator::reserve(1000u);
    dml::test::variables_t::seed = static_cast<uint32_t>(std::chrono::high_resolution_clock::now()
                                                            .time_since_epoch()
                                                            .count());

    if (cmd_parser::exists(OWN_HELP_ARG))
    {
        std::cout << std::endl << "DML Test arguments:" << std::endl;

        std::cout << "  "     << OWN_NTHREADS_ARG                 <<  std::endl
                  << "      " << "Number of threads for testing"  << std::endl;
        std::cout << "  "     << OWN_PATH_ARG << "=(auto/sw/hw)"  << std::endl
                  << "      " << "Sets execution path"            << std::endl;

        exit(0);
    }
    if (cmd_parser::exists(OWN_PATH_ARG))
    {
        const auto path = cmd_parser::get_string(OWN_PATH_ARG);

        if ("hw" == path)
        {
            dml::test::variables_t::path = DML_PATH_HW;
        }
        else if ("sw" == path)
        {
            dml::test::variables_t::path = DML_PATH_SW;
        }
        else if ("auto" == path)
        {
            dml::test::variables_t::path = DML_PATH_AUTO;
        }
        else
        {
            std::cout << "  Incorrect parameters"  << std::endl;
            std::cout << "  Type " << OWN_HELP_ARG << " for help" << std::endl;

            exit(0);
        }
    }
    if (cmd_parser::exists(OWN_NTHREADS_ARG))
    {
        threads_count = cmd_parser::get_value<uint32_t>(OWN_NTHREADS_ARG);

        if (0u == threads_count)
        {
            std::cout << "Incorrect nthread value" << std::endl;
            exit(0);
        }
    }

    std::cout << "threads count: " << threads_count << std::endl;

    return run(threads_count);
}
