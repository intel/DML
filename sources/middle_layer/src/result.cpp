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

#include <core/completion_record_views.hpp>
#include <core/descriptor_views.hpp>
#include <dml/detail/ml/result.hpp>

#include "ml_utils.hpp"

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

    private:
        volatile uint8_t *address_ptr_;        /**<Pointer to memory that should be asynchronously changed */
        uint32_t          period_        = 0u; /**<Number of clocks between checks */
        uint8_t           initial_value_ = 0u; /**<Value to compare with */
        uint32_t          idle_state_    = 0u; /**<State for CPU wait control */
    };

    void wait(result &res) noexcept
    {
        awaiter wait_for(static_cast<volatile void *>(&res), 0);
    }

    void bind(operation &op, result &res) noexcept
    {
        auto view = core::any_descriptor(as_core(op));

        view.flags() |= static_cast<flags_t>(flag::completion_record_address_valid) | static_cast<flags_t>(flag::request_completion_record);

        view.completion_record_address() = reinterpret_cast<address_t>(&res);

        res.bytes[0] = 0;
    }

    detail::execution_status get_status(result &res) noexcept
    {
        return static_cast<execution_status>(core::any_completion_record(as_core(res)).status());
    }

    detail::result_t get_result(result &res) noexcept
    {
        return core::any_completion_record(as_core(res)).result();
    }

    detail::transfer_size_t get_bytes_completed(result &res) noexcept
    {
        return core::any_completion_record(as_core(res)).bytes_completed();
    }

    detail::transfer_size_t get_delta_record_size(result &res) noexcept
    {
        return core::create_delta_completion_record(as_core(res)).delta_record_size();
    }

    detail::transfer_size_t get_crc_value(result &res) noexcept
    {
        return core::crc_completion_record(as_core(res)).crc_value();
    }

}  // namespace dml::detail::ml
