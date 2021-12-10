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

#ifndef DML_COMMON_UTILS_ENUM_HPP
#define DML_COMMON_UTILS_ENUM_HPP

#include <type_traits>

namespace dml::detail
{
    template <typename enum_t>
    [[nodiscard]] constexpr auto to_underlying(const enum_t enum_value) noexcept
    {
        return static_cast<std::underlying_type_t<enum_t>>(enum_value);
    }

    template <typename enum_t>
    [[nodiscard]] constexpr bool intersects(const std::underlying_type_t<enum_t> lhs, const enum_t rhs) noexcept
    {
        return (lhs & to_underlying(rhs)) == to_underlying(rhs);
    }
}  // namespace dml::detail

#endif  //DML_COMMON_UTILS_ENUM_HPP
