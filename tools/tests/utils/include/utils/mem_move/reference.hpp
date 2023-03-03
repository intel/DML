/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_MEM_MOVE_REFERENCE_HPP
#define DML_TESTING_MEM_MOVE_REFERENCE_HPP

#include <cstring>
#include <utils/mem_move/result_builder.hpp>
#include <utils/mem_move/workload.hpp>

namespace dml::testing
{
    inline auto ReferenceImplementation(Workload<MemMoveOperation>& workload) noexcept
    {
        std::memmove(workload.get_dst().data(),
                     workload.get_src().data(),
                     workload.get_src().size());

        return ResultBuilder<MemMoveOperation>().set_status(StatusCode::Success).build();
    }
}  // namespace dml::testing

#endif  //DML_TESTING_MEM_MOVE_REFERENCE_HPP
