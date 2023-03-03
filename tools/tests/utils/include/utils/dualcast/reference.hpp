/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_DUALCAST_REFERENCE_HPP
#define DML_TESTING_DUALCAST_REFERENCE_HPP

#include <cstring>
#include <utils/dualcast/result_builder.hpp>
#include <utils/dualcast/workload.hpp>

namespace dml::testing
{
    inline auto ReferenceImplementation(Workload<DualcastOperation>& workload) noexcept
    {
        std::memmove(workload.get_dst1().data(),
                     workload.get_src().data(),
                     workload.get_src().size());

        std::memmove(workload.get_dst2().data(),
                     workload.get_src().data(),
                     workload.get_src().size());

        return ResultBuilder<DualcastOperation>().set_status(StatusCode::Success).build();
    }
}  // namespace dml::testing

#endif  //DML_TESTING_DUALCAST_REFERENCE_HPP
