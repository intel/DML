/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains common helper functions for testing
 * @details Function list:
 *      - @ref t_check_expected_result
 *
 * @date 3/5/2020
 *
 */

#ifndef DML_T_TEMPLATES_HPP__
#define DML_T_TEMPLATES_HPP__

/**
 * @brief Check function on expected result with sent function arguments
 *
 * @tparam callable         function to call
 * @tparam test_conditions  pair with return value and parameters list
 *
 * @param[in] function      function to call
 * @param[in] conditions    pair with return value and parameters list
 *
 */
template<class callable, class test_conditions>
static inline auto t_check_expected_result(const callable function,
                                           const test_conditions &conditions) -> void
{
    for(auto condition: conditions)
    {
        const auto expected_status = condition.first;
        const auto kernel_status   = std::apply(function, condition.second);

        EXPECT_EQ(expected_status, kernel_status);
    }
}

#endif //DML_T_TEMPLATES_HPP__
