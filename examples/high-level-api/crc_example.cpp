/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml/dml.hpp>
#include <iostream>
#include <string>
#include <vector>

constexpr auto string  = "Calculate CRC value for this string...\n";

template <typename path>
int execute_crc() {
    std::cout << "Starting dml::crc example...\n";
    std::cout << string;

    // Prepare data
    auto crc_seed = std::uint32_t(0u);
    auto src      = std::basic_string<std::uint8_t>(reinterpret_cast<const std::uint8_t *>(string));

    // Run operation
    auto result = dml::execute<path>(dml::crc, dml::make_view(src), crc_seed);

    // Check result
    if (result.status == dml::status_code::ok) {
        std::cout << "Finished successfully. Calculated CRC is: 0x" << std::hex << result.crc_value << std::endl;
    }
    else {
        std::cout << "Failure occurred." << std::endl;
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
        return execute_crc<dml::hardware>();
    }
    else if (path == "software_path") {
        std::cout << "Executing using dml::software path" << std::endl;
        return execute_crc<dml::software>();
    }
    else if (path == "auto_path") {
        std::cout << "Executing using dml::automatic path" << std::endl;
        return execute_crc<dml::automatic>();
    }
    else {
        std::cout << "Unrecognized value for parameter."
                  << "Use hardware_path, software_path or automatic_path." << std::endl;
        return 1;
    }
}
