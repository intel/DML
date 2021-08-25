/*
 * Copyright 2021 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they
 * were provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit
 * this software or the related documents without Intel's prior written
 * permission.
 *
 * This software and the related documents are provided as is, with no
 * express or implied warranties, other than those that are expressly
 * stated in the License.
 *
 */

#include <dml/dml.hpp>
#include <iostream>
#include <numeric>
#include <vector>

constexpr auto size    = 1024u;  // 1 KB
constexpr auto padding = 0x1000; // dml::dualcast requirement, see documentation

using execution_path = dml::software;

int main()
{
    std::cout << "Starting dml::dualcast example...\n";
    std::cout << "Copy 1KB of data from source into two destinations...\n";

    // Prepare data
    auto src = std::vector<std::uint8_t>(size);
    std::iota(src.begin(), src.end(), 0u);
    auto dst_data = std::vector<std::uint8_t>(size + padding, 0u);
    auto dst1_ptr = dst_data.data();
    auto dst2_ptr = dst1_ptr + padding;

    // Run operation
    auto result = dml::execute<execution_path>(
        dml::dualcast, dml::make_view(src), dml::make_view(dst1_ptr, size), dml::make_view(dst2_ptr, size));

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

    if (src != std::vector(dst1_ptr, dst1_ptr + size) || src != std::vector(dst2_ptr, dst2_ptr + size))
    {
        std::cout << "But operation was done wrongly.\n";
        return -1;
    }

    return 0;
}
