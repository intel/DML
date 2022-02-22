/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_CORE_OWN_KERNELS_CPUID_H
#define DML_CORE_OWN_KERNELS_CPUID_H

#ifdef _WIN32
typedef int dml_register_t;
#else
typedef unsigned dml_register_t;
#endif

#include <stddef.h>

#define DML_CPUID_EXTENSIONS 0x7

#define DML_AVX512F  (1 << 16)
#define DML_AVX512DQ (1 << 17)
#define DML_AVX512CD (1 << 28)
#define DML_AVX512BW (1 << 30)
#define DML_AVX512VL (1u << 31)

#define DML_AVX512_MASK (DML_AVX512F | DML_AVX512DQ | DML_AVX512CD | DML_AVX512BW | DML_AVX512VL)

#define DML_CLFLUSHOPT (1 << 23)
#define DML_CLWB       (1 << 24)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    dml_register_t eax;
    dml_register_t ebx;
    dml_register_t ecx;
    dml_register_t edx;
} dml_core_registers;

dml_core_registers dml_core_cpuidex(dml_register_t leaf, dml_register_t sub_leaf);

dml_core_registers dml_core_cpuid(dml_register_t leaf);

size_t dml_core_get_cache_size();

#ifdef __cplusplus
}
#endif

#endif  // DML_CORE_OWN_KERNELS_CPUID_H
