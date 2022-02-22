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

using execution_path = dml::software;

int main()
{
    std::cout << "Starting dml::create_delta and dml::apply_delta example...\n";
    std::cout << "Create delta record of two 1KB memory_regions...\n";

    // Prepare data
    auto src1 = std::vector<std::uint8_t>(size);
    std::iota(src1.begin(), src1.end(), 0u);
    auto src2 = std::vector<std::uint8_t>(size);
    std::iota(src2.begin(), src2.end(), 1u);
    auto delta = std::vector<std::uint8_t>(delta_record_size, 0u);

    // Run operation
    auto create_result = dml::execute<execution_path>(
        dml::create_delta, dml::make_view(src1), dml::make_view(src2), dml::make_view(delta));

    // Check result
    if (create_result.status == dml::status_code::ok)
    {
        std::cout << "Done. Next step...\n";
    }
    else
    {
        std::cout << "Failure occurred!\n";
        return -1;
    }

    std::cout << "Apply delta record to the first 1KB memory_region...\n";

    auto apply_result = dml::execute<execution_path>(
        dml::apply_delta, dml::make_view(delta), dml::make_view(src1), create_result);

    // Check result
    if (apply_result.status == dml::status_code::ok)
    {
        std::cout << "Finished successfully!\n";
    }
    else
    {
        std::cout << "Failure occurred!\n";
        return -1;
    }

    if (src1 != src2)
    {
        std::cout << "But operation was done wrongly.\n";
        return -1;
    }

    return 0;
}
