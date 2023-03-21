/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml/dml.hpp>
#include <numeric>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

constexpr auto size = 1024u;  // 1 KB
constexpr auto delta_record_size = size * 10u / 8u;

template <typename path>
int execute_batch() {
    
    std::cout << "Starting dml::batch example..." << std::endl;

    // Prepare data
    auto pattern = 0x00ABCDEFABCDEF00;
    auto src = std::vector<std::uint8_t>(size);

    auto dst1 = std::vector<std::uint8_t>(size, 0u);
    auto dst2 = std::vector<std::uint8_t>(size, 0u);
    auto dst3 = std::vector<std::uint8_t>(size);

    auto delta = std::vector<std::uint8_t>(delta_record_size, 0u);
    constexpr auto count  = 7u;

    auto sequence = dml::sequence(count, std::allocator<dml::byte_t>());

    std::cout << "Fill 1KB of data with a 0x00ABCDEFABCDEF00 pattern" << std::endl;
    sequence.add(dml::fill, pattern, dml::make_view(src));

    std::cout << "Add No-op to batch to ensure fill completes before copying..." << std::endl;
    sequence.add(dml::nop);
    
    std::cout << "Copy 1KB of data from source into destination..." << std::endl;
    sequence.add(dml::mem_move, dml::make_view(src), dml::make_view(dst1));
    
    std::cout << "Copy 1KB of data from source1 into two destinations..." << std::endl;
    sequence.add(dml::dualcast, dml::make_view(src), dml::make_view(dst2), dml::make_view(dst3));

    std::cout << "Add No-op to batch to ensure all copies complete..." << std::endl;
    sequence.add(dml::nop);

    std::cout << "Compare 1KB memory region with a 0x00ABCDEFABCDEF00 pattern..." << std::endl;
    sequence.add(dml::compare_pattern, pattern, dml::make_view(dst1));

    std::cout << "Compare equal memory regions with size of 1KB..." << std::endl;
    sequence.add(dml::compare, dml::make_view(dst2), dml::make_view(dst3));

    // Run operation
    auto result = dml::execute<path>(dml::batch, sequence);

    // Check result
    if (result.status == dml::status_code::ok) {
        std::cout << "Finished successfully." << std::endl;;
    }
    else {
        std::cout << "Failure occurred." << std::endl;;
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
        return execute_batch<dml::hardware>();
    }
    else if (path == "software_path") {
        std::cout << "Executing using dml::software path" << std::endl;
        return execute_batch<dml::software>();
    }
    else if (path == "auto_path") {
        std::cout << "Executing using dml::automatic path" << std::endl;
        return execute_batch<dml::automatic>();
    }
    else {
        std::cout << "Unrecognized value for parameter."
                  << "Use hardware_path, software_path or automatic_path." << std::endl;
        return 1;
    }
}
