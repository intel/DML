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

#include "dml/dmldefs.h"

#ifndef OWN_DML_CHECKERS_HPP_
#define OWN_DML_CHECKERS_HPP_

/* ====== DML Flow Checkers ====== */

/**
 * @brief Checks that @ref dml_job_t in the batch mode
 */
#define IS_BATCH_MODE(dml_job_ptr) ((dml_job_ptr)->flags & (DML_FLAG_BATCH | DML_FLAG_BATCH_LAST))


/**
 * @brief Checks that there is a place in the batch
 */
#define IS_AVAILABLE_SPACE_IN_BATCH(batch_info) ((batch_info).size < (batch_info).max_size)


/**
 * @brief Checks that batch length will be greater then 1
 */
#define IS_ALLOWED_BATCH_LENGTH(dml_job_ptr, batch_info) \
 !(\
     ((dml_job_ptr)->flags & DML_FLAG_BATCH_LAST) \
     &((batch_info).size == 0u)\
     )


/**
 * @brief Transforms pointer into unsigned integer
 */
#define OWN_POINTER_TO_UINT64(ptr) ((uint64_t) (ptr))

/**
 * @brief Checks if one buffer spans another
 */
#define IS_BUFFER_SPAN(left_buffer_ptr, left_buffer_size, right_buffer_ptr) \
        ((OWN_POINTER_TO_UINT64(left_buffer_ptr) <= OWN_POINTER_TO_UINT64(right_buffer_ptr)) \
          && (OWN_POINTER_TO_UINT64((left_buffer_ptr) + (left_buffer_size)) > OWN_POINTER_TO_UINT64(right_buffer_ptr)))

/* ====== DML Bad Argument Checkers ====== */

#ifdef DML_BADARG_CHECK
    #define DML_BAD_ARGUMENT_RETURN(expression, error_code) \
    if(expression) \
    { \
        return (error_code); \
    }
#else
    #define DML_BAD_ARGUMENT_RETURN(expression, error_code ) /**< */
#endif


// Bad Argument wrappers
#define DML_BAD_ARGUMENT_NULL_POINTER(pointer) \
    DML_BAD_ARGUMENT_RETURN( NULL == (pointer), DML_STATUS_NULL_POINTER_ERROR);

#define DML_BAD_ARGUMENT_INCORRECT_PATH(path) \
    DML_BAD_ARGUMENT_RETURN( DML_PATH_AUTO != (path) \
                             && DML_PATH_SW != (path) \
                             && DML_PATH_HW != (path), \
                                DML_STATUS_PATH_ERROR);

#define DML_BAD_ARGUMENT_INCORRECT_BATCH_OPERATION(dml_job_ptr) \
    DML_BAD_ARGUMENT_RETURN(IS_BATCH_MODE(dml_job_ptr) \
                            && ((dml_job_ptr)->operation == DML_OP_DRAIN), \
                            DML_STATUS_JOB_OPERATION_ERROR)

#define DML_BAD_ARGUMENT_BUFFERS_OVERLAPPING(buffer_1_ptr, buffer_1_size, buffer_2_ptr, buffer_2_size) \
        DML_BAD_ARGUMENT_RETURN(IS_BUFFER_SPAN(buffer_1_ptr, buffer_1_size, buffer_2_ptr)\
                                || IS_BUFFER_SPAN(buffer_2_ptr, buffer_2_size, buffer_1_ptr), \
                                DML_STATUS_OVERLAPPING_BUFFER_ERROR)

#endif //OWN_DML_CHECKERS_HPP_

/** @} */
