/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_CORE_UTILS_HPP
#define DML_CORE_UTILS_HPP

#include <core/view.hpp>

namespace dml::core
{
    // TODO: Should not be passed by ref
    template <typename type>
    [[nodiscard]] inline auto& get_completion_record(any_descriptor<type>& view) noexcept
    {
        return *reinterpret_cast<completion_record*>(view.completion_record_address());
    }

    [[nodiscard]] inline auto& get_completion_record(const descriptor& dsc) noexcept
    {
        // TODO: One-liner after the function above fixed
        auto view = any_descriptor(dsc);
        return get_completion_record(view);
    }
}  // namespace dml::core

#endif  //DML_CORE_UTILS_HPP
