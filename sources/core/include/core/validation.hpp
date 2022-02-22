/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_CORE_VALIDATION_HPP
#define DML_CORE_VALIDATION_HPP

#include <core/types.hpp>
#include <dml/detail/common/status.hpp>

namespace dml::core
{
    [[nodiscard]] dml::detail::validation_status validate(const descriptor &dsc) noexcept;
}  // namespace dml::core

#endif  //DML_CORE_VALIDATION_HPP
