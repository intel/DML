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

using execution_path = dml::software;

int main()
{
    std::cout << "Starting dml::compare example...\n";
    std::cout << "Compare equal memory regions with size of 1KB...\n";

    // Prepare data
    auto src1 = std::vector<std::uint8_t>(size);
    std::iota(src1.begin(), src1.end(), 0u);
    auto src2 = src1;

    // Run operation
    auto result = dml::execute<execution_path>(dml::compare, dml::make_view(src1), dml::make_view(src2));

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
