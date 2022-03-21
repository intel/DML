/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contain base wrappers to get random test parameters:
 *      - @ref get_seed();
 *
 * @date 2/11/2020
 *
 */

#include "t_random_parameters.hpp"
#include "t_common.hpp"


uint32_t test_system::get_seed()
{
    DML_TEST_LOG("seed: " << dml::test::variables_t::seed);
    return dml::test::variables_t::seed;
}

int32_t test_system::get_test_id() {
    return dml::test::variables_t::test_case_id;
}
