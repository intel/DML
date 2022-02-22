/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_STATE_HPP
#define DML_STATE_HPP

#include <cstddef>
#include <dml/detail/ml/task.hpp>

namespace dml
{
    struct state
    {
        dml::detail::ml::task<dml::detail::ml::stack_allocator> task;
        dml_path_t                                              path;
    };
}  // namespace dml

#endif  // DML_STATE_HPP
