/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_ML_RESULT_HPP
#define DML_ML_RESULT_HPP

#include <dml/detail/common/status.hpp>
#include <dml/detail/common/types.hpp>

namespace dml::detail::ml
{
    [[nodiscard]] detail::execution_status get_status(completion_record& record) noexcept;

    [[nodiscard]] detail::result_t get_result(completion_record& record) noexcept;

    [[nodiscard]] detail::transfer_size_t get_bytes_completed(completion_record& record) noexcept;

    [[nodiscard]] detail::transfer_size_t get_delta_record_size(completion_record& record) noexcept;

    [[nodiscard]] detail::transfer_size_t get_crc_value(completion_record& record) noexcept;
}  // namespace dml::detail::ml

#endif  //DML_ML_RESULT_HPP
