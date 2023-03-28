/*******************************************************************************
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include "../dml_kernels.h"

#if defined(__linux__)
#include <x86intrin.h>
#else
#include <emmintrin.h>
#include <intrin.h>
#endif

extern "C" void dml_wait_busy_poll(const volatile uint8_t * const pointer)
{
  while (*pointer == 0)
  {
      _mm_pause();
  }
}

extern "C" void dml_wait_umwait(const volatile uint8_t* const pointer)
{
#if defined(__linux__)
    while (*pointer == 0)
    {
        // UMONITOR
        asm volatile(".byte 0xf3, 0x48, 0x0f, 0xae, 0xf0" : : "a"(pointer));

        const auto state = uint32_t(0u);

        const auto timeout      = __rdtsc() + 200u; // I have no idea why 200 is used.
        const auto timeout_low  = static_cast<uint32_t>(timeout);
        const auto timeout_high = static_cast<uint32_t>(timeout >> 32);

        auto r = uint8_t(0);

        // UMWAIT
        asm volatile(".byte 0xf2, 0x48, 0x0f, 0xae, 0xf1\t\n"
                     "setc %0\t\n"
                     : "=r"(r)
                     : "c"(state), "a"(timeout_low), "d"(timeout_high));
    }
#else
    dml_wait_busy_poll(pointer);
#endif
}
