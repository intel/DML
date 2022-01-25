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
