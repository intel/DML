/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @date
 *
 * @details  Master include file for Intel(R) Data Mover Library (Intel® DML)
 * Contains all public DML API definitions
 *
 */

/**
 * @defgroup dml_job_public_features Public Features
 * @ingroup dml_job_public
 * @{
 *
 * @brief Public functions for Intel(R) Data Mover Library (Intel® DML)
 */

#ifndef DML_H__
#define DML_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "dmldefs.h"

/**
 * @brief Returns state of the current build
 *
 * @return Pointer to @ref dml_library_version_t, which contains information about the current build
 *
 */
const dml_library_version_t *dml_get_library_version();

/**
 * @brief Calculates the amount of memory, in bytes, required for the @ref dml_job_t structure.
 * Some kind of dispatching is done at this stage - different paths may require different amount of memory
 *
 * @param[in] dml_path          type of implementation path to use:
 *                                  - @ref DML_PATH_AUTO,
 *                                  - @ref DML_PATH_HW
 *                                  - @ref DML_PATH_SW
 * @param[out] job_size_ptr     a pointer to uint32_t where to store the @ref dml_job_t size (in bytes)
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 * - @ref DML_STATUS_OK
 * - @ref DML_STATUS_PATH_ERROR
 * - @ref DML_STATUS_NULL_POINTER_ERROR
 *
 */
dml_status_t dml_get_job_size(dml_path_t dml_path, uint32_t *job_size_ptr);

/**
 * @brief Performs dml_job_t structure initialization.
 *
 * Memory for dml_job_t structure must be allocated at the application side. Size (in bytes)
 * must be obtained with @ref dml_get_job_size() function and then type-casted to dml_job_t*:
 *
 * @code
 *  uint32_t  size;
 *  dml_job_t *dml_job_ptr;
 *  status = dml_get_job_size( &size );
 *  dml_job_ptr = (dml_job_t*)malloc( size );
 * @endcode
 *
 * @ref dml_job_t may contain additional internal memory buffers for SW or/and HW paths (descriptor, completion record, queue (libaccel-config).
 *
 * @param[in] path              type of implementation path to use:
 *                                  - @ref DML_PATH_AUTO,
 *                                  - @ref DML_PATH_HW
 *                                  - @ref DML_PATH_SW
 * @param[in,out] dml_job_ptr   a pointer to @ref dml_job_t structure
 *
 * @remark Usage example: @ref JOB_API_INIT_DML_JOB_EXAMPLE
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value otherwise
 * Return values:
 * - @ref DML_STATUS_OK
 * - @ref DML_STATUS_PATH_ERROR
 * - @ref DML_STATUS_NULL_POINTER_ERROR
 * - @ref DML_STATUS_LIBACCEL_NOT_FOUND
 * - @ref DML_STATUS_LIBACCEL_ERROR
 *
 */
dml_status_t dml_init_job(dml_path_t path, dml_job_t *dml_job_ptr);

/**
 * @brief Frees all necessary resources that are used by initialized @ref dml_job_t
 *
 * @param[in,out] dml_job_ptr   a pointer to @ref dml_job_t structure
 *
 * @remark Usage example: @ref JOB_API_FINALIZE_DML_JOB_EXAMPLE
 *
 * @return The follow statuses;
 *      - @ref DML_STATUS_OK;
 *
 */
dml_status_t dml_finalize_job(dml_job_t *dml_job_ptr);

/**
 * @brief Performs dml_job_t structure parsing and forming the corresponding processing functions pipeline.
 *
 * @param[in,out] dml_job_ptr   Pointer to the initialized @ref dml_job_t structure
 *
 * @remark Usage example: @ref JOB_API_EXECUTE_DML_JOB_EXAMPLE
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value otherwise in accordance with specified operation
 * Return values:
 * - @ref DML_STATUS_OK;
 * - @ref DML_STATUS_NULL_POINTER_ERROR;
 * - @ref DML_STATUS_JOB_CORRUPTED;
 * - or other status depending on the DML operation in the @ref dml_job_t.operation field.
 *
 */
dml_status_t dml_execute_job(dml_job_t *dml_job_ptr);

/**
 * @brief Performs @ref dml_job_t structure parsing and forming the corresponding processing functions pipeline.
 * An alias for ExecuteJob in case of software path.
 *
 * @param[in,out] dml_job_ptr   Pointer to the initialized @ref dml_job_t structure
 *
 * @remark Usage example: @ref JOB_API_SUBMIT_DML_JOB_EXAMPLE
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value otherwise in accordance with specified operation
 * Return values:
 * - @ref DML_STATUS_OK;
 * - @ref DML_STATUS_NULL_POINTER_ERROR;
 * - @ref DML_STATUS_JOB_CORRUPTED;
 * - or other status depending on the DML operation in the @ref dml_job_t.operation field.
 *
 */
