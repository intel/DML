/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @file
 * @brief Contains RNG for tests
 * @date 4/17/2020
 */
#ifndef DML_TEST_RANGE
#define DML_TEST_RANGE


#include <limits>


namespace dml::test
{

    /**
     * @brief Struct defines standart range
     */
    template<typename value_type>
    struct range_t
    {
        /**
         * @brief Wrapper for value type
         */
        using value_type_t = value_type;


        /**
         * @brief Minimum value in range
         */
        value_type_t min = 0;
        /**
         * @brief Maximum value in range
         */
        value_type_t max = std::numeric_limits<value_type_t>::max();
    };

}


#endif
