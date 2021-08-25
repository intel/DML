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
 *      - @ref dmlc_fill_with_pattern_8u()
 *
 * @date 2/21/2020
 *
 */


#include "core_memory.h"
#include "own_dmlc_definitions.h"

#if defined(AVX512)
    // TODO: I cannot load mask on MSVC17, so I disabled optimizations
    #if (_MSC_VER >= 1928) || defined(__GNUC__)
        #include "avx512/dmlc_fill_8u.cxx"
    #else
        #include "default/dmlc_fill_8u.cxx"
    #endif
#else
    #include "default/dmlc_fill_8u.cxx"
#endif

DML_CORE_API(dmlc_status_t, fill_with_pattern_8u, ( uint64_t        pattern,
                                                     uint8_t  *const memory_region_ptr,
                                                     uint32_t        bytes_to_process ) )
{
    return dmlc_own_opt_fill_with_pattern_8u(pattern, memory_region_ptr, bytes_to_process);
}
