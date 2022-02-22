/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

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
