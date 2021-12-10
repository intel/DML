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

#include <core/device.hpp>
#include <dml/detail/ml/execution_path.hpp>

#include "ml_utils.hpp"

namespace dml::detail::ml::execution_path
{
    submission_status software::submit(operation& op, result& res) noexcept
    {
        return core::software_device().submit(as_core(op), as_core(res));
    }

    submission_status hardware::submit(operation& op, result& res) noexcept
    {
        return core::hardware_device().submit(as_core(op), as_core(res));
    }
}  // namespace dml::detail::ml::execution_path
