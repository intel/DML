/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @date 3/16/2020
 *
 * @defgroup HW_PATH_DESCRIPTOR_API Descriptors fillers API
 * @ingroup HW_PATH
 * @{
 * @brief Contains functions, which are used to fill DSA descriptor
 */
#include "hardware_definitions.h"

#ifndef DSA_HARDWARE_DESCRIPTORS_API_H__
#define DSA_HARDWARE_DESCRIPTORS_API_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Fills a descriptor for the @ref DML_OP_MEM_MOVE operation
 *
 * @param[out] descriptor_ptr  pointer to @ref dsahw_descriptor_t
 * @param[in]  source_ptr      pointer to the memory to copy
 * @param[in]  source_length   length of the memory to copy
 * @param[out] destination_ptr pointer to the memory where to copy
 * @param[in]  flags           specific operation flags
 * @param[in]  result_ptr      pointer to the memory that will contain operation result after descriptor execution
 *
 * @return The following statuses:
 * @todo add return statuses
 */
dsahw_status_t DML_HW_API(init_mem_move_descriptor)(dsahw_descriptor_t        *descriptor_ptr,
                                                    const uint8_t             *source_ptr,
                                                    uint32_t                   source_length,
                                                    uint8_t                   *destination_ptr,
                                                    dml_operation_flags_t      flags,
                                                    dsahw_completion_record_t *result_ptr);

/**
 * @brief Fills a descriptor for the @ref DML_OP_FILL operation
 *
 * @param[out] descriptor_ptr     pointer to @ref dsahw_descriptor_t
 * @param[in]  pattern_ptr        pointer to the pattern
 * @param[out] destination_ptr    pointer to the memory to fill
 * @param[in]  destination_length length of the memory to fill
 * @param[in]  flags              specific operation flags
 * @param[in]  result_ptr         pointer to the memory that will contain operation result after descriptor execution
 *
 * @return The following statuses:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
dsahw_status_t DML_HW_API(init_fill_descriptor)(dsahw_descriptor_t        *descriptor_ptr,
                                                uint64_t                  *pattern_ptr,
                                                uint8_t                   *destination_ptr,
                                                uint32_t                   destination_length,
                                                dml_operation_flags_t      flags,
                                                dsahw_completion_record_t *result_ptr);

/**
 * @brief Fills a descriptor for the @ref DML_OP_COMPARE operation
 *
 * @param[out] descriptor_ptr    pointer to @ref dsahw_descriptor_t
 * @param[in]  source_first_ptr  pointer to the first vector to compare
 * @param[in]  source_second_ptr pointer to the second vector to compare
 * @param[in]  source_length     length of the memory to compare
 * @param[in]  expected_result   expected result of the operation
 * @param[in]  flags             specific operation flags
 * @param[in]  result_ptr        pointer to the memory that will contain operation result after descriptor execution
 *
 * @return The following statuses:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
dsahw_status_t DML_HW_API(init_compare_descriptor)(dsahw_descriptor_t        *descriptor_ptr,
                                                   const uint8_t             *source_first_ptr,
                                                   const uint8_t             *source_second_ptr,
                                                   uint32_t                   source_length,
                                                   dml_meta_result_t          expected_result,
                                                   dml_operation_flags_t      flags,
                                                   dsahw_completion_record_t *result_ptr);

/**
 * @brief Fills a descriptor for the @ref DML_OP_COMPARE_PATTERN operation
 *
 * @param[out] descriptor_ptr  pointer to @ref dsahw_descriptor_t
 * @param[in]  source_ptr      pointer to the memory to compare
 * @param[in]  source_length   length of the memory to compare
 * @param[in]  pattern_ptr     pointer to the pattern to compare
 * @param[in]  expected_result expected result of the operation
 * @param[in]  flags           specific operation flags
 * @param[in]  result_ptr      pointer to the memory that will contain operation result after descriptor execution
 *
 * @return The following statuses:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
dsahw_status_t DML_HW_API(init_compare_pattern_descriptor)(dsahw_descriptor_t        *descriptor_ptr,
                                                           const uint8_t             *source_ptr,
                                                           uint32_t                   source_length,
                                                           const uint64_t            *pattern_ptr,
                                                           dml_meta_result_t          expected_result,
                                                           dml_operation_flags_t      flags,
                                                           dsahw_completion_record_t *result_ptr);

/**
 * @brief Fills a descriptor for the @ref DML_OP_DELTA_CREATE operation
 *
 * @param[out] descriptor_ptr          pointer to @ref dsahw_descriptor_t
 * @param[in]  source_first_ptr        pointer to the first vector to compare
 * @param[in]  source_second_ptr       pointer to the second vector to compare
 * @param[in]  source_length           length of the memory to compare
 * @param[out] delta_record_ptr        pointer to the result of the operation
 * @param[in]  max_delta_record_length maximal length of the output buffer
 * @param[in]  expected_result         expected result of the operation
 * @param[in]  flags                   specific operation flags
 * @param[in]  result_ptr              pointer to the memory that will contain operation result after descriptor execution
 *
 * @return The following statuses:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
dsahw_status_t DML_HW_API(init_delta_create_descriptor)(dsahw_descriptor_t        *descriptor_ptr,
                                                        uint8_t                   *source_first_ptr,
                                                        uint8_t                   *source_second_ptr,
                                                        uint32_t                   source_length,
                                                        uint8_t                   *delta_record_ptr,
                                                        uint32_t                   max_delta_record_length,
                                                        dml_meta_result_t          expected_result,
                                                        dml_operation_flags_t      flags,
                                                        dsahw_completion_record_t *result_ptr);

/**
 * @brief Fills a descriptor for the @ref DML_OP_DELTA_APPLY operation
 *
 * @param[out]    descriptor_ptr      pointer to @ref dsahw_descriptor_t
 * @param[in]     delta_record_ptr    pointer to the created delta record
 * @param[in]     delta_record_length length of the created delta record
 * @param[in,out] destination_ptr     pointer to the memory to fix
 * @param[in]     destination_length  length of the destination vector
 * @param[in]     flags               specific operation flags
 * @param[in]     result_ptr          pointer to the memory that will contain operation result after descriptor execution
 *
 * @return The following statuses:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
dsahw_status_t DML_HW_API(init_delta_apply_descriptor)(dsahw_descriptor_t        *descriptor_ptr,
                                                       uint8_t                   *delta_record_ptr,
                                                       uint32_t                   delta_record_length,
                                                       uint8_t                   *destination_ptr,
                                                       uint32_t                   destination_length,
                                                       dml_operation_flags_t      flags,
                                                       dsahw_completion_record_t *result_ptr);

/**
 * @brief Fills a descriptor for the @ref DML_OP_DUALCAST operation
 *
 * @param[out] descriptor_ptr         pointer to @ref dsahw_descriptor_t
 * @param[in]  source_ptr             pointer to the memory to copy
 * @param[in]  source_length          length of the memory to copy
 * @param[out] destination_first_ptr  pointer to the first destination vector
 * @param[out] destination_second_ptr pointer to the second destination vector
 * @param[in]  flags                  specific operation flags
 * @param[in]  result_ptr             pointer to the memory that will contain operation result after descriptor execution
 *
 * @return The following statuses:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
dsahw_status_t DML_HW_API(init_dualcast_descriptor)(dsahw_descriptor_t        *descriptor_ptr,
                                                    const uint8_t             *source_ptr,
                                                    uint32_t                   source_length,
                                                    uint8_t                   *destination_first_ptr,
                                                    uint8_t                   *destination_second_ptr,
                                                    dml_operation_flags_t      flags,
                                                    dsahw_completion_record_t *result_ptr);

/**
 * @brief Fills a descriptor for @ref DML_OP_CRC operation
 *
 * @param[out] descriptor_ptr   pointer to @ref dsahw_descriptor_t
 * @param[in]  source_ptr       pointer to the memory to calculate crc
 * @param[in]  source_length    length of the source vector
 * @param[in]  crc_checksum_ptr pointer to the crc operation seed
 * @param[in]  flags            specific operation flags
 * @param[in]  result_ptr       pointer to the memory that will contain operation result after descriptor execution
 *
 * @return The following statuses:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
dsahw_status_t DML_HW_API(init_crc_descriptor)(dsahw_descriptor_t        *descriptor_ptr,
                                               uint8_t                   *source_ptr,
                                               uint32_t                   source_length,
                                               uint32_t                  *crc_checksum_ptr,
                                               dml_operation_flags_t      flags,
                                               dsahw_completion_record_t *result_ptr);

/**
 * @brief Fills a descriptor for @ref DML_OP_COPY_CRC operation
 *
 * @param[out] descriptor_ptr   pointer to @ref dsahw_descriptor_t
 * @param[in]  source_ptr       pointer to the memory to calculate crc
 * @param[in]  source_length    length of the source vector
 * @param[in]  crc_checksum_ptr pointer to the crc operation seed
 * @param[out] destination_ptr  pointer to the memory where to copy
 * @param[in]  flags            specific operation flags
 * @param[in]  result_ptr       pointer to the memory that will contain operation result after descriptor execution
 *
 * @return The following statuses:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
dsahw_status_t DML_HW_API(init_crc_copy_descriptor)(dsahw_descriptor_t        *descriptor_ptr,
                                                    uint8_t                   *source_ptr,
                                                    uint32_t                   source_length,
                                                    uint32_t                  *crc_checksum_ptr,
                                                    uint8_t                   *destination_ptr,
                                                    dml_operation_flags_t      flags,
                                                    dsahw_completion_record_t *result_ptr);

/**
 * @brief Fills a descriptor for @ref DML_OP_CACHE_FLUSH operation
 *
 * @param[out] descriptor_ptr       pointer to @ref dsahw_descriptor_t
 * @param[out] memory_region_ptr    pointer to the memory to flush
 * @param[in]  memory_region_length length of the memory to flush
 * @param[in]  flags                specific operation flags
 * @param[in]  result_ptr           pointer to the memory that will contain operation result after descriptor execution
 *
 * @return The following statuses:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
dsahw_status_t DML_HW_API(init_cache_flush_descriptor)(dsahw_descriptor_t        *descriptor_ptr,
                                                       uint8_t                   *memory_region_ptr,
                                                       uint32_t                   memory_region_length,
                                                       dml_operation_flags_t      flags,
                                                       dsahw_completion_record_t *result_ptr);

/**
 * @brief Fills a descriptor for the @ref DML_OP_DIF_CHECK operation
 *
 * @param[out] descriptor_ptr  pointer to @ref dsahw_descriptor_t
 * @param[in]  source_ptr      pointer to the memory to check
 * @param[in]  source_length   length of the memory to check
 * @param[in]  dif_config_ptr  pointer to the dif config
 * @param[in]  flags           specific operation flags
 * @param[in]  result_ptr      pointer to the memory that will contain operation result after descriptor execution
 *
 * @return The following statuses:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_JOB_LENGTH_ERROR;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
dsahw_status_t DML_HW_API(init_check_dif_descriptor)(dsahw_descriptor_t        *descriptor_ptr,
                                                     uint8_t                   *source_ptr,
                                                     uint32_t                   source_length,
                                                     const dml_dif_config_t    *dif_config_ptr,
                                                     dml_operation_flags_t      flags,
                                                     dsahw_completion_record_t *result_ptr);

/**
 * @brief Fills a descriptor for the @ref DML_OP_DIF_INSERT operation
 *
 * @param[out] descriptor_ptr  pointer to @ref dsahw_descriptor_t
 * @param[in]  source_ptr      pointer to the memory to copy
 * @param[in]  source_length   length of the memory to copy
 * @param[in]  dif_config_ptr  pointer to the dif config
 * @param[out] destination_ptr pointer to the output buffer
 * @param[in]  flags           specific operation flags
 * @param[in]  result_ptr      pointer to the memory that will contain operation result after descriptor execution
 *
 * @return The following statuses:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_JOB_LENGTH_ERROR;
 */
dsahw_status_t DML_HW_API(init_insert_dif_descriptor)(dsahw_descriptor_t        *descriptor_ptr,
                                                      uint8_t                   *source_ptr,
                                                      uint32_t                   source_length,
                                                      const dml_dif_config_t    *dif_config_ptr,
                                                      uint8_t                   *destination_ptr,
                                                      dml_operation_flags_t      flags,
                                                      dsahw_completion_record_t *result_ptr);

/**
 * @brief Fills a descriptor for the @ref DML_OP_DIF_STRIP operation
 *
 * @param[out] descriptor_ptr  pointer to @ref dsahw_descriptor_t
 * @param[in]  source_ptr      pointer to the memory to copy
 * @param[in]  source_length   length of the memory to copy
 * @param[in]  dif_config_ptr  pointer to the dif config
 * @param[out] destination_ptr pointer to the output buffer
 * @param[in]  flags           specific operation flags
 * @param[in]  result_ptr      pointer to the memory that will contain operation result after descriptor execution
 *
 * @return The following statuses:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_JOB_LENGTH_ERROR;
 */
dsahw_status_t DML_HW_API(init_strip_dif_descriptor)(dsahw_descriptor_t        *descriptor_ptr,
                                                     uint8_t                   *source_ptr,
                                                     uint32_t                   source_length,
                                                     const dml_dif_config_t    *dif_config_ptr,
                                                     uint8_t                   *destination_ptr,
                                                     dml_operation_flags_t      flags,
                                                     dsahw_completion_record_t *result_ptr);

/**
 * @brief Fills a descriptor for the @ref DML_OP_DIF_UPDATE operation
 *
 * @param[out] descriptor_ptr  pointer to @ref dsahw_descriptor_t
 * @param[in]  source_ptr      pointer to the memory to copy
 * @param[in]  source_length   length of the memory to copy
 * @param[in]  dif_config_ptr  pointer to the dif config
 * @param[out] destination_ptr pointer to the output buffer
 * @param[in]  flags           specific operation flags
 * @param[in]  result_ptr      pointer to the memory that will contain operation result after descriptor execution
 *
 * @return The following statuses:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_JOB_LENGTH_ERROR;
 */
dsahw_status_t DML_HW_API(init_update_dif_descriptor)(dsahw_descriptor_t        *descriptor_ptr,
                                                      uint8_t                   *source_ptr,
                                                      uint32_t                   source_length,
                                                      const dml_dif_config_t    *dif_config_ptr,
                                                      uint8_t                   *destination_ptr,
                                                      dml_operation_flags_t      flags,
                                                      dsahw_completion_record_t *result_ptr);

/**
 * @brief Fills a descriptor for the @ref DML_OP_NOP operation
 *
 * @param[out] descriptor_ptr  pointer to @ref dsahw_descriptor_t
 * @param[in]  flags           specific operation flags
 * @param[in]  result_ptr      pointer to the memory that will contain operation result after descriptor execution
 *
 * @return The following statuses:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
dsahw_status_t DML_HW_API(init_nop_descriptor)(dsahw_descriptor_t        *descriptor_ptr,
                                               dml_operation_flags_t      flags,
                                               dsahw_completion_record_t *result_ptr);

/**
 * @brief Fills a descriptor for the @ref DML_OP_BATCH operation
 *
 * @param[out] descriptor_ptr              pointer to @ref dsahw_descriptor_t
 * @param[in]  internal_descriptors_ptr    pointer to the internal batch descriptors
 * @param[in]  internal_descriptors_count  number of the internal batch descriptors
 * @param[in]  flags                       specific operation flags
 * @param[in]  result_ptr                  pointer to the memory that will contain operation result after descriptor execution
 *
 * @return The following statuses:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
dsahw_status_t DML_HW_API(init_batch_descriptor)(dsahw_descriptor_t        *descriptor_ptr,
                                                 const dsahw_descriptor_t  *internal_descriptors_ptr,
                                                 uint32_t                   internal_descriptors_count,
                                                 dml_operation_flags_t      flags,
                                                 dsahw_completion_record_t *result_ptr);

/**
 * @brief Fills a descriptor for the @ref DML_OP_DRAIN operation
 *
 * @param[out] descriptor_ptr              pointer to @ref dsahw_descriptor_t
 * @param[in]  readback_address1_ptr       the first pointer where HW will issue readbacks
 * @param[in]  readback_address2_ptr       the second pointer where HW will issue readbacks
 * @param[in]  flags                       specific operation flags
 * @param[in]  result_ptr                  pointer to the memory that will contain operation result after descriptor execution
 *
 * @return The following statuses:
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 */
dsahw_status_t DML_HW_API(init_drain_descriptor)(dsahw_descriptor_t        *descriptor_ptr,
                                                 uint64_t                  *readback_address1_ptr,
                                                 uint64_t                  *readback_address2_ptr,
                                                 dml_operation_flags_t      flags,
                                                 dsahw_completion_record_t *result_ptr);

#ifdef __cplusplus
}
#endif

#endif  //DSA_HARDWARE_DESCRIPTORS_API_H__

/** @} */
