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

/**
 * @date 05/19/2021
 * @defgroup dmlml DML Middle Layer
 * @brief Middle Layer for Intel(R) Data Mover Library (Intel® DML)
 */

#ifndef DML_ML_OPERATION_HPP
#define DML_ML_OPERATION_HPP

#include <dml_common/status_code.hpp>
#include <dml_common/types.hpp>
#include <dml_ml/result.hpp>

namespace dml::ml
{
    /**
     * @defgroup dmlml_operations Operations
     * @ingroup dmlml
     * @brief Contains description for all operations
     * @{
     */

    /**
     * @brief Base class for all operations.
     *
     * May be used as generic (polymorphic) operation.
     * Underlying data array is used in implementation files.
     */
    class alignas(64u) operation
    {
        /**
         * @brief This class memory size requirement
         */
        static constexpr auto memory_size = 64u;

    public:
        /**
         * @brief Default constructor.
         *
         * Fills all the data with zeroes
         */
        operation() noexcept = default;

        /**
         * @brief Associate a result with this operation instance
         *
         * To be used before execution.
         *
         * @param record Reference to result instance
         */
        void associate(result& record) noexcept;

        /**
         * @brief Returns raw pointer to the underlying data array
         *
         * @return A pointer to the underlying data.
         */
        [[nodiscard]] byte_t *data() noexcept { return data_; }

        /**
         * @brief Returns raw pointer to the underlying data array (const version)
         *
         * @return A const pointer to the underlying data.
         */
        [[nodiscard]] const byte_t *data() const noexcept { return data_; }

        /**
         * @brief Executes operation
         *
         * What operation it is initialized with is known at runtime.
         */
        void operator()() const noexcept;

    private:
        byte_t data_[memory_size]{}; /**< Underlying data array */
    };

    /**
     * @}
     */
}  // namespace dml::ml

#endif  //DML_ML_OPERATION_HPP