dml_status_t dml_submit_job(dml_job_t *dml_job_ptr);

/**
 * @brief Waits for the end of @ref dml_job_t processing. (waits until the job is completed)
 *
 * @param[in,out] dml_job_ptr   Pointer to the initialized @ref dml_job_t structure
 *
 * @remark Usage example: @ref JOB_API_SUBMIT_DML_JOB_EXAMPLE
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value otherwise
 * Return values:
 * - @ref DML_STATUS_OK
 * - or other status depending on the DML operation in the @ref dml_job_t.operation field
 *
 */
dml_status_t dml_wait_job(dml_job_t *dml_job_ptr);

/**
 * @brief Checks the status of @ref dml_job_t processing.
 * (Can be queried periodically to check the status of dml_submit_job)
 *
 * @param[in,out] dml_job_ptr    Pointer to the initialized @ref dml_job_t structure
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value otherwise
 * Return values:
 * - @ref DML_STATUS_OK
 * - @ref DML_STATUS_BEING_PROCESSED
 * - @ref DML_STATUS_JOB_CORRUPTED
 *
 */
dml_status_t dml_check_job(dml_job_t *dml_job_ptr);

/**
 * @brief The service function that returns the maximum number of jobs available in a batch mode.
 *
 * @param[in]  dml_job_ptr       Pointer to the initialized @ref dml_job_t structure
 * @param[out] dml_limits_ptr    Pointer to the dml_limits_t structure where to return the result
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_JOB_CORRUPTED
 *
 */
dml_status_t dml_get_limits(dml_job_t *dml_job_ptr, dml_limits_t *dml_limits_ptr);

/**
 * @brief The service function that returns the number of bytes to initialize a batch.
 *
 * @param[in]  dml_job_ptr       Pointer to the initialized @ref dml_job_t structure
 * @param[in]  task_count        Desired number of tasks in a batch
 * @param[out] byte_size_ptr     Number of bytes for memory allocation
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_JOB_OPERATION_ERROR
 *      - @ref DML_STATUS_BATCH_SIZE_ERROR
 *
 */
dml_status_t dml_get_batch_size(const dml_job_t *dml_job_ptr, uint32_t task_count, uint32_t *byte_size_ptr);

/**
 * @brief The service function that sets the specific task for the @ref DML_OP_NOP operation.
 *
 * @param[in]  dml_job_ptr       Pointer to the initialized @ref dml_job_t structure
 * @param[in]  task_index        Index of the desired batch task
 * @param[in]  flags             Specific operation flags
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_BATCH_TASK_INDEX_OVERFLOW
 *
 */
dml_status_t dml_batch_set_nop_by_index(dml_job_t *dml_job_ptr, uint32_t task_index, dml_operation_flags_t flags);

/**
 * @brief The service function that sets the specific task for the @ref DML_OP_MEM_MOVE operation.
 *
 * @param[in]  dml_job_ptr       Pointer to the initialized @ref dml_job_t structure
 * @param[in]  task_index        Index of the desired batch task
 * @param[in]  source_ptr        Memory region to copy
 * @param[out] destination_ptr   Destination memory region
 * @param[in]  byte_length       Number of bytes to copy
 * @param[in]  flags             Specific operation flags
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_BATCH_TASK_INDEX_OVERFLOW
 *
 */
dml_status_t dml_batch_set_mem_move_by_index(dml_job_t            *dml_job_ptr,
                                             uint32_t              task_index,
                                             uint8_t              *source_ptr,
                                             uint8_t              *destination_ptr,
                                             uint32_t              byte_length,
                                             dml_operation_flags_t flags);

/**
 * @brief The service function that sets the specific task for the @ref DML_OP_DUALCAST operation.
 *
 * @param[in]  dml_job_ptr              Pointer to the initialized @ref dml_job_t structure
 * @param[in]  task_index               Index of the desired batch task
 * @param[in]  source_ptr               Memory region to copy
 * @param[out] destination_first_ptr    First destination memory region
 * @param[out] destination_second_ptr   Second destination memory region
 * @param[in]  byte_length              Number of bytes to copy
 * @param[in]  flags                    Specific operation flags
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_BATCH_TASK_INDEX_OVERFLOW
 *
 */
