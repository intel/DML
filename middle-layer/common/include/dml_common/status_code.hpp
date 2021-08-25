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

#ifndef DML_COMMON_STATUS_HPP
#define DML_COMMON_STATUS_HPP

/**
 * @date 05/19/2021
 * @brief Contains definition of status type
 */

#include <dml_common/types.hpp>

namespace dml
{
    /**
     * @ingroup dml_cpp_common_types
     * @brief Status codes for error reporting
     */
    enum class status_code : uint32_t
    {
        ok,                   /**< Operation completed successfully */
        false_predicate,    /**< Operation completed successfully, but result is unexpected */
        nullptr_error,        /**< One of data pointers is NULL */
        bad_size,             /**< Invalid byte size was specified */
        bad_length,           /**< Invalid number of elements was specified */
        inconsistent_size,    /**< Input data sizes are different */
        dualcast_bad_padding, /**< Bits 11:0 of the two destination addresses are not the same. */
        bad_alignment,        /**< One of data pointers has invalid alignment */
        buffers_overlapping,  /**< Buffers overlap with each other */
        delta_bad_size,       /**< Invalid delta record size was specified */
        delta_delta_empty,    /**< Delta record is empty */
        batch_overflow,       /**< Batch is full */
        execution_failed,     /**< Unknown execution error */
        error                 /**< Internal library error occurred */
    };
}  // namespace dml

#endif  //DML_COMMON_STATUS_HPP
