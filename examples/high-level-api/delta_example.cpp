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
constexpr auto delta_record_size = size * 10u / 8u;

template <typename path>
int execute_delta() {
    std::cout << "Starting dml::create_delta and dml::apply_delta example..." << std::endl;
    std::cout << "Create delta record of two 1KB memory_regions..." << std::endl;

    // Prepare data
    auto src1 = std::vector<std::uint8_t>(size);
    std::iota(src1.begin(), src1.end(), 0u);
    auto src2 = std::vector<std::uint8_t>(size);
    std::iota(src2.begin(), src2.end(), 1u);
    auto delta = std::vector<std::uint8_t>(delta_record_size, 0u);

    // Run operation
    auto create_result = dml::execute<path>(
        dml::create_delta, dml::make_view(src1), dml::make_view(src2), dml::make_view(delta));

    // Check result
    if (create_result.status == dml::status_code::ok) {
        std::cout << "Done. Next step..." << std::endl;
    }
    else {
        std::cout << "Failure occurred!" << std::endl;
        return -1;
    }

    std::cout << "Apply delta record to the first 1KB memory_region..." << std::endl;

    auto apply_result = dml::execute<path>(
        dml::apply_delta, dml::make_view(delta), dml::make_view(src1), create_result);

    // Check result
    if (apply_result.status == dml::status_code::ok) {
        std::cout << "Finished successfully!" << std::endl;
    }
    else {
        std::cout << "Failure occurred!" << std::endl;
        return -1;
    }

    if (src1 != src2) {
        std::cout << "But operation was done wrongly." << std::endl;
        return -1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cout << "Missing the execution path as the first parameter."
                  << "Use hardware_path, software_path or automatic_path." << std::endl;
        return 1;
    }

    std::string path = argv[1];
    if (path == "hardware_path") {
        std::cout << "Executing using dml::hardware path" << std::endl;
        return execute_delta<dml::hardware>();
    }
    else if (path == "software_path") {
        std::cout << "Executing using dml::software path" << std::endl;
        return execute_delta<dml::software>();
    }
    else if (path == "auto_path") {
        std::cout << "Executing using dml::automatic path" << std::endl;
        return execute_delta<dml::automatic>();
    }
    else {
        std::cout << "Unrecognized value for parameter."
                  << "Use hardware_path, software_path or automatic_path." << std::endl;
        return 1;
    }
}
