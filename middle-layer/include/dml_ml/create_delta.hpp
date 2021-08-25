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
 * @brief Contains definitions of @ref dml::ml::create_delta type
 */

#ifndef DML_ML_CREATE_DELTA_HPP
#define DML_ML_CREATE_DELTA_HPP

#include <dml_common/types.hpp>
#include <dml_ml/operation.hpp>

namespace dml::ml
{
    /**
     * @ingroup dmlml_operations
     * @brief Create Delta operation
     *
     * May be considered as if being derived from @ref operation
     */
    class create_delta
    {
    public:
        /**
         * @brief Initializes underlying operation with Create Delta operation
         *
         * @param src1            Pointer to the first source memory region
         * @param src2            Pointer to the second source memory region
         * @param size            Byte size of the the memory regions
         * @param delta_record    Pointer to the memory region for delta record
         * @param delta_max_size  Byte size of the delta record memory region
         */
        create_delta(const byte_t *src1,
                     const byte_t *src2,
                     size_t        size,
                     byte_t *      delta_record,
                     size_t        delta_max_size) noexcept;

        /**
         * @brief Executes as Create Delta operation
         */
        void operator()() const noexcept;

        /**
         * @brief Provides polymorphic behavior via casting to @ref operation base class
         *
         * Intentionally implicit
         *
         * @return Reference to the "base" class.
         */
        operator operation &() noexcept { return operation_; }

        /**
         * @brief Provides polymorphic behavior via casting to @ref operation base class (const version)
         *
         * Intentionally implicit
         *
         * @return Reference to the "base" class.
         */
        operator const operation &() const noexcept { return operation_; }

    private:
        operation operation_; /**< Underlying operation */
    };
}  // namespace dml::ml

#endif  //DML_ML_CREATE_DELTA_HPP
