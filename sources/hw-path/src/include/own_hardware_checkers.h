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
 * @brief Contains a specific checkers for hardware path
 * @date 3/19/2020
 *
 */

#ifndef DML_OWN_HARDWARE_CHECKERS_H__
#define DML_OWN_HARDWARE_CHECKERS_H__

#ifdef __cplusplus
extern "C" {
#endif

static const uint32_t own_dif_block_sizes[4] = { 512u, 520u, 4096u, 4104u }; /**< Contain associative data block size for protection with DIF */

/**
 * @brief Returns specified code in case if condition not true
 */
#define OWN_HARDWARE_RETURN_IF(condition, error_code) \
if(condition)\
{ \
  return error_code;  \
}


/**
 * @brief Checks @ref own_hw_state_t on case if that is corrupted
 */
#define OWN_CHECK_DML_HARDWARE_STATE(hw_state_ptr) \
if (hw_state_ptr->guard != OWN_HW_STATE_GUARD) \
{ \
     return DML_STATUS_JOB_CORRUPTED; \
}


#define OWN_CHECK_UNPROTECTED_STREAM_SIZE(stream_size, block_size)\
if ( (stream_size) % own_dif_block_sizes[block_size])\
{\
    return DML_STATUS_JOB_LENGTH_ERROR;\
}


#define OWN_CHECK_PROTECTED_STREAM_SIZE(stream_size, block_size)\
if ( (stream_size) % (own_dif_block_sizes[block_size] + 8u))\
{\
    return DML_STATUS_JOB_LENGTH_ERROR;\
}


#ifdef DML_BADARG_CHECK
    #define DML_BAD_ARGUMENT_RETURN(expression, error_code) \
    if(expression) \
    { \
        return (error_code); \
    }
#else
    #define DML_BAD_ARGUMENT_RETURN(expression, error_code ) /**< */
#endif


/**
 * @brief Macros for internal errors handling
 */
#define DML_RETURN_IN_CASE_OF_ERROR(status) \
if ((status) && ((status) != DML_STATUS_FALSE_PREDICATE_OK))\
{\
    return (status);\
}


/**
 * @brief Macros for internal success status handling
 */
#define DML_RETURN_IN_CASE_OF_SUCCESS(status) \
if (((status) == DML_STATUS_OK) || ((status) == DML_STATUS_FALSE_PREDICATE_OK))\
{\
    return (status);\
}


// Bad Argument wrappers
#define DML_BAD_ARGUMENT_NULL_POINTER(pointer) \
    DML_BAD_ARGUMENT_RETURN( NULL == (pointer), DML_STATUS_NULL_POINTER_ERROR);


#ifdef __cplusplus
}
#endif

#endif //DML_OWN_HARDWARE_CHECKERS_H__
