/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml/dml.hpp>
#include <iostream>
#include <numeric>
#include <vector>

constexpr auto size    = 1024u;  // 1 KB
constexpr auto padding = 0x1000; // dml::dualcast requirement, see documentation

template <typename path>
int execute_dualcast() {
    std::cout << "Starting dml::dualcast example..." << std::endl;
    std::cout << "Copy 1KB of data from source into two destinations..." << std::endl;

    // Prepare data
    auto src = std::vector<std::uint8_t>(size);
    std::iota(src.begin(), src.end(), 0u);
    auto dst_data = std::vector<std::uint8_t>(size + padding, 0u);
    auto dst1_ptr = dst_data.data();
    auto dst2_ptr = dst1_ptr + padding;

    // Run operation
    auto result = dml::execute<path>(
        dml::dualcast, dml::make_view(src), dml::make_view(dst1_ptr, size), dml::make_view(dst2_ptr, size));

    // Check result
    if (result.status == dml::status_code::ok) {
        std::cout << "Finished successfully." << std::endl;
    }
    else {
        std::cout << "Failure occurred." << std::endl;
        return -1;
    }

    if (src != std::vector(dst1_ptr, dst1_ptr + size) || src != std::vector(dst2_ptr, dst2_ptr + size)) {
        std::cout << "Operation result is incorrect." << std::endl;
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
        return execute_dualcast<dml::hardware>();
    }
    else if (path == "software_path") {
        std::cout << "Executing using dml::software path" << std::endl;
        return execute_dualcast<dml::software>();
    }
    else if (path == "auto_path") {
        std::cout << "Executing using dml::automatic path" << std::endl;
        return execute_dualcast<dml::automatic>();
    }
    else {
        std::cout << "Unrecognized value for parameter."
                  << "Use hardware_path, software_path or automatic_path." << std::endl;
        return 1;
    }
}