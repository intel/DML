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
int execute_mem_move() {
    std::cout << "Starting dml::mem_move example..." << std::endl;
    std::cout << "Copy 1KB of data from source into destination..." << std::endl;

    // Prepare data
    auto src = std::vector<std::uint8_t>(size);
    std::iota(src.begin(), src.end(), 0u);
    auto dst = std::vector<std::uint8_t>(size, 0u);

    // Run operation
    auto result = dml::execute<path>(dml::mem_move, dml::make_view(src), dml::make_view(dst));

    // Check result
    if (result.status == dml::status_code::ok) {
        std::cout << "Finished successfully." << std::endl;
    }
    else {
        std::cout << "Failure occurred." << std::endl;
        return -1;
    }

    if (src != dst) {
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
        return execute_mem_move<dml::hardware>();
    }
    else if (path == "software_path") {
        std::cout << "Executing using dml::software path" << std::endl;
        return execute_mem_move<dml::software>();
    }
    else if (path == "auto_path") {
        std::cout << "Executing using dml::automatic path" << std::endl;
        return execute_mem_move<dml::automatic>();
    }
    else {
        std::cout << "Unrecognized value for parameter."
                  << "Use hardware_path, software_path or automatic_path." << std::endl;
        return 1;
    }
}