dml_status_t dml_batch_set_dualcast_by_index(dml_job_t            *dml_job_ptr,
                                             uint32_t              task_index,
                                             uint8_t              *source_ptr,
                                             uint8_t              *destination_first_ptr,
                                             uint8_t              *destination_second_ptr,
                                             uint32_t              byte_length,
                                             dml_operation_flags_t flags);

/**
 * @brief The service function that sets the specific task for the @ref DML_OP_COMPARE operation.
 *
 * @param[in]  dml_job_ptr         Pointer to the initialized @ref dml_job_t structure
 * @param[in]  task_index          Index of the desired batch task
 * @param[in]  source_first_ptr    First memory region to compare
 * @param[in]  source_second_ptr   First memory region to compare
 * @param[in]  byte_length         Number of bytes to copy
 * @param[in]  expected_result     Expected result of the operation
 * @param[in]  flags               Specific operation flags
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_BATCH_TASK_INDEX_OVERFLOW
 *
 */
dml_status_t dml_batch_set_compare_by_index(dml_job_t            *dml_job_ptr,
                                            uint32_t              task_index,
                                            uint8_t              *source_first_ptr,
                                            uint8_t              *source_second_ptr,
                                            uint32_t              byte_length,
                                            dml_meta_result_t     expected_result,
                                            dml_operation_flags_t flags);

/**
 * @brief The service function that sets the specific task for the @ref DML_OP_COMPARE_PATTERN operation.
 *
 * @param[in]  dml_job_ptr         Pointer to the initialized @ref dml_job_t structure
 * @param[in]  task_index          Index of the desired batch task
 * @param[in]  source_ptr          Memory region to compare
 * @param[in]  pattern_ptr         Pointer to the pattern to compare
 * @param[in]  byte_length         Number of bytes to copy
 * @param[in]  expected_result     Expected result of the operation
 * @param[in]  flags               Specific operation flags
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_BATCH_TASK_INDEX_OVERFLOW
 *
 */
dml_status_t dml_batch_set_compare_pattern_by_index(dml_job_t            *dml_job_ptr,
                                                    uint32_t              task_index,
                                                    uint8_t              *source_ptr,
                                                    uint8_t              *pattern_ptr,
                                                    uint32_t              byte_length,
                                                    dml_meta_result_t     expected_result,
                                                    dml_operation_flags_t flags);

/**
 * @brief The service function that sets the specific task for the @ref DML_OP_CRC operation.
 *
 * @param[in]  dml_job_ptr         Pointer to the initialized @ref dml_job_t structure
 * @param[in]  task_index          Index of the desired batch task
 * @param[in]  source_ptr          Memory region for the operation
 * @param[in]  byte_length         Number of bytes to copy
 * @param[in]  crc_seed_ptr        Pointer to the initial crc value
 * @param[in]  flags               Specific operation flags
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_BATCH_TASK_INDEX_OVERFLOW
 *
 */
dml_status_t dml_batch_set_crc_by_index(dml_job_t            *dml_job_ptr,
                                        uint32_t              task_index,
                                        uint8_t              *source_ptr,
                                        uint32_t              byte_length,
                                        uint32_t             *crc_seed_ptr,
                                        dml_operation_flags_t flags);

/**
 * @brief The service function that sets the specific task for the @ref DML_OP_COPY_CRC operation.
 *
 * @param[in]  dml_job_ptr         Pointer to the initialized @ref dml_job_t structure
 * @param[in]  task_index          Index of the desired batch task
 * @param[in]  source_ptr          Memory region for the operation
 * @param[in]  byte_length         Number of bytes to copy
 * @param[in]  crc_seed_ptr        Pointer to the initial crc value
 * @param[out] destination_ptr     Destination memory region
 * @param[in]  flags               Specific operation flags
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_BATCH_TASK_INDEX_OVERFLOW
 *
 */
dml_status_t dml_batch_set_copy_crc_by_index(dml_job_t            *dml_job_ptr,
                                             uint32_t              task_index,
                                             uint8_t              *source_ptr,
                                             uint32_t              byte_length,
                                             uint32_t             *crc_seed_ptr,
                                             uint8_t              *destination_ptr,
                                             dml_operation_flags_t flags);

