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

#include <core/view.hpp>
#include <dml/detail/ml/result.hpp>

namespace dml::detail::ml
{
    detail::execution_status get_status(completion_record &record) noexcept
    {
        return static_cast<execution_status>(core::any_completion_record(record).status());
    }

    detail::result_t get_result(completion_record &record) noexcept
    {
        return core::any_completion_record(record).result();
    }

    detail::transfer_size_t get_bytes_completed(completion_record &record) noexcept
    {
        return core::any_completion_record(record).bytes_completed();
    }

    detail::transfer_size_t get_delta_record_size(completion_record &record) noexcept
    {
        return core::make_view<core::operation::create_delta>(record).delta_record_size();
    }

    detail::transfer_size_t get_crc_value(completion_record &record) noexcept
    {
        return core::make_view<core::operation::crc>(record).crc_value();
    }

}  // namespace dml::detail::ml
