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

#ifndef DML_ML_VALIDATION
#define DML_ML_VALIDATION

#include <dml/detail/common/status.hpp>
#include <dml/detail/ml/operation.hpp>

namespace dml::detail::ml
{
    [[nodiscard]] validation_status validate(operation& op) noexcept;
}  // namespace dml::detail::ml

#endif  //DML_ML_VALIDATION
