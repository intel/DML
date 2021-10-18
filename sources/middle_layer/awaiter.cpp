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

#include <dml/cpp/middle_layer/awaiter.hpp>

#if defined(linux)
#include <x86intrin.h>
#else
#include <intrin.h>
#include <emmintrin.h>
#endif

namespace dml::ml {

#ifdef DML_EFFICIENT_WAIT
    static inline uint64_t current_time() {
        return __rdtsc();
    }

    static inline void monitor_address(volatile void *address) {
        asm volatile(".byte 0xf3, 0x48, 0x0f, 0xae, 0xf0" : : "a"(address));
    }

    static inline int wait_until(unsigned long timeout, unsigned int state) {
        uint8_t r            = 0u;
        auto    timeout_low  = static_cast<uint32_t>(timeout);
        auto    timeout_high = static_cast<uint32_t>(timeout >> 32);

        asm volatile(".byte 0xf2, 0x48, 0x0f, 0xae, 0xf1\t\n"
                     "setc %0\t\n"
        : "=r"(r)
        : "c"(state), "a"(timeout_low), "d"(timeout_high));

        return r;
    }
#endif

    awaiter::awaiter(volatile void *address,
                     uint8_t initial_value,
                     uint32_t period) noexcept
            : address_ptr_(reinterpret_cast<volatile uint8_t *>(address)),
              period_(period),
              initial_value_(initial_value) {
        // Empty constructor
    }

    awaiter::~awaiter() noexcept {
#ifdef DML_EFFICIENT_WAIT
        while (initial_value_ == *address_ptr_) {
            monitor_address(address_ptr_);

            auto start = current_time();
            wait_until(start + period_, idle_state_);
        }
#else
        while (initial_value_ == *address_ptr_) {
            _mm_pause();
        }
#endif
    }
}
