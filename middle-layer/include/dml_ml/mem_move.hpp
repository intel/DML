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
 * @brief Contains definitions of @ref dml::ml::mem_move type
 */

#ifndef DML_ML_MEM_MOVE_HPP
#define DML_ML_MEM_MOVE_HPP

#include <dml_common/types.hpp>
#include <dml_ml/operation.hpp>

namespace dml::ml
{
    /**
     * @ingroup dmlml_operations
     * @brief Memory Move operation
     *
     * May be considered as if being derived from @ref operation
     */
    class mem_move
    {
    public:
        /**
         * @brief Initializes underlying operation with Memory Move operation
         *
         * @param src     Pointer to the source memory region
         * @param dst     Pointer to the destination memory region
         * @param size    Byte size of the memory regions
         */
        mem_move(const byte_t *src, byte_t *dst, size_t size) noexcept;

        /**
         * @brief Executes as Memory Move operation
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

#endif  //DML_ML_MEM_MOVE_HPP
