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
 * @brief Contains a
 * @date 3/19/2020
 *
 */


#include "own_dml_definitions.h"

#define DML_DEFAULT_ALIGNMENT 64u

OWN_FUN_INLINE (int64_t, DML_INT_PTR,( const void* ptr ))
{
    union
    {
        void    *pointer_ptr;
        int64_t long_signed_int;
    } dd;

    dd.pointer_ptr = (void *) ptr;

    return dd.long_signed_int;
}

OWN_FUN_INLINE (uint64_t, DML_UINT_PTR,( const void* ptr ))
{
    union
    {
        void     *ptr;
        uint64_t long_unsigned_int;
    } dd;

    dd.long_unsigned_int = (uint64_t) ((void *) ptr);

    return dd.long_unsigned_int;
}

#define DML_BYTES_TO_ALIGN(ptr, align) ((-(idml_##DML_INT_PTR(ptr)&((align)-1u)))&((align)-1u))

#define DML_ALIGNED_PTR(ptr, align) (void*)( (uint8_t *)(ptr) + (DML_BYTES_TO_ALIGN( ptr, align )) )

#define DML_ALIGNED_SIZE(size, align) (((size)+(align)-1u)&~((align)-1u))
