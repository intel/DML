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
 *      - @ref dmlc_compare_8u()
 *      - @ref dmlc_compare_pattern_8u()
 *
 * @date 2/10/2020
 *
 */

#include "core_compare.h"
#include "own_dmlc_definitions.h"
#if defined(AVX512)
#include "avx512/dmlc_compare_8u_k0.cxx"
#else
#include "default/dmlc_compare_8u_px.cxx"
#endif


DML_CORE_API(dmlc_status_t, compare_8u, (const uint8_t* first_vector_ptr,
    const uint8_t* second_vector_ptr,
    const uint32_t size,
    uint32_t* const mismatch_offset_ptr))
{
    return dmlc_own_compare_8u(first_vector_ptr, second_vector_ptr, size, mismatch_offset_ptr);
}

DML_CORE_API(dmlc_status_t, compare_with_pattern_8u, (const uint8_t *memory_region_ptr,
                                                      const pattern_t pattern,
                                                      const uint32_t size,
                                                      uint32_t *const mismatch_offset_ptr))
{
    return dmlc_own_compare_with_pattern_8u(memory_region_ptr, pattern, size, mismatch_offset_ptr);
}
