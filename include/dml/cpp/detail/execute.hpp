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
 * @brief Contains internal execute implementation
 */

#ifndef DML_DETAIL_EXECUTE_HPP
#define DML_DETAIL_EXECUTE_HPP

#include <dml/cpp/detail/utils.hpp>
#include <dml/cpp/execution_path.hpp>
#include <dml/cpp/middle_layer/completion_record.hpp>
#include <dml/cpp/middle_layer/make_descriptor.hpp>
#include <dml/cpp/middle_layer/validation.hpp>
#include <dml/cpp/status_code.hpp>

#include "make_result.hpp"

namespace dml::detail
{
    /**
     * @brief Provides common execute implementation
     *
     * @tparam execution_path   Type of execution path
     * @tparam operation        Type of operation
     * @tparam make_operation_t Type of callable make functor
     * @tparam range_check_t    Type of callable range check
     *
     * @param make_operation    Instance of callable make functor
     * @param range_check       Instance of callable range check
     *
     * @return Result of operation
     */
    template <typename execution_path, typename operation, typename make_operation_t, typename range_check_t = detail::always_success>
    inline auto execute(make_operation_t &&make_operation, range_check_t range_check = range_check_t()) noexcept
    {
        if (auto status = range_check(); status != status_code::ok)
        {
            return typename operation::result_type{ status };
        }

        auto descriptor = make_operation();

        if (auto status = ml::validate(descriptor); status != ml::validation_status::success)
        {
            return typename operation::result_type{ detail::to_own(status) };
        }

        auto record = ml::completion_record();
        // If execution_path::run returns status code
        auto status = execution_path()(descriptor, record);
        if (status != ml::submission_status::success)
        {
            return typename operation::result_type{ status_code::error };
        }

#ifdef DML_HW
        if constexpr (std::is_same_v<execution_path, hardware>)
        {
            ml::wait(record);
        }
#endif

        return make_result<typename operation::result_type>(record);
    }

}  // namespace dml::detail

#endif  //DML_DETAIL_EXECUTE_HPP
