 .. ***************************************************************************
 .. * Copyright (C) 2021 Intel Corporation
 .. *
 .. * SPDX-License-Identifier: MIT
 .. ***************************************************************************/

Quick start
###########


The quick start sample below shows how to compute CRC on the data
using Intel® Data Mover Library (Intel® DML). 
See `CRC Generation <../api_docs/high_level_api.html#crc-generation>`__ 
operation for more information.


.. code-block:: cpp

    #include <dml/dml.hpp>
    #include <iostream>
    #include <string>
    #include <vector>

    constexpr auto string  = "Calculate CRC value for this string...\n";

    using execution_path = dml::software;

    int main()
    {
        std::cout << "Starting dml::crc example...\n";
        std::cout << string;

        // Prepare data
        auto crc_seed = std::uint32_t(0u);
        auto src = std::basic_string<std::uint8_t>(reinterpret_cast<const std::uint8_t *>(string));

        // Run operation
        auto result = dml::execute<execution_path>(dml::crc, dml::make_view(src), crc_seed);

        // Check result
        if (result.status == dml::status_code::ok)
        {
            std::cout << "Finished successfully! Calculated CRC is: 0x" << std::hex << result.crc_value << "\n";
        }
        else
        {
            std::cout << "Failure occurred!\n";
            return -1;
        }

        return 0;
    }
