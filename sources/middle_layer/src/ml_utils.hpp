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

#ifndef DML_ML_OWN_UTILS_HPP
#define DML_ML_OWN_UTILS_HPP

#include <core/types.hpp>
#include <dml/detail/ml/operation.hpp>
#include <dml/detail/ml/result.hpp>
#include <new>

namespace dml::detail::ml
{
    // Wrapper for reinterpret_cast<core::completion_record*>
    [[nodiscard]] static inline auto& as_core(result& res) noexcept
    {
        return *std::launder(reinterpret_cast<core::completion_record*>(&res));
    }

    // Wrapper for reinterpret_cast<core::descriptor*>
    [[nodiscard]] static inline auto& as_core(operation& res) noexcept
    {
        return *std::launder(reinterpret_cast<core::descriptor*>(&res));
    }

    // Wrapper for reinterpret_cast<core::result*>
    [[nodiscard]] static inline auto& as_ml(core::completion_record& res) noexcept
    {
        return *std::launder(reinterpret_cast<result*>(&res));
    }

    // Wrapper for reinterpret_cast<operation*>
    [[nodiscard]] static inline auto& as_ml(core::descriptor& res) noexcept
    {
        return *std::launder(reinterpret_cast<operation*>(&res));
    }
}  // namespace dml::detail::ml

#endif  // DML_ML_OWN_UTILS_HPP
