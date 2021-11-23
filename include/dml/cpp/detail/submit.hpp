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
 * @brief Contains internal submit implementation
 */

#ifndef DML_DETAIL_SUBMIT_HPP
#define DML_DETAIL_SUBMIT_HPP

#include <dml/cpp/detail/handler.hpp>
#include <dml/cpp/detail/utils.hpp>
#include <dml/cpp/execution_path.hpp>
#include <dml/cpp/middle_layer/status.hpp>

namespace dml::detail
{
    /**
     * @brief Provides common submit implementation
     *
     * @tparam execution_path        Type of execution path
     * @tparam operation_t           Type of operation
     * @tparam execution_interface_t Type of execution interface
     * @tparam range_check_t         Type of callable range check
     * @tparam make_operation_t      Type of callable make functor
     * @param executor               Instance of execution interface
     * @param range_check            Instance of callable range check
     * @param make_operation         Instance of callable make functor
     *
     * @return Handler for operation
     */
    template <typename execution_path,
              typename operation_t,
              typename execution_interface_t,
              typename make_operation_t,
              typename range_check_t = detail::always_success>
    inline auto submit(const execution_interface_t& executor,
                       make_operation_t&&           make_operation,
                       range_check_t                range_check = range_check_t())
    {
        if (auto status = range_check(); status != status_code::ok)
        {
            return executor.template make_handler<operation_t>(status);
        }

        auto operation = make_operation();

        auto op_handler = executor.template make_handler<operation_t>(detail::to_own(ml::validate(operation)));

        if (!op_handler.valid())
        {
            return op_handler;
        }

        // If execution_path{} returns status code (hw path)
#ifdef DML_HW
        if constexpr (std::is_same_v<execution_path, hardware>)
        {
            auto& result = detail::get_ml_result(op_handler);
            auto  status = executor.execute(
                [operation, &result]() mutable
                {
                    return execution_path{}(operation, result);
                });

            if (status != ml::submission_status::success)
            {
                return executor.template make_handler<operation_t>(status_code::error);
            }
        }
        else
        {
#endif
            auto& result = detail::get_ml_result(op_handler);
            executor.execute(
                [operation, &result]() mutable
                {
                    auto status = execution_path{}(operation, result);
                    if (status != ml::submission_status::success)
                    {
                        result.bytes[0] = 0xFF;  // Temporary
                    }
                });
#ifdef DML_HW
        }
#endif

        return op_handler;
    }

}  // namespace dml::detail

#endif  //DML_DETAIL_SUBMIT_HPP
