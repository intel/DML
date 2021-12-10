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

#ifndef DML_ML_RESULT_HPP
#define DML_ML_RESULT_HPP

#include <dml/detail/common/status.hpp>
#include <dml/detail/common/types.hpp>
#include <dml/detail/ml/operation.hpp>

namespace dml::detail::ml
{
    struct alignas(32) result
    {
        byte_t bytes[32];
    };

    void bind(operation& op, result& res) noexcept;

    void wait(result& res) noexcept;

    [[nodiscard]] detail::execution_status get_status(result& res) noexcept;

    [[nodiscard]] detail::result_t get_result(result& res) noexcept;

    [[nodiscard]] detail::transfer_size_t get_bytes_completed(result& res) noexcept;

    [[nodiscard]] detail::transfer_size_t get_delta_record_size(result& res) noexcept;

    [[nodiscard]] detail::transfer_size_t get_crc_value(result& res) noexcept;

    [[nodiscard]] inline bool is_finished(const volatile result& res) noexcept
    {
        return 0 != res.bytes[0];
    }
}  // namespace dml::detail::ml

#endif  //DML_ML_RESULT_HPP
