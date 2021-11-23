/*
 * Copyright 2021 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they
 * were provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit
 * this software or the related documents without Intel's prior written
 * permission.
 *
 * This software and the related documents are provided as is, with no
 * express or implied warranties, other than those that are expressly
 * stated in the License.
 *
 */

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

#endif // DML_UTILS_HPP
