/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#if defined(linux)
#include <x86intrin.h>
#endif

#include "numa.hpp"

namespace dml::core::util
{
    uint32_t get_numa_id() noexcept
    {
#if defined(linux)
        uint32_t tsc_aux = 0;

        __rdtscp(&tsc_aux);

        // Linux encodes NUMA node into [32:12] of TSC_AUX
        return tsc_aux >> 12;
#else
        // Not supported in Windows yet
        return 0;
#endif
    }

}  // namespace dml::core::util