/**
 * @brief The service function that sets the specific task for the @ref DML_OP_FILL operation.
 *
 * @param[in]  dml_job_ptr         Pointer to the initialized @ref dml_job_t structure
 * @param[in]  task_index          Index of the desired batch task
 * @param[in]  pattern_ptr         Pointer to the pattern to compare
 * @param[out] destination_ptr     Destination memory region
 * @param[in]  byte_length         Number of bytes to fill
 * @param[in]  flags               Specific operation flags
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_BATCH_TASK_INDEX_OVERFLOW
 *
 */
dml_status_t dml_batch_set_fill_by_index(dml_job_t            *dml_job_ptr,
                                         uint32_t              task_index,
                                         const uint8_t        *pattern_ptr,
                                         uint8_t              *destination_ptr,
                                         uint32_t              byte_length,
                                         dml_operation_flags_t flags);

/**
 * @brief The service function that sets the specific task for the @ref DML_OP_CACHE_FLUSH operation.
 *
 * @param[in]  dml_job_ptr         Pointer to the initialized @ref dml_job_t structure
 * @param[in]  task_index          Index of the desired batch task
 * @param[out] destination_ptr     Destination memory region
 * @param[in]  byte_length         Number of bytes in the destination vector
 * @param[in]  flags               Specific operation flags
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_BATCH_TASK_INDEX_OVERFLOW
 *
 */
dml_status_t dml_batch_set_cache_flush_by_index(dml_job_t            *dml_job_ptr,
                                                uint32_t              task_index,
                                                uint8_t              *destination_ptr,
                                                uint32_t              byte_length,
                                                dml_operation_flags_t flags);

/**
 * @brief The service function that sets the specific task for the @ref DML_OP_DELTA_CREATE operation.
 *
 * @param[in]  dml_job_ptr         Pointer to the initialized @ref dml_job_t structure
 * @param[in]  task_index          Index of the desired batch task
 * @param[in]  source_ptr          Source vector to compare
 * @param[in]  reference_ptr       Reference vector to compare
 * @param[in]  compare_length      Byte length to compare
 * @param[out] delta_record_ptr    Pointer to the delta record output buffer
 * @param[in]  delta_record_length Number of bytes in the delta record output buffer
 * @param[in]  expected_result     Expected result of the operation
 * @param[in]  flags               Specific operation flags
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_BATCH_TASK_INDEX_OVERFLOW
 *
 */
dml_status_t dml_batch_set_delta_create_by_index(dml_job_t            *dml_job_ptr,
                                                 uint32_t              task_index,
                                                 uint8_t              *source_ptr,
                                                 uint8_t              *reference_ptr,
                                                 uint32_t              compare_length,
                                                 uint8_t              *delta_record_ptr,
                                                 uint32_t              delta_record_length,
                                                 dml_meta_result_t     expected_result,
                                                 dml_operation_flags_t flags);

/**
 * @brief The service function that sets the specific task for the @ref DML_OP_DELTA_APPLY operation.
 *
 * @param[in]  dml_job_ptr         Pointer to the initialized @ref dml_job_t structure
 * @param[in]  task_index          Index of the desired batch task
 * @param[in]  delta_record_ptr    Pointer to the existing delta record
 * @param[in]  delta_record_length Number of bytes in the existing delta record
 * @param[out] destination_ptr     Vector to update
 * @param[in]  destination_length  Number of bytes in the vector to update
 * @param[in]  flags               Specific operation flags
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_BATCH_TASK_INDEX_OVERFLOW
 *
 */
dml_status_t dml_batch_set_delta_apply_by_index(dml_job_t            *dml_job_ptr,
                                                uint32_t              task_index,
                                                uint8_t              *delta_record_ptr,
                                                uint32_t              delta_record_length,
                                                uint8_t              *destination_ptr,
                                                uint32_t              destination_length,
                                                dml_operation_flags_t flags);

/**
 * @brief The service function that sets the specific task for the @ref DML_OP_DIF_CHECK operation.
 *
 * @param[in]  dml_job_ptr         Pointer to the initialized @ref dml_job_t structure
 * @param[in]  task_index          Index of the desired batch task
 * @param[in]  source_ptr          Vector to check
 * @param[in]  source_length       Byte length of the source vector
 * @param[in]  dif_config_ptr      Pointer to the DIF config
 * @param[in]  flags               Specific operation flags
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_BATCH_TASK_INDEX_OVERFLOW
 *
 */
dml_status_t dml_batch_set_dif_check_by_index(dml_job_t              *dml_job_ptr,
                                              uint32_t                task_index,
                                              uint8_t                *source_ptr,
                                              uint32_t                source_length,
                                              const dml_dif_config_t *dif_config_ptr,
                                              dml_operation_flags_t   flags);

