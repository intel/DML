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
int execute_cache_flush(){
    std::cout << "Starting dml::cache_flush example..." << std::endl;
    std::cout << "Flush 1KB of data from the cache to the memory..." << std::endl;

    // Prepare data
    auto dst = std::vector<std::uint8_t>(size, 0u);

    // Run operation
    auto result = dml::execute<path>(dml::cache_flush, dml::make_view(dst));

    // Check result
    if (result.status == dml::status_code::ok) {
        std::cout << "Finished successfully" << std::endl;
        return 0;
    }
    else {
        std::cout << "Failure occurred" << std::endl;
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
        return execute_cache_flush<dml::hardware>();
    }
    else if (path == "software_path") {
        std::cout << "Executing using dml::software path" << std::endl;
        return execute_cache_flush<dml::software>();
    }
    else if (path == "auto_path") {
        std::cout << "Executing using dml::automatic path" << std::endl;
        return execute_cache_flush<dml::automatic>();
    }
    else {
        std::cout << "Unrecognized value for parameter."
                  << "Use hardware_path, software_path or automatic_path." << std::endl;
        return 1;
    }
}
