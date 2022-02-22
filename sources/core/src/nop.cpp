/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <core/utils.hpp>
#include <dml/detail/common/status.hpp>

#include "kernels.hpp"

namespace dml::core::kernels
{
    void nop(const_view<descriptor, operation::nop> dsc) noexcept
    {
        auto record = make_view<operation::nop>(get_completion_record(dsc));

        record.status() = static_cast<status_t>(dml::detail::execution_status::success);
    }

}  // namespace dml::core::kernels
