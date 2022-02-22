/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @date 05/20/2021
 * @brief Contains internal utilities
 */

#ifndef DML_DETAIL_UTILS_HPP
#define DML_DETAIL_UTILS_HPP

#include <dml/detail/common/status.hpp>
#include <dml/hl/status_code.hpp>

/**
 * @brief Checks whether two sizes are the same
 */
#define DML_VALIDATE_SIZE_CONSISTENCY(lhs, rhs) \
    if (lhs != rhs)                             \
        return dml::status_code::inconsistent_size;

namespace dml::detail
{
    /**
     * @todo
     */
    struct always_success
    {
        /**
         * @brief Functor which always return success status
         *
         * @return status_code::ok
         */
        auto operator()() const noexcept
        {
            return status_code::ok;
        }
    };

    /**
     * @brief Converts Middle Layer status code to @ref dml::status_code
     *
     * @param status Status for conversion
     *
     * @return dml::status_code
     */
    [[nodiscard]] static constexpr auto to_own(detail::validation_status status) noexcept
    {
        switch (status)
        {
            case detail::validation_status::success:
                return status_code::ok;
            case detail::validation_status::null_address:
                return status_code::nullptr_error;
            case detail::validation_status::null_size:
                return status_code::bad_size;
            case detail::validation_status::wrong_size:
                return status_code::bad_size;
            case detail::validation_status::large_size:
                return status_code::bad_size;
            case detail::validation_status::overlapping:
                return status_code::buffers_overlapping;
            case detail::validation_status::misalignment:
                return status_code::bad_alignment;
            case detail::validation_status::wrong_delta_size:
                return status_code::delta_bad_size;
            case detail::validation_status::wrong_dualcast_address:
                return status_code::dualcast_bad_padding;
            case detail::validation_status::wrong_batch_size:
                return status_code::bad_length;
            case detail::validation_status::unsupported_operation:
                return status_code::unsupported_operation;
            default:
                return status_code::error;
        }
    }
}  // namespace dml::detail

#endif  //DML_DETAIL_UTILS_HPP
