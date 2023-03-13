/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml/dml.hpp>
#include <iostream>
#include <numeric>
#include <vector>

constexpr auto size = 1024u;  // 1 KB

template <typename path>
int execute_compare_pattern() {
    std::cout << "Starting dml::compare_pattern example..." << std::endl;
    std::cout << "Compare 1KB memory region with a 0x00ABCDEFABCDEF00 pattern..." << std::endl;

    // Prepare data
    auto pattern  = 0x00ABCDEFABCDEF00;
    auto src      = std::vector<std::uint8_t>(size);
    auto src_data = reinterpret_cast<std::uint64_t *>(src.data());
    std::fill_n(src_data, size / sizeof(std::uint64_t), pattern);

    // Run operation
    auto result = dml::execute<path>(dml::compare_pattern, pattern, dml::make_view(src));

    // Check result
    if (result.status == dml::status_code::ok) {
        std::cout << "Finished successfully." << std::endl;
    }
    else {
        std::cout << "Failure occurred." << std::endl;
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
        return execute_compare_pattern<dml::hardware>();
    }
    else if (path == "software_path") {
        std::cout << "Executing using dml::software path" << std::endl;
        return execute_compare_pattern<dml::software>();
    }
    else if (path == "auto_path") {
        std::cout << "Executing using dml::automatic path" << std::endl;
        return execute_compare_pattern<dml::automatic>();
    }
    else {
        std::cout << "Unrecognized value for parameter."
                  << "Use hardware_path, software_path or automatic_path." << std::endl;
        return 1;
    }
}
