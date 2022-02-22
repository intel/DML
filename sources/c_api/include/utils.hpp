/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_UTILS_HPP
#define DML_UTILS_HPP

#include <cstddef>

namespace dml
{
    template <typename element_t>
    constexpr auto* align(element_t* pointer, std::size_t alignment = 64) noexcept
    {
        auto offset = static_cast<std::size_t>(reinterpret_cast<uintptr_t>(pointer) & (alignment - 1));
        if (offset != 0)
        {
            offset = alignment - offset;
        }
        return reinterpret_cast<element_t*>(reinterpret_cast<uint8_t*>(pointer) + offset);
    }
}  // namespace dml

#endif  // DML_UTILS_HPP
