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
 * @brief Contain implementation of the follow functions:
 *      - @ref dmlc_copy_forward_8u()
 *      - @ref dmlc_copy_backward_8u()
 *      - @ref dmlc_dualcast_copy_8u()
 *
 * @date 2/20/2020
 *
 */


#include "core_memory.h"
#include "own_dmlc_definitions.h"
#if defined(AVX512)
#include "avx512/dmlc_copy_8u_k0.cxx"
#else
#include "default/dmlc_copy_8u_px.cxx"
#endif

/** Checks 0:11 bits for equality **/
#define OWN_BAD_ARGUMENT_DUALCAST_DST_ALIGNMENT(dst_ptr1, dst_ptr2)         \
        DML_CORE_BAD_ARGUMENT_RETURN( ((((uint64_t) (dst_ptr1)) & 0xFFFu) != \
                                      (((uint64_t) (dst_ptr2)) & 0xFFFu)),  \
                                          DML_STATUS_DUALCAST_ALIGN_ERROR )


DML_CORE_API(dmlc_status_t, copy_8u, ( const uint8_t  *const source_ptr,
                                                     uint8_t  *const destination_ptr,
                                                     uint32_t        bytes_to_process ) )
{
    // Main action
    dmlc_own_copy_8u(source_ptr, destination_ptr, bytes_to_process);

    // Success
    return DML_STATUS_OK;
}


DML_CORE_API(dmlc_status_t, move_8u, ( const uint8_t  *const source_ptr,
                                                      uint8_t  *const destination_ptr,
                                                      uint32_t        bytes_to_process ) )
{
    // Main action
    dmlc_own_move_8u(source_ptr, destination_ptr, bytes_to_process);

    // Success
    return DML_STATUS_OK;
}


 DML_CORE_API(dmlc_status_t, dualcast_copy_8u, ( const uint8_t  *const source_ptr,
                                                       uint8_t  *const first_destination_ptr,
                                                       uint8_t  *const second_destination_ptr,
                                                       uint32_t        bytes_to_process ) )
{
    // Main action
    dmlc_own_dualcast_copy_8u(source_ptr, first_destination_ptr, second_destination_ptr, bytes_to_process);

    // Success
    return DML_STATUS_OK;
}
