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

#include <core/utils.hpp>
#include <dml/detail/common/status.hpp>

#include "kernels.hpp"

namespace dml::core::kernels
{
    void drain(const_view<descriptor, operation::drain> dsc) noexcept
    {
        auto record = make_view<operation::drain>(get_completion_record(dsc));

        record.status() = static_cast<status_t>(dml::detail::execution_status::success);
    }
}  // namespace dml::core::kernels
