/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains user-defined literals for tests
 * @date 4/17/2020
 *
 */
#include <cstdint>


namespace dml::test::literals
{

    /**
     * @brief Converts value to kilobytes
     */
    constexpr unsigned long long operator "" _KB(const unsigned long long value)
    {
        return static_cast<unsigned long long>(1000ull * value);
    }


    /**
     * @brief Converts value to kibibytes
     */
    constexpr unsigned long long operator "" _KiB(const unsigned long long value)
    {
        return static_cast<unsigned long long>(1024ull * value);
    }

}
