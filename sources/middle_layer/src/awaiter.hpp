/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_ML_OWN_AWAITER_HPP
#define DML_ML_OWN_AWAITER_HPP

#if defined(linux)
#include <x86intrin.h>
#else
#include <emmintrin.h>
#include <intrin.h>
#endif

namespace dml::detail::ml
{
    /**
     * @brief Class that allows to defer scope exit to the moment when a certain address is changed
     */
    class awaiter final
    {
    public:
        /**
         * @brief Constructor of the class
         *
         * @param address       pointer to memory that should be asynchronously changed
         * @param initial_value value to compare with
         * @param period        number of clocks between checks
         */
        explicit awaiter(volatile void *address, uint8_t initial_value, uint32_t period = 200) noexcept:
            address_ptr_(reinterpret_cast<volatile uint8_t *>(address)),
            period_(period),
            initial_value_(initial_value)
        {
            // Empty constructor
        }

        /**
         * @brief Destructor that performs actual wait
         */
        ~awaiter() noexcept
        {
#ifdef DML_EFFICIENT_WAIT
            while (initial_value_ == *address_ptr_)
            {
                monitor_address(address_ptr_);

                auto start = current_time();
                wait_until(start + period_, idle_state_);
            }
#else
            while (initial_value_ == *address_ptr_)
            {
                _mm_pause();
            }
#endif
        }

#ifdef DML_EFFICIENT_WAIT
    private:
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

    private:
        volatile uint8_t *address_ptr_;        /**<Pointer to memory that should be asynchronously changed */
        uint32_t          period_        = 0u; /**<Number of clocks between checks */
        uint8_t           initial_value_ = 0u; /**<Value to compare with */
        uint32_t          idle_state_    = 0u; /**<State for CPU wait control */
    };
}  // namespace dml::detail::ml

#endif  // DML_ML_OWN_AWAITER_HPP
