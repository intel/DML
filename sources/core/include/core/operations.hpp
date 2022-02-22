/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_CORE_OPERATIONS_HPP
#define DML_CORE_OPERATIONS_HPP

#include <core/types.hpp>
#include <type_traits>

namespace dml::core
{
    enum class operation : operation_t
    {
        nop             = 0x00,
        batch           = 0x01,
        drain           = 0x02,
        mem_move        = 0x03,
        fill            = 0x04,
        compare         = 0x05,
        compare_pattern = 0x06,
        create_delta    = 0x07,
        apply_delta     = 0x08,
        dualcast        = 0x09,
        crc             = 0x10,
        copy_crc        = 0x11,
        dif_check       = 0x12,
        dif_insert      = 0x13,
        dif_strip       = 0x14,
        dif_update      = 0x15,
        cache_flush     = 0x20
    };
}  // namespace dml::core

#endif  //DML_CORE_OPERATIONS_HPP
