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
    std::cout << "Starting dml::cache_flush example...\n";
    std::cout << "Flush 1KB of data from the cache to the memory...\n";

    // Prepare data
    auto dst = std::vector<std::uint8_t>(size, 0u);

    // Run operation
    auto result = dml::execute<execution_path>(dml::cache_flush, dml::make_view(dst));

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

    return 0;
}
