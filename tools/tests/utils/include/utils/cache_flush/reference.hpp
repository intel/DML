/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_CACHE_FLUSH_REFERENCE_HPP
#define DML_TESTING_CACHE_FLUSH_REFERENCE_HPP

#include <cstring>
#include <utils/cache_flush/result_builder.hpp>
#include <utils/cache_flush/workload.hpp>

namespace dml::testing
{
    inline auto ReferenceImplementation(Workload<CacheFlushOperation>& workload) noexcept
    {
        // No point in flush caches in reference, there will be no side effects.
        static_cast<void>(workload);

        return ResultBuilder<CacheFlushOperation>().set_status(StatusCode::Success).build();
    }
}  // namespace dml::testing

#endif  //DML_TESTING_CACHE_FLUSH_REFERENCE_HPP
