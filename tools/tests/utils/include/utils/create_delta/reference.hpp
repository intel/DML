/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_CREATE_DELTA_REFERENCE_HPP
#define DML_TESTING_CREATE_DELTA_REFERENCE_HPP

#include <cstring>
#include <utils/create_delta/result_builder.hpp>
#include <utils/create_delta/workload.hpp>

namespace dml::testing
{
    inline auto ReferenceImplementation(Workload<CreateDeltaOperation> &workload) noexcept
    {
        const auto block_count = workload.get_src1().size() / 8u;

        auto *src1  = reinterpret_cast<std::uint64_t *>(workload.get_src1().data());
        auto *src2  = reinterpret_cast<std::uint64_t *>(workload.get_src2().data());
        auto *delta = workload.get_delta().data();

        auto written_size = 0u;
        auto result       = 0u;
        for (auto i = 0u; i < block_count; ++i)
        {
            if (src1[i] != src2[i])
            {
                if (written_size < workload.get_delta().size())
                {
                    result = 1u;

                    *(reinterpret_cast<std::uint16_t *>(delta + written_size)) = std::uint16_t(i);
                    *(reinterpret_cast<std::uint64_t *>(delta + written_size + 2u)) = src2[i];

                    written_size += 10u;
                }
                else
                {
                    result = 2u;
                    break;
                }
            }
        }

        // Does not check for 111 and 000 masks
        auto status = workload.check_result_enabled()
                          ? result == (uint8_t(workload.get_expected_result() >> 1u)) ? StatusCode::Success : StatusCode::FalsePredicate
                          : StatusCode::Success;

        return ResultBuilder<CreateDeltaOperation>()
            .set_status(status)
            .set_result(result)
            .set_written_size(written_size)
            .build();
    }
}  // namespace dml::testing

#endif  //DML_TESTING_CREATE_DELTA_REFERENCE_HPP
