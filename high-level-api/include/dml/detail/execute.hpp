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

#include <dml/execution_path.hpp>
#include <dml_common/status_code.hpp>
#include <dml_ml/operation.hpp>
#include <dml_ml/result.hpp>

namespace dml::detail
{
    /**
     * @brief Provides common execute implementation
     *
     * @tparam execution_path   Type of execution path
     * @tparam operation        Type of operation
     * @tparam range_check_t    Type of callable range check
     * @tparam make_operation_t Type of callable make functor
     * @param range_check       Instance of callable range check
     * @param make_operation    Instance of callable make functor
     *
     * @return Result of operation
     */
    template <typename execution_path, typename operation, typename range_check_t, typename make_operation_t>
    inline auto execute(range_check_t &&range_check, make_operation_t &&make_operation) noexcept
    {
        auto status = range_check();

        if (status != status_code::ok)
        {
            return typename operation::result_type{status};
        }

        auto result = ml::result();

        // If execution_path::run returns status code
        if constexpr (std::is_same_v<status_code, std::invoke_result_t<execution_path, ml::operation, ml::result&>>)
        {
            status = execution_path{}(make_operation(), result);
            if (status != status_code::ok)
            {
                return typename operation::result_type{status};
            }
        }
        else
        {
            execution_path{}(make_operation(), result);
        }

#ifdef DML_HW
        if constexpr (std::is_same_v<execution_path, hardware>)
        {
            result.wait();
        }
#endif

        return static_cast<typename operation::result_type>(result);
    }

}  // namespace dml::detail

#endif  //DML_DETAIL_EXECUTE_HPP
