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

#ifndef DML_DETAIL_MAKE_RESULT_HPP
#define DML_DETAIL_MAKE_RESULT_HPP

#include <dml/cpp/common/result.hpp>
#include <dml/cpp/common/status_code.hpp>
#include <dml/cpp/middle_layer/completion_record.hpp>
#include <dml/cpp/middle_layer/result_views.hpp>

namespace dml::detail
{
    /**
     * @todo
     */
    inline auto to_own(ml::execution_status status) noexcept
    {
        switch (status)
        {
            case ml::execution_status::success:
                return status_code::ok;
            case ml::execution_status::false_predicate_success:
                return status_code::false_predicate;
            default:
                // Anything else is considered an error temporarily
                return status_code::error;
        }
    }

    /**
     * @todo
     */
    template <typename result_type>
    auto make_result(ml::completion_record& record) noexcept
    {
        if constexpr (std::is_same_v<result_type, mem_move_result>)
        {
            auto view = ml::views::mem_move_result(record);

            return mem_move_result{ to_own(static_cast<ml::execution_status>(view.status())) };
        }
        if constexpr (std::is_same_v<result_type, mem_copy_result>)
        {
            auto view = ml::views::mem_move_result(record);

            return mem_copy_result{ to_own(static_cast<ml::execution_status>(view.status())) };
        }
        else if constexpr (std::is_same_v<result_type, fill_result>)
        {
            auto view = ml::views::fill_result(record);

            return fill_result{ to_own(static_cast<ml::execution_status>(view.status())) };
        }
        else if constexpr (std::is_same_v<result_type, compare_result>)
        {
            auto view = ml::views::compare_result(record);

            return compare_result{ to_own(static_cast<ml::execution_status>(view.status())),
                                   static_cast<comparison_result>(view.result()),
                                   view.bytes_completed() };
        }
        else if constexpr (std::is_same_v<result_type, create_delta_result>)
        {
            auto view = ml::views::create_delta_result(record);

            return create_delta_result{ to_own(static_cast<ml::execution_status>(view.status())),
                                        static_cast<comparison_result>(view.result()),
                                        view.bytes_completed(),
                                        view.delta_record_size() };
        }
        else if constexpr (std::is_same_v<result_type, apply_delta_result>)
        {
            auto view = ml::views::apply_delta_result(record);

            return apply_delta_result{ to_own(static_cast<ml::execution_status>(view.status())) };
        }
        else if constexpr (std::is_same_v<result_type, dualcast_result>)
        {
            auto view = ml::views::dualcast_result(record);

            return dualcast_result{ to_own(static_cast<ml::execution_status>(view.status())) };
        }
        else if constexpr (std::is_same_v<result_type, crc_result>)
        {
            auto view = ml::views::crc_result(record);

            return crc_result{ to_own(static_cast<ml::execution_status>(view.status())),
                               view.crc_value() };
        }
        else if constexpr (std::is_same_v<result_type, cache_flush_result>)
        {
            auto view = ml::views::cache_flush_result(record);

            return cache_flush_result{ to_own(static_cast<ml::execution_status>(view.status())) };
        }
        else if constexpr (std::is_same_v<result_type, batch_result>)
        {
            auto view = ml::views::batch_result(record);

            return batch_result{ to_own(static_cast<ml::execution_status>(view.status())),
                                 view.descriptors_completed() };
        }
    }

}  // namespace dml::detail

#endif  //DML_DETAIL_MAKE_RESULT_HPP
