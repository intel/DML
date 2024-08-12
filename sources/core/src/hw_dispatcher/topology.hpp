/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_MIDDLE_LAYER_DISPATCHER_TOPOLOGY_HPP_
#define DML_MIDDLE_LAYER_DISPATCHER_TOPOLOGY_HPP_

#include <cstdint>

namespace dml::core::util
{
    [[nodiscard]] uint32_t get_numa_id() noexcept;

    [[nodiscard]] uint64_t get_socket_id() noexcept;

    [[nodiscard]] uint64_t get_socket_id(int numa_node) noexcept;
}

#endif  //DML_MIDDLE_LAYER_DISPATCHER_TOPOLOGY_HPP_
