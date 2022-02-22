/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include "dml_cpuid.h"

#ifdef _WIN32
#include "intrin.h"
typedef int dml_register_t;
#else
#include <cpuid.h>
typedef unsigned dml_register_t;
#endif

dml_core_registers dml_core_cpuidex(dml_register_t leaf, dml_register_t sub_leaf)
{
#ifdef _WIN32
    const dml_register_t eax = 0;
    const dml_register_t ebx = 1;
    const dml_register_t ecx = 2;
    const dml_register_t edx = 3;

    dml_register_t regs[4];
    __cpuidex(regs, leaf, sub_leaf);

    dml_core_registers registers = { regs[eax], regs[ebx], regs[ecx], regs[edx] };

    return registers;
#else
    dml_core_registers registers;
    __cpuid_count(leaf, sub_leaf, registers.eax, registers.ebx, registers.ecx, registers.edx);

    return registers;
#endif
}

dml_core_registers dml_core_cpuid(dml_register_t leaf)
{
    return dml_core_cpuidex(leaf, 0x0);
}

size_t dml_core_get_cache_size()
{
    static size_t cache_size = 0u;

    if (cache_size > 0)
    {
        return cache_size;
    }

    const size_t max_cache_types = 32;

    dml_register_t max_cache_size = 0;

    for (dml_register_t cache_type = 0; cache_type < max_cache_types; cache_type++)
    {
        const dml_register_t cache_parameters = 0x4;

        const dml_core_registers registers = dml_core_cpuidex(cache_parameters, cache_type);

        const dml_register_t none_type         = 0x1f;
        const dml_register_t instruction_cache = 0x2;
        if ((registers.eax & none_type) == none_type)
        {
            break;
        }
        if ((registers.eax & 0x1f) != instruction_cache)
        {
            // Sets = ECX
            const dml_register_t sets = registers.ecx;

            // Line_Size = EBX[11:0]
            const dml_register_t line_size_mask = 0xfff;
            const dml_register_t line_size      = (registers.ebx & line_size_mask);

            // Partitions = EBX[21:12]
            const dml_register_t partitions_mask   = 0x3ff;
            const dml_register_t partitions_offset = 12;
            const dml_register_t partitions        = ((registers.ebx >> partitions_offset) & partitions_mask) + 1;

            // Ways = EBX[31:22]
            const dml_register_t ways_mask   = 0x3ff;
            const dml_register_t ways_offset = 22;
            const dml_register_t ways        = ((registers.ebx >> ways_offset) & ways_mask) + 1;

            // This cache size in bytes
            const dml_register_t this_cache_size = (ways + 1) * (partitions + 1) * (line_size + 1) * (sets + 1);

            if (this_cache_size > (dml_register_t)max_cache_size)
            {
                max_cache_size = this_cache_size;
            }
        }
    }

    cache_size = (size_t)max_cache_size;

    return cache_size;
}
