/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml/dml.hpp>
#include <numeric>
#include <vector>
#include <iostream>

constexpr auto size = 1024u;  // 1 KB

template <typename path>
int execute_compare(){
    std::cout << "Starting dml::compare example..." << std::endl;
    std::cout << "Compare equal memory regions with size of 1KB..." << std::endl;

    // Prepare data
    auto src1 = std::vector<std::uint8_t>(size);
    std::iota(src1.begin(), src1.end(), 0u);
    auto src2 = src1;

    // Run operation
    auto result = dml::execute<path>(dml::compare, dml::make_view(src1), dml::make_view(src2));

    // Check result
    if (result.status == dml::status_code::ok) {
        std::cout << "Finished successfully" << std::endl;
    }
    else {
        std::cout << "Failure occurred" << std::endl;
        return -1;
    }

    if (result.result != dml::comparison_result::equal) {
        std::cout << "Operation result is incorrect" << std::endl;
        return -1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cout << "Missing the execution path as the first parameter."
                  <<  "Use hardware_path, software_path or automatic_path." << std::endl;
        return 1;
    }

    std::string path = argv[1];
    if (path == "hardware_path") {
        std::cout << "Executing using dml::hardware path" << std::endl;
        return execute_compare<dml::hardware>();
    }
    else if (path == "software_path") {
        std::cout << "Executing using dml::software path" << std::endl;
        return execute_compare<dml::software>();
    }
    else if (path == "auto_path") {
        std::cout << "Executing using dml::automatic path" << std::endl;
        return execute_compare<dml::automatic>();
    }
    else {
        std::cout << "Unrecognized value for parameter."
                  << "Use hardware_path, software_path or automatic_path." << std::endl;
        return 1;
    }
}
