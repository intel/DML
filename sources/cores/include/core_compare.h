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
 * @file
 * @date 2/10/2020
 *
 * @defgroup core_public_compare Compare Features
 * @ingroup core_public_features
 * @{
 *
 * @brief Features to compare memory region with another one or with a pattern.
 *
 * @details Compare group contains optimized cores, which perform the following tasks:
 *	-   Comparing between vectors;
 *	-   Comparing between vector and a pattern;
 *	-   Comparing vector values with some key/range to create a bit mask;
 *	-   Creating delta between two memory regions.
 *
 */

#include "core_definitions.h"

#ifndef DML_KERNEL_COMPARE_H__
#define DML_KERNEL_COMPARE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ------ Kernel Compare Defines ------ */

#define DML_COMPARE_STATUS_EQ DML_STATUS_OK                  /**< Redefinition of default status in context of Compare functions */
#define DML_COMPARE_STATUS_NE DML_STATUS_FALSE_PREDICATE_OK  /**< Redefinition of default status in context of Compare functions */

typedef uint64_t pattern_t;     /**< Special type for 8-byte pattern */
#define DML_SIZE_PATTERN_T  64  /**< pattern_t size in bits */


/* ------ Kernel Compare Functions ------ */

/**
 * @brief Compares specified memory regions.
 *
 * @param[in] first_vector_ptr          pointer to the reference vector
 * @param[in] second_vector_ptr         pointer to the vector to compare
 * @param[in] size                      number of bytes to compare
 * @param[out] mismatch_offset_ptr      first mismatch offset
 *
 * @note No memory alignment required.
 * @note After function execution mismatch_offset_ptr contains the first miss_match offset
 * if vectors are not equal.
 *
 * @return
 *      - @ref DML_COMPARE_STATUS_EQ;
 *      - @ref DML_COMPARE_STATUS_NE;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
DML_CORE_API(dmlc_status_t, compare_8u, (const uint8_t *first_vector_ptr,
                                         const uint8_t *second_vector_ptr,
                                         const uint32_t size,
                                         uint32_t *const mismatch_offset_ptr));


/**
 * @brief Compares specified memory region with 8-byte pattern.
 *
 * @param[in] memory_region_ptr         pointer to the base vector
 * @param[in] pattern                   expected 8-byte memory pattern
 * @param[in] size                      number of bytes to compare
 * @param[out] mismatch_offset_ptr      first mismatch offset
 *
 * @note After function execution mismatch_offset_ptr contains the first miss_match offset
 * if vectors are not equal.
 * @note Mismatch_offset_ptr may not be the exact byte location,
 * but it is guaranteed to be no greater than the first difference.
 *
 * @return
 *      - @ref DML_COMPARE_STATUS_EQ;
 *      - @ref DML_COMPARE_STATUS_NE;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
DML_CORE_API(dmlc_status_t, compare_with_pattern_8u, (const uint8_t *memory_region_ptr,
                                                      const pattern_t pattern,
                                                      const uint32_t size,
                                                      uint32_t *const mismatch_offset_ptr));

/**
 * @brief Creates delta record if vectors are not equal
 *
 * @param[in]  reference_vector_ptr    pointer to the base vector
 * @param[in]  second_vector_ptr       pointer to the delta that is written into the delta record
 * @param[in]  compared_bytes          number of bytes to compare
 * @param[in]  delta_record_max_size   maximal delta record size
 * @param[out] delta_record_ptr        pointer to the delta record
 * @param[out] record_size_ptr         created delta record size
 *
 * @warning: Compared vectors addresses must be aligned to a multiple of 8.
 * @warning: Number of bytes to compare must be multiple of 8.
 * @warning: Number of bytes to compare must be less or equal to the maximum supported offset,
 *           which is 524,280 bytes (0x7FFF8).
 * @warning: Number of available bytes in delta record must be multiple of 10.
 *
 * @return
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_DELTA_ALIGN_ERROR in case if vector address is not aligned to be a
 *             multiple of 8;
 *      - @ref DML_STATUS_DELTA_INPUT_SIZE_ERROR in case if input vector size is not multiple of 8, or
 *             in case if input vector size is greater then max delta offset supported;
 *      - @ref DML_STATUS_DELTA_RECORD_SIZE_ERROR in case if max_delta_record_size is not sufficient
 *             for delta record creation, or max_delta_record_size is not a multiple of 10u
 *
 */
DML_CORE_API(dmlc_status_t, create_delta_record_8u, (const uint8_t *reference_vector_ptr,
                                                    const uint8_t *second_vector_ptr,
                                                    const uint32_t compared_bytes,
                                                    const uint32_t delta_record_max_size,
                                                    uint8_t* delta_record_ptr,
                                                    uint32_t *const record_size_ptr));

/**
 * @brief Applies delta record to the contents of memory at destination address
 *
 * @param[out] memory_region_ptr    pointer to a memory region that is updated with a delta
 * @param[in]  delta_record_ptr     pointer to a delta record
 * @param[in]  memory_region_size   destination size
 * @param[in]  delta_record_size    delta record size
 *
 * @warning Memory region byte size must be multiply of 8.
 * @warning Delta record byte size must be multiply of 10.
 * @warning Function does not support vectors' overlap.
 * @warning Maximal supported offset is 524,280 bytes (0x7FFF8).
 *
 * @return
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_DELTA_ALIGN_ERROR in case if memory_region_ptr address is not aligned a
 *             multiple of 8;
 *      - @ref DML_STATUS_DELTA_INPUT_SIZE_ERROR in case if memory region size is not multiple of 8;
 *      - @ref DML_STATUS_DELTA_RECORD_SIZE_ERROR in case if delta record size is not multiple of 10;
 *      - @ref DML_STATUS_OVERLAPPING_BUFFER_ERROR in case if vectors overlap
 *      - @ref DML_STATUS_MEMORY_OVERFLOW_ERROR in case if offset is greater than memory region size
 */
DML_CORE_API(dmlc_status_t, apply_delta_record_8u, (uint8_t * memory_region_ptr,
                                                   const uint8_t *delta_record_ptr,
                                                   const uint32_t memory_region_size,
                                                   const uint32_t delta_record_size));

#ifdef __cplusplus
}
#endif

#endif //DML_KERNEL_COMPARE_H__
/** @} */
