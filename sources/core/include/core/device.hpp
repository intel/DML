/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_CORE_EXECUTION_DEVICE_HPP
#define DML_CORE_EXECUTION_DEVICE_HPP

#include <core/types.hpp>
#include <dml/detail/common/status.hpp>

namespace dml::core
{
    class software_device
    {
    public:
        [[nodiscard]] dml::detail::submission_status submit(const descriptor& dsc) noexcept;
    };

    class hardware_device
    {
    public:
        [[nodiscard]] dml::detail::submission_status submit(const descriptor& descriptor, std::uint32_t numa_id) noexcept;
    };
}  // namespace dml::core

#endif  //DML_CORE_EXECUTION_DEVICE_HPP
