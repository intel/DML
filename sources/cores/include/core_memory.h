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
 * @date 2/20/2020
 *
 * @defgroup core_public_memory Memory Features
 * @ingroup core_public_features
 * @{
 *
 * @brief Features to move or copy memory region or to fill it with a pattern.
 *
 * @details Memory group contains optimized functions that perform the following tasks:
 *	-   Copying the data from source to destination;
 *	-   Movement the data from one memory region into another;
 *	-   Filling vectors with some value, pattern.
 *
 */


 #include "core_definitions.h"

#ifndef DML_KERNEL_MEMORY_H__
#define DML_KERNEL_MEMORY_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Copies bytes from vector to another vector.
 *
 * @param[in]  source_ptr              pointer to source start
 * @param[out] destination_ptr         pointer to destination start
 * @param[in]  bytes_to_process        number of bytes to process
 *
 * @note No memory alignment is required.
 *
 * @return
 *      - @ref DML_STATUS_OK;
 */
DML_CORE_API(dmlc_status_t, copy_8u, ( const uint8_t  *const source_ptr,
                                                      uint8_t  *const destination_ptr,
                                                      uint32_t        bytes_to_process ) );


/**
 * @brief Moves bytes from vector to another vector.
 *
 * @param[in]  source_ptr              pointer to source start
 * @param[out] destination_ptr         pointer to destination start
 * @param[in]  bytes_to_process        count of bytes to process
 *
 * @note No memory alignment is required.
 *
 * @return
 *      - @ref DML_STATUS_OK;
 */
DML_CORE_API(dmlc_status_t, move_8u, (const uint8_t *const source_ptr,
                                               uint8_t *const destination_ptr,
                                               uint32_t bytes_to_process));


/**
 * @brief Copies bytes from vector to two vectors.
 *
 * @param[in]  source_ptr              pointer to source start
 * @param[out] first_destination_ptr   pointer to first destination start
 * @param[out] second_destination_ptr  pointer to second destination start
 * @param[in]  bytes_to_process        number of bytes to process
 *
 * @warning 0:11 bits in destination_first_ptr and destination_second_ptr must be equal.
 * @warning Function does not support vectors' overlap.
 *
 * @return
 *      - @ref DML_STATUS_OK;
 */
 DML_CORE_API(dmlc_status_t, dualcast_copy_8u, (const uint8_t *const source_ptr,
                                                uint8_t *const first_destination_ptr,
                                                uint8_t *const second_destination_ptr,
                                                uint32_t bytes_to_process));


/**
 * @brief Fills the source vector with the value in the pattern field.
 *
 * @param[in]  pattern                 64-bit pattern to fill
 * @param[out] memory_region_ptr       memory region address
 * @param[in]  bytes_to_process        count of bytes to process
 *
 * @note No memory alignment is required.
 *
 * @return
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
DML_CORE_API(dmlc_status_t, fill_with_pattern_8u, (uint64_t pattern,
                                                   uint8_t *const memory_region_ptr,
                                                   uint32_t bytes_to_process));


#ifdef __cplusplus
}
#endif

#endif // DML_KERNEL_MEMORY_H__
/** @} */
