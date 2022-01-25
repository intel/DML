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

#include <dml/detail/ml/result.hpp>
#include <dml/detail/ml/view.hpp>
#include <dml/hl/detail/utils.hpp>
#include <dml/hl/execution_path.hpp>
#include <dml/hl/status_code.hpp>

#include "make_result.hpp"

namespace dml::detail
{
    /**
     * @brief Provides common execute implementation
     *
     * @tparam execution_path   Type of execution path
     * @tparam operation        Type of operation
     * @tparam make_task_t      Type of callable make functor
     * @tparam range_check_t    Type of callable range check
     *
     * @param make_task         Instance of callable make functor
     * @param range_check       Instance of callable range check
     *
     * @return Result of operation
     */
    template <typename execution_path_t, typename operation, typename make_task_t, typename range_check_t = detail::always_success>
    inline auto execute(make_task_t &&make_task, range_check_t range_check = range_check_t()) noexcept
    {
        constexpr auto numa = std::numeric_limits<std::uint32_t>::max();

        using execution_path = typename execution_path_t::execution_path;

        if (auto status = range_check(); status != status_code::ok)
        {
            return typename operation::result_type{ status };
        }

        auto task = make_task();

        auto [validation_status, submission_status] = execute<execution_path>(make_view(task), numa);

        if (validation_status != detail::validation_status::success)
        {
            return typename operation::result_type{ detail::to_own(validation_status) };
        }

        if (submission_status != detail::submission_status::success)
        {
            return typename operation::result_type{ status_code::error };
        }

        return make_result<typename operation::result_type>(make_view(task).get_completion_record());
    }

}  // namespace dml::detail

#endif  //DML_DETAIL_EXECUTE_HPP
