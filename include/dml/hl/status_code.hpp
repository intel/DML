/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_COMMON_STATUS_HPP
#define DML_COMMON_STATUS_HPP

/**
 * @date 05/19/2021
 * @brief Contains definition of status type
 */

#include <dml/hl/types.hpp>

namespace dml
{
    /**
     * @ingroup dml_cpp_common_types
     * @brief Status codes for error reporting
     */
    enum class status_code : uint32_t
    {
        ok,                    /**< Operation completed successfully */
        false_predicate,       /**< Operation completed successfully, but result is unexpected */
        partial_completion,    /**< Operation was partially completed */
        nullptr_error,         /**< One of data pointers is NULL */
        bad_size,              /**< Invalid byte size was specified */
        bad_length,            /**< Invalid number of elements was specified */
        inconsistent_size,     /**< Input data sizes are different */
        dualcast_bad_padding,  /**< Bits 11:0 of the two destination addresses are not the same. */
        bad_alignment,         /**< One of data pointers has invalid alignment */
        buffers_overlapping,   /**< Buffers overlap with each other */
        delta_bad_size,        /**< Invalid delta record size was specified */
        delta_delta_empty,     /**< Delta record is empty */
        batch_overflow,        /**< Batch is full */
        execution_failed,      /**< Unknown execution error */
        unsupported_operation, /**< Unknown execution error */
        queue_busy,            /**< Enqueue failed to one or several queues */
        error                  /**< Internal library error occurred */
    };
}  // namespace dml

#endif  //DML_COMMON_STATUS_HPP
