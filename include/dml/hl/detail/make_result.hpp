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

#include <dml/detail/common/status.hpp>
#include <dml/hl/result.hpp>
#include <dml/hl/status_code.hpp>

namespace dml::detail
{
    /**
     * @todo
     */
    inline auto to_own(detail::execution_status status) noexcept
    {
        switch (status)
        {
            case detail::execution_status::success:
                return status_code::ok;
            case detail::execution_status::false_predicate_success:
                return status_code::false_predicate;
            case detail::execution_status::page_fault_during_processing:
                return status_code::partial_completion;
            default:
                // Anything else is considered an error temporarily
                return status_code::error;
        }
    }

    /**
     * @todo
     */
    template <typename result_type>
    auto make_result(detail::completion_record& record) noexcept
    {
        if constexpr (std::is_same_v<result_type, mem_move_result>)
        {
            return mem_move_result{ to_own(detail::ml::get_status(record)) };
        }
        if constexpr (std::is_same_v<result_type, mem_copy_result>)
        {
            return mem_copy_result{ to_own(detail::ml::get_status(record)) };
        }
        else if constexpr (std::is_same_v<result_type, fill_result>)
        {
            return fill_result{ to_own(detail::ml::get_status(record)) };
        }
        else if constexpr (std::is_same_v<result_type, dml::compare_result>)
        {
            return dml::compare_result{ to_own(detail::ml::get_status(record)),
                                        static_cast<comparison_result>(detail::ml::get_result(record)),
                                        detail::ml::get_bytes_completed(record) };
        }
        else if constexpr (std::is_same_v<result_type, dml::create_delta_result>)
        {
            return dml::create_delta_result{ to_own(detail::ml::get_status(record)),
                                             static_cast<comparison_result>(detail::ml::get_result(record)),
                                             detail::ml::get_bytes_completed(record),
                                             detail::ml::get_delta_record_size(record) };
        }
        else if constexpr (std::is_same_v<result_type, apply_delta_result>)
        {
            return apply_delta_result{ to_own(detail::ml::get_status(record)) };
        }
        else if constexpr (std::is_same_v<result_type, dualcast_result>)
        {
            return dualcast_result{ to_own(detail::ml::get_status(record)) };
        }
        else if constexpr (std::is_same_v<result_type, crc_result>)
        {
            return crc_result{ to_own(detail::ml::get_status(record)), detail::ml::get_crc_value(record) };
        }
        else if constexpr (std::is_same_v<result_type, cache_flush_result>)
        {
            return cache_flush_result{ to_own(detail::ml::get_status(record)) };
        }
        else if constexpr (std::is_same_v<result_type, batch_result>)
        {
            return batch_result{ to_own(detail::ml::get_status(record)), detail::ml::get_bytes_completed(record) };
        }
    }

}  // namespace dml::detail

#endif  //DML_DETAIL_MAKE_RESULT_HPP
