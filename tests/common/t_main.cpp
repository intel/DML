/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/


#define _HAS_STD_BYTE 0

#include <chrono>

#include "t_command_line_parser.hpp"
#include "t_random_generator.hpp"
#include "gtest/gtest.h"
#include "t_common.hpp"
#include "cctype"


#define OWN_FUZZING_ARG ("--enable_fuzzing")
#define OWN_HELP_ARG    ("--help")
#define OWN_SEED_ARG    ("--seed")
#define OWN_TEST_ID_ARG ("--testid")
#define OWN_PATH_ARG    ("--path")


auto main(int argc, char **argv) -> int
{
    using cmd_parser = dml::test::command_line_arguments_parser_t;

    ::testing::InitGoogleTest(&argc, argv);
    cmd_parser::parse(argc, argv);

    dml::test::global_allocator::reserve(1000u);
    dml::test::variables_t::seed = static_cast<uint32_t>(std::chrono::high_resolution_clock::now()
                                                            .time_since_epoch()
                                                            .count());
    auto fuzzing_enabled = false;

    // Switch command line arguments
    if (cmd_parser::exists(OWN_HELP_ARG))
    {
        std::cout << std::endl << "DML Test arguments:" << std::endl;

        std::cout << "  "     << OWN_FUZZING_ARG                  <<  std::endl 
                  << "      " << "Testing before the first error" << std::endl;
        std::cout << "  "     << OWN_SEED_ARG << "=(value)"       << std::endl
                  << "      " << "Sets user-defined seed"         << std::endl;
        std::cout << "  "     << OWN_PATH_ARG << "=(auto/sw/hw)"  << std::endl
                  << "      " << "Sets execution path"            << std::endl;
        std::cout << "  "     << OWN_TEST_ID_ARG << "=(value)"  << std::endl
                  << "      " << "Sets specified test case to be run (value = -1 is reserved)" << std::endl;

        exit(0);
    }
    if (cmd_parser::exists(OWN_FUZZING_ARG))
    {
        fuzzing_enabled = true;

        cmd_parser::remove(OWN_FUZZING_ARG);
    }
    if (cmd_parser::exists(OWN_SEED_ARG))
    {
        dml::test::variables_t::seed = cmd_parser::get_value<uint32_t>(OWN_SEED_ARG);
    }
    if (cmd_parser::exists(OWN_TEST_ID_ARG))
    {
        dml::test::variables_t::test_case_id = cmd_parser::get_value<int32_t>(OWN_TEST_ID_ARG);
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
    if (!cmd_parser::empty())
    {
        std::cout << "  Incorrect parameters"  << std::endl;
        std::cout << "  Type " << OWN_HELP_ARG << " for help" << std::endl;

        exit(0);
    }

    const auto build_information = dml_get_library_version();

    std::cout << std::endl;
    std::cout << "Library Name:         " << build_information->name                << std::endl;
    std::cout << "Version:              " << build_information->version             << std::endl;
    std::cout << "Minimal CPU ISA:      " << build_information->minimal_cpu_isa     << std::endl;
    std::cout << "Build Date:           " << build_information->date                << std::endl;
    std::cout << "Supported HW Version: " << build_information->hw_minimal_version  << std::endl;
    std::cout << "Commit Hash:          " << std::hex <<  build_information->commit << std::endl;
    std::cout << std::endl;

    // Fix for hexadecimal output
    std::cout << std::dec;

    // Switch test types
    if (fuzzing_enabled)
    {
        auto seed_generator = dml::test::random_t<uint32_t>(dml::test::variables_t::seed);

        do
        {
            dml::test::variables_t::seed = seed_generator.get_next();
            DML_TEST_LOG("seed: " << dml::test::variables_t::seed);

        } while(!RUN_ALL_TESTS());

        // 1 - is error key for google-test.
        // No special macro exists.
        return 1;
    }
    else
    {
        return RUN_ALL_TESTS();
    }
}