/**
 * @brief The service function that sets the specific task for the @ref DML_OP_DIF_UPDATE operation.
 *
 * @param[in]  dml_job_ptr         Pointer to the initialized @ref dml_job_t structure
 * @param[in]  task_index          Index of the desired batch task
 * @param[in]  source_ptr          Vector to copy
 * @param[in]  source_length       Byte length of the source vector
 * @param[in]  dif_config_ptr      Pointer to the DIF config
 * @param[out] destination_ptr     Output buffer
 * @param[in]  destination_length  Byte length of the destination vector
 * @param[in]  flags               Specific global flags
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_BATCH_TASK_INDEX_OVERFLOW
 *
 */
dml_status_t dml_batch_set_dif_update_by_index(dml_job_t              *dml_job_ptr,
                                               uint32_t                task_index,
                                               uint8_t                *source_ptr,
                                               uint32_t                source_length,
                                               const dml_dif_config_t *dif_config_ptr,
                                               uint8_t                *destination_ptr,
                                               uint32_t                destination_length,
                                               dml_operation_flags_t   flags);

/**
 * @brief The service function that sets the specific task for the @ref DML_OP_DIF_INSERT operation.
 *
 * @param[in]  dml_job_ptr         Pointer to the initialized @ref dml_job_t structure
 * @param[in]  task_index          Index of the desired batch task
 * @param[in]  source_ptr          Vector to copy
 * @param[in]  source_length       Byte length of the source vector
 * @param[in]  dif_config_ptr      Pointer to the DIF config
 * @param[out] destination_ptr     Output buffer
 * @param[in]  destination_length  Byte length of the destination vector
 * @param[in]  flags               Specific global flags
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_BATCH_TASK_INDEX_OVERFLOW
 *
 */
dml_status_t dml_batch_set_dif_insert_by_index(dml_job_t              *dml_job_ptr,
                                               uint32_t                task_index,
                                               uint8_t                *source_ptr,
                                               uint32_t                source_length,
                                               const dml_dif_config_t *dif_config_ptr,
                                               uint8_t                *destination_ptr,
                                               uint32_t                destination_length,
                                               dml_operation_flags_t   flags);

/**
 * @brief The service function that sets the specific task for the @ref DML_OP_DIF_STRIP operation.
 *
 * @param[in]  dml_job_ptr         Pointer to the initialized @ref dml_job_t structure
 * @param[in]  task_index          Index of the desired batch task
 * @param[in]  source_ptr          Vector to copy
 * @param[in]  source_length       Byte length of the source vector
 * @param[in]  dif_config_ptr      Pointer to the DIF config
 * @param[out] destination_ptr     Output buffer
 * @param[in]  destination_length  Byte length of the destination vector
 * @param[in]  flags               Specific global flags
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_BATCH_TASK_INDEX_OVERFLOW
 *
 */
dml_status_t dml_batch_set_dif_strip_by_index(dml_job_t              *dml_job_ptr,
                                              uint32_t                task_index,
                                              uint8_t                *source_ptr,
                                              uint32_t                source_length,
                                              const dml_dif_config_t *dif_config_ptr,
                                              uint8_t                *destination_ptr,
                                              uint32_t                destination_length,
                                              dml_operation_flags_t   flags);

/**
 * @brief The service function that gets an access to the result.
 *
 * @param[in]  dml_job_ptr   Pointer to the initialized @ref dml_job_t structure
 * @param[in]  task_index    Index of the desired batch task
 * @param[out] result_ptr    Output buffer
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_BATCH_TASK_INDEX_OVERFLOW
 *
 */
dml_status_t dml_batch_get_result(const dml_job_t *dml_job_ptr, uint32_t task_index, dml_meta_result_t *result_ptr);

/**
 * @brief The service function that gets an access to the status.
 *
 * @param[in]  dml_job_ptr    Pointer to the initialized @ref dml_job_t structure
 * @param[in]  task_index     Index of the desired batch task
 * @param[out] status_ptr     Output buffer
 *
 * @return @ref DML_STATUS_OK in case of success execution, or non-zero value, otherwise
 * Return values:
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_BATCH_TASK_INDEX_OVERFLOW
 *
 */
dml_status_t dml_batch_get_status(const dml_job_t *dml_job_ptr, uint32_t task_index, dml_status_t *status_ptr);

#ifdef __cplusplus
}
#endif

#endif /* //DML_H__ */

/** @} */
