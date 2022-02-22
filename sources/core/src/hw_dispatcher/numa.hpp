/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_MIDDLE_LAYER_DISPATCHER_NUMA_HPP_
#define DML_MIDDLE_LAYER_DISPATCHER_NUMA_HPP_

#include <cstdint>

namespace dml::core::util
{
    [[nodiscard]] uint32_t get_numa_id() noexcept;
}

#endif  //DML_MIDDLE_LAYER_DISPATCHER_NUMA_HPP_
