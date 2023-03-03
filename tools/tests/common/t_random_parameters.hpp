/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief
 * @date 2/11/2020
 *
 */

#ifndef DML_T_RANDOM_PARAMS_HPP__
#define DML_T_RANDOM_PARAMS_HPP__

#include "t_common.hpp"

namespace test_system
{
    /**
     * @brief Representation of high and low limits.
     */
    typedef struct
    {
        uint32_t begin; /**< Low limit */
        uint32_t end;   /**< High limit */
    } range_t;

    /**
     * @brief Ask test system for seed
     *
     * @return seed value
     */
    uint32_t get_seed();

    /**
     * @brief Ask test system for test id
     *
     * @return test id specified by user
     */
    int32_t get_test_id();

}

#endif //DML_T_RANDOM_PARAMS_HPP_
