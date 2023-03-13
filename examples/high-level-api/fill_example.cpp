/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml/dml.hpp>
#include <vector>
#include <algorithm>
#include <iostream>

constexpr auto size = 1024u;  // 1 KB

template <typename path>
int execute_fill() {
    std::cout << "Starting dml::fill example..." << std::endl;;
    std::cout << "Fill 1KB of data with a 0x00ABCDEFABCDEF00 pattern" << std::endl;;

    // Prepare data
    auto pattern = 0x00ABCDEFABCDEF00;
    auto dst = std::vector<std::uint8_t>(size, 0u);

    // Run operation
    auto result = dml::execute<path>(dml::fill, pattern, dml::make_view(dst));

    // Check result
    if (result.status == dml::status_code::ok) {
        std::cout << "Finished successfully." << std::endl;;
    }
    else {
        std::cout << "Failure occurred." << std::endl;;
        return -1;
    }

    auto ref = std::vector<std::uint8_t>(size);
    auto ref_data = reinterpret_cast<std::uint64_t*>(ref.data());
    std::fill_n(ref_data, size / sizeof(std::uint64_t), pattern);
    if (ref != dst)
    {
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
        return execute_fill<dml::hardware>();
    }
    else if (path == "software_path") {
        std::cout << "Executing using dml::software path" << std::endl;
        return execute_fill<dml::software>();
    }
    else if (path == "auto_path") {
        std::cout << "Executing using dml::automatic path" << std::endl;
        return execute_fill<dml::automatic>();
    }
    else {
        std::cout << "Unrecognized value for parameter."
                  << "Use hardware_path, software_path or automatic_path." << std::endl;
        return 1;
    }
}
