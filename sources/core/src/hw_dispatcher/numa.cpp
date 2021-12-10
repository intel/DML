/*
 * Copyright 2021 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they
 * were provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit
 * this software or the related documents without Intel's prior written
 * permission.
 *
 * This software and the related documents are provided as is, with no
 * express or implied warranties, other than those that are expressly
 * stated in the License.
 *
 */

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
