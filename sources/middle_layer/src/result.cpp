/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <core/view.hpp>
#include <dml/detail/ml/result.hpp>

namespace dml::detail::ml
{
    detail::execution_status get_status(completion_record &record) noexcept
    {
        return static_cast<execution_status>(0b111111 & core::any_completion_record(record).status());
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
