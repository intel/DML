/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_FILL_REFERENCE_HPP
#define DML_TESTING_FILL_REFERENCE_HPP

#include <cstring>
#include <utils/fill/result_builder.hpp>
#include <utils/fill/workload.hpp>

namespace dml::testing
{
    inline auto ReferenceImplementation(Workload<FillOperation>& workload) noexcept
    {
        auto i = 0u;
        for (auto& elem : workload.get_dst())
        {
            elem = reinterpret_cast<std::uint8_t*>(&workload.get_pattern())[i];

            i = (i + 1) % sizeof(std::uint64_t);
        }

        return ResultBuilder<FillOperation>().set_status(StatusCode::Success).build();
    }
}  // namespace dml::testing

#endif  //DML_TESTING_FILL_REFERENCE_HPP
