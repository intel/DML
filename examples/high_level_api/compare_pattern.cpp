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

using execution_path = dml::software;

int main()
{
    std::cout << "Starting dml::compare_pattern example...\n";
    std::cout << "Compare 1KB memory region with a 0x00ABCDEFABCDEF00 pattern...\n";

    // Prepare data
    auto pattern  = 0x00ABCDEFABCDEF00;
    auto src      = std::vector<std::uint8_t>(size);
    auto src_data = reinterpret_cast<std::uint64_t *>(src.data());
    std::fill_n(src_data, size / sizeof(std::uint64_t), pattern);

    // Run operation
    auto result = dml::execute<execution_path>(dml::compare_pattern, pattern, dml::make_view(src));

    // Check result
    if (result.status == dml::status_code::ok)
    {
        std::cout << "Finished successfully!\n";
    }
    else
    {
        std::cout << "Failure occurred!\n";
        return -1;
    }

    if (result.result != dml::comparison_result::equal)
    {
        std::cout << "But operation was done wrongly.\n";
        return -1;
    }

    return 0;
}
