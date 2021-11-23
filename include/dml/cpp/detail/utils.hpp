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

/**
 * @date 05/20/2021
 * @brief Contains internal utilities
 */

#ifndef DML_DETAIL_UTILS_HPP
#define DML_DETAIL_UTILS_HPP

#include <dml/cpp/middle_layer/values.hpp>
#include <dml/cpp/status_code.hpp>

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
        auto operator()() const noexcept
        {
            return status_code::ok;
        }
    };

    /**
     * @brief Converts Middle Layer's @ref dml::ml::validation_status to @ref dml::status_code
     *
     * @param status Status for conversion
     *
     * @return dml::status_code that represents dml::ml::validation_status
     */
    [[nodiscard]] static constexpr auto to_own(ml::validation_status status) noexcept
    {
        switch (status)
        {
            case ml::validation_status::success:
                return status_code::ok;
            case ml::validation_status::address_is_null:
                return status_code::nullptr_error;
            case ml::validation_status::size_is_null:
                return status_code::bad_size;
            case ml::validation_status::delta_size_is_wrong:
                return status_code::bad_size;
            case ml::validation_status::delta_input_size_is_wrong:
                return status_code::bad_size;
            case ml::validation_status::delta_input_size_overflow:
                return status_code::bad_size;
            case ml::validation_status::buffers_overlap:
                return status_code::buffers_overlapping;
            case ml::validation_status::address_is_misaligned:
                return status_code::bad_alignment;
            case ml::validation_status::delta_record_size_is_wrong:
                return status_code::delta_bad_size;
            case ml::validation_status::dualcast_address_is_wrong:
                return status_code::dualcast_bad_padding;
            case ml::validation_status::batch_size_is_wrong:
                return status_code::bad_length;
            case ml::validation_status::unsupported_operation:
                return status_code::unsupported_operation;
            default:
                return status_code::error;
        }
    }
}  // namespace dml::detail

#endif  //DML_DETAIL_UTILS_HPP
