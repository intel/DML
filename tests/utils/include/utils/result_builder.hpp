/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_RESULT_BUILDER_HPP
#define DML_TESTING_RESULT_BUILDER_HPP

#include <type_traits>

namespace dml::testing
{
    template <typename Operation>
    class ResultBuilder
    {
        static_assert(!std::is_same_v<Operation, Operation>,
                      "No result builder defined for operation");
    };
}  // namespace dml::testing

#endif  //DML_TESTING_RESULT_BUILDER_HPP
