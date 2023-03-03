/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_WORKLOAD_HPP
#define DML_TESTING_WORKLOAD_HPP

#include <type_traits>
#include <ostream>

namespace dml::testing
{
    class WorkloadBase
    {
      protected:
        explicit WorkloadBase(Memory&& memory) : memory_(std::move(memory)) {}

      public:
        friend std::ostream &operator<<(std::ostream &os, const WorkloadBase &base)
        {
            return os << base.memory_;
        }

      protected:
        Memory memory_;
    };

    template <typename Operation>
    class Workload
    {
        static_assert(!std::is_same_v<Operation, Operation>, "No workload defined for operation");
    };
}  // namespace dml::testing

#endif  //DML_TESTING_WORKLOAD_HPP
