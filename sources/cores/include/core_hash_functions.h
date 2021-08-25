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
 * @date 3/5/2020
 *
 * @defgroup core_public_hash Hash Features
 * @ingroup core_public_features
 * @{
 *
 * @brief Features to calculate CRC
 *
 * @details Hash group contains optimized functions that calculate a hash value using
 * different hash algorithms.
 *
 */


#include "core_definitions.h"

#ifndef DML_KERNEL_HASH_H__
#define DML_KERNEL_HASH_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Shift value for extracting next byte for the CRC16 function
 */
#define OWN_CRC16_BYTE_SHIFT ( 8u )

/**
 * @brief Shift value for extracting next byte the for CRC32 function
 */
#define OWN_CRC32_BYTE_SHIFT ( 24u )


/**
 * @brief Calculates CRC16 hash/checksum for a signified memory region
 *
 * @param[in]     memory_region_ptr        address of memory region to hash
 * @param[in]     bytes_to_hash            memory region size, in bytes, to hash
 * @param[in,out] crc_ptr                  CRC seed / result
 * @param[in]     polynomial	           polynomial to XORing
 *
 * @note No memory alignment is required;
 * @note crc_ptr is the initial seed for CRC16 calculation and result storing
 *
 * @return
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
DML_CORE_API(dmlc_status_t, calculate_crc_16u, (const uint8_t  *const memory_region_ptr,
                                                uint32_t bytes_to_hash,
                                                uint16_t *const crc_ptr,
                                                uint16_t polynomial));


/**
 * @brief Calculates CRC32 hash/checksum for a signified memory region
 *
 * @param[in]     memory_region_ptr        address of memory region to hash
 * @param[in]     bytes_to_hash            memory region size, in bytes, to hash
 * @param[in,out] crc_ptr                  CRC seed / result
 * @param[in]     polynomial	           polynomial to XORing
 *
 * @note No memory alignment is required;
 * @note crc_ptr is the initial seed for CRC32 calculation and result storing
 *
 * @return
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
DML_CORE_API(dmlc_status_t, calculate_crc_32u, (const uint8_t *const memory_region_ptr,
                                                uint32_t bytes_to_hash,
                                                uint32_t *const crc_ptr,
                                                uint32_t polynomial));


/**
 * @brief Calculates CRC32 hash/checksum for a signified memory region with reversed bytes bits
 *
 * @param[in]     memory_region_ptr        address of memory region to hash
 * @param[in]     bytes_to_hash            memory region size, in bytes, to hash
 * @param[in,out] crc_ptr                  CRC seed / result
 * @param[in]     polynomial	           polynomial to XORing
 *
 * @note No memory alignment is required;
 * @note crc_ptr is the initial seed for CRC32 calculation and result storing
 *
 * @return
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
DML_CORE_API(dmlc_status_t, calculate_crc_reflected_32u, (const uint8_t *const memory_region_ptr,
                                                          uint32_t bytes_to_hash,
                                                          uint32_t *const crc_ptr,
                                                          uint32_t polynomial ) );


#ifdef __cplusplus
}
#endif

#endif // DML_KERNEL_HASH_H__
/** @} */
