/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_ML_OWN_UTILS_HPP
#define DML_ML_OWN_UTILS_HPP

#include <dml/detail/common/status.hpp>
#include <tuple>

#define RETURN_STATUS_IF(expr, status) \
    if ((expr))                        \
    return (status)

namespace dml::core
{
    template <typename... args_t>
    bool any_equal_zero(args_t... args) noexcept
    {
        const auto is_zero = [](auto v)
        {
            return v == 0;
        };
        return (is_zero(args) || ...);
    }

    bool overlaps(address_t lhs, address_t rhs, transfer_size_t size) noexcept
    {
        return ((lhs <= rhs) && ((lhs + size) > rhs)) || ((rhs <= lhs) && ((rhs + size) > lhs));
    }

    bool overlaps(address_t lhs, transfer_size_t lhs_size, address_t rhs, transfer_size_t rhs_size) noexcept
    {
        return ((lhs <= rhs) && ((lhs + lhs_size) > rhs)) || ((rhs <= lhs) && ((rhs + rhs_size) > lhs));
    }

    bool adjacent(address_t lhs, transfer_size_t lhs_size, address_t rhs) noexcept
    {
        // If end of lhs is the same as begin of rhs
        return (lhs + lhs_size) == rhs;
    }

    template <std::size_t alignment, typename... args_t>
    bool any_misaligned(args_t... args) noexcept
    {
        const auto is_misaligned = [](auto v)
        {
            return v % alignment != 0;
        };
        return (is_misaligned(args) || ...);
    }
}  // namespace dml::core

#endif  // DML_ML_OWN_UTILS_HPP
