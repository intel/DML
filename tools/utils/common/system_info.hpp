/*******************************************************************************
 * Copyright (C) 2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TOOLS_UTILS_COMMON_SYSTEM_INFO_HPP_
#define DML_TOOLS_UTILS_COMMON_SYSTEM_INFO_HPP_

#include <cstdint>

#if defined(__linux__)
#include <x86intrin.h>
#endif

namespace dml::test {

/**
 * @brief Returns current numa_id, -1 on error
 *
*/
static inline int32_t get_numa_id() {
#if defined(__linux__)
    uint32_t tsc_aux = 0U;

    __rdtscp(&tsc_aux);

    // Linux encodes NUMA node into [32:12] of TSC_AUX
    return static_cast<int32_t>(tsc_aux >> 12);
#else
    // Not supported in Windows yet
    return -1;
#endif
}

} // namespace dml::test

#endif // DML_TOOLS_UTILS_COMMON_SYSTEM_INFO_HPP_
