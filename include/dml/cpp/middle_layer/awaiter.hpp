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

#ifndef DML_AWAITER_HPP
#define DML_AWAITER_HPP

#include <cstdint>

namespace dml::ml
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
        explicit awaiter(volatile void *address, uint8_t initial_value, uint32_t period = 200) noexcept;

        /**
         * @brief Destructor that performs actual wait
         */
        ~awaiter() noexcept;

    private:
        volatile uint8_t *address_ptr_;        /**<Pointer to memory that should be asynchronously changed */
        uint32_t          period_        = 0u; /**<Number of clocks between checks */
        uint8_t           initial_value_ = 0u; /**<Value to compare with */
        uint32_t          idle_state_    = 0u; /**<State for CPU wait control */
    };
}  // namespace dml::ml

#endif  //DML_AWAITER_HPP
