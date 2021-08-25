/*
 * Copyright 2020-2021 Intel Corporation.
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
* @brief
* @date 2/10/2020
*
* @addtogroup core_own
* @{
*/

#include "core_definitions.h"

#ifndef DML_OWN_BAD_ARGUMENT_MACROS_HPP_
#define DML_OWN_BAD_ARGUMENT_MACROS_HPP_

#ifdef DML_CORES_BADARG_CHECK
    #define DML_CORE_BAD_ARGUMENT_RETURN(expression, error_code) \
    if(expression) \
    { \
        return (error_code); \
    }

#else
    #define DML_CORE_BAD_ARGUMENT_RETURN(expression, error_code ) /**< */
#endif

    // Bad Argument wrappers
    #define DML_CORE_CHECK_NULL_POINTER(pointer) \
        DML_CORE_BAD_ARGUMENT_RETURN( NULL == (pointer), DML_STATUS_NULL_POINTER_ERROR)

    #define DML_CORE_CHECK_OVERLAPPING_FORWARD(dst_ptr, src_ptr, length) \
        DML_CORE_BAD_ARGUMENT_RETURN((( (uint64_t) (src_ptr) ) <= ( (uint64_t) (dst_ptr) )) && \
                                     ( ( (uint64_t)((src_ptr) + (length)) ) > ( (uint64_t) (dst_ptr)) ), DML_STATUS_OVERLAPPING_BUFFER_ERROR)

    #define DML_CORE_CHECK_OVERLAPPING_BACKWARD(dst_ptr, src_ptr, length) \
        DML_CORE_CHECK_OVERLAPPING_FORWARD(src_ptr, dst_ptr, length)

    #define DML_CORE_CHECK_OVERLAPPING(pointer1, pointer2, length) \
        DML_CORE_CHECK_OVERLAPPING_FORWARD(pointer1, pointer2, length) \
        DML_CORE_CHECK_OVERLAPPING_BACKWARD(pointer1, pointer2, length)


    #define DML_CORE_CHECK_INPUT_SIZE(condition, status) \
        DML_CORE_BAD_ARGUMENT_RETURN((condition), (status))

    #define DML_CORE_CHECK_OUTPUT_SIZE(condition, status) \
        DML_CORE_BAD_ARGUMENT_RETURN((condition), (status))

#endif //DML_OWN_BAD_ARGUMENT_MACROS_HPP_

/** @} */
