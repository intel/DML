/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @date 3/10/2020
 *
 * @defgroup Examples Intel DML Job API usage examples
 * @{
 * @brief Contains examples of Intel(R) Data Mover Library (Intel® DML) features call.
 *
 */

#include "dml/dmldefs.h"

#ifndef DML_SAMPLES_H__
#define DML_SAMPLES_H__

#define SAMPLES_COUNT 15u /**< Total count of samples to run in main function. */
#define PATTERN_SIZE 8u /**< Size of pattern for the @ref DML_OP_COMPARE_PATTERN and the @ref DML_OP_FILL. */

/* ------ LAUNCHERS ARRAY ------ */

/**
 * @brief Array of launchers
 *
 * @param dml_job_ptr  pointer to @ref dml_job_t structure
 *
 * @return @ref dml_status_t.
 */
extern dml_status_t (*wrapper_launchers[SAMPLES_COUNT])(dml_job_t *const dml_job_ptr);

/* ------ INTEL DML INIT FUNCTIONS ------ */

/**
 * @brief Allocate and initialize the dml_job instance.
 *
 * @param[in]     path          execution path
 *
 * @return pointer to allocated @ref dml_job_t.
 */
dml_job_t *init_dml_job(const dml_path_t path);

/**
 * @brief Finalize and deallocate @ref dml_job_t allocated with the  @ref init_dml_job function.
 *
 * @param[in] dml_job_ptr  pointer to allocated DML Job
 *
 */
dml_status_t finalize_dml_job(dml_job_t *const dml_job_ptr);

/* ------ INTEL DML WRAPPERS ------ */

/**
 * @brief No-Op wrapper.
 *
 * @param[in,out] dml_job_ptr       pointer to allocated @ref dml_job_t
 *
 * @return @ref DML_OP_NOP operation result.
 */
dml_status_t dml_nop(dml_job_t *const dml_job_ptr);

/**
 * @brief Simple memory move operation wrapper.
 *
 * @param[in]     source_ptr        pointer to data to copy
 * @param[out]    destination_ptr   pointer to memory region, where copied data is placed
 * @param[in]     length            number bytes to copy
 * @param[in,out] dml_job_ptr       pointer to allocated @ref dml_job_t
 *
 * @return @ref DML_OP_MEM_MOVE operation result.
 */
dml_status_t dml_move_8u(const uint8_t *const source_ptr,
                         uint8_t *const destination_ptr,
                         const uint32_t length,
                         dml_job_t *const dml_job_ptr);

/**
 * @brief Simple fill operation wrapper.
 *
 * @param[out] destination_ptr      memory to fill
 * @param[in] pattern_8e_ptr        pattern to fill
 * @param[in] destination_length    number bytes to fill
 * @param[in,out] dml_job_ptr       pointer to allocated @ref dml_job_t
 *
 * @return @ref DML_OP_FILL operation result.
 */
dml_status_t dml_fill_pattern_64u(uint8_t *const destination_ptr,
                                  const uint8_t *const pattern_8e_ptr,
                                  const uint32_t destination_length,
                                  dml_job_t *const dml_job_ptr);

/**
 * @brief Simple compare with pattern operation wrapper.
 *
 * @param[in] source_ptr        memory for comparison
 * @param[in] pattern_8e_ptr    pattern for comparison
 * @param[in] source_length     number bytes to compare
 * @param[in,out] dml_job_ptr   pointer to allocated @ref dml_job_t
 *
 * @return @ref DML_OP_COMPARE_PATTERN operation result.
 */
dml_status_t dml_compare_pattern_64u(const uint8_t *const source_ptr,
                                     const uint8_t *const pattern_8e_ptr,
                                     const uint32_t source_length,
                                     dml_job_t *const dml_job_ptr);

/**
 * @brief Simple CRC operation wrapper.
 *
 * @param[in]     source_ptr        pointer to data
 * @param[in]     length            number bytes to generate
 * @param[in,out] crc_ptr           value of the CRC seed
 * @param[in,out] dml_job_ptr       pointer to allocated @ref dml_job_t
 *
 * @return @ref DML_OP_CRC operation result.
 */
dml_status_t dml_crc(uint8_t *const source_ptr,
                     const uint32_t length,
                     uint32_t *const crc_ptr,
                     dml_job_t *const dml_job_ptr);

/**
 * @brief Simple copy with CRC operation wrapper.
 *
 * @param[in]     source_ptr        pointer to data to copy
 * @param[out]    destination_ptr   pointer to memory region where copied data is placed
 * @param[in]     length            number bytes to copy
 * @param[in,out] crc_ptr           value of the CRC seed and CRC result
 * @param[in,out] dml_job_ptr       pointer to allocated @ref dml_job_t
 *
 * @return @ref DML_OP_COPY_CRC operation result.
 */
dml_status_t dml_crc_copy_8u(const uint8_t *const source_ptr,
                             uint8_t *const destination_ptr,
                             const uint32_t length,
                             uint32_t *const crc_ptr,
                             dml_job_t *const dml_job_ptr);

/**
 * @brief Memory copy with dualcast operation wrapper.
 *
 * @param[in]     source_ptr                pointer to data to copy
 * @param[out]    destination_first_ptr     pointer to the first memory region where copied data is placed
 * @param[out]    destination_second_ptr    pointer to the second memory region where copied data is placed
 * @param[in]     length                    number bytes to copy
 * @param[in,out] dml_job_ptr               pointer to allocated @ref dml_job_t
 *
 * @note If, for example, `destination_second_ptr` points to a durable memory - this fact should be
 *       specified at the initialization stage with hint parameter:
 *
 *  status = dml_init_job(DML_PATH_AUTO, 0, dml_job_ptr);  both src and dst are in RAM
 *  status = dml_init_job(DML_PATH_AUTO, DML_HINT_DST2_DURABLE, dml_job_ptr); src is RAM, dst is in durable memory
 *
 * @return @ref DML_OP_DUALCAST operation result.
 */
dml_status_t dml_copy_dualcast_8u(const uint8_t *const source_ptr,
                                  uint8_t *const destination_first_ptr,
                                  uint8_t *const destination_second_ptr,
                                  const uint32_t length,
                                  dml_job_t *const dml_job_ptr);

/**
 * @brief Create delta record operation wrapper.
 *
 * @param[in]     source_first_ptr     pointer to memory that is used as a reference
 * @param[in]     source_second_ptr    pointer to memory for difference search
 * @param[out]    delta_ptr            pointer to memory containing a delta record
 * @param[in]     max_delta_size       maximal available size of a delta record
 * @param[in]     length               size of bytes for processing and creation of a delta record
 * @param[out]    delta_size_ptr       resulting delta record size
 * @param[in,out] dml_job_ptr          pointer to allocated @ref dml_job_t
 *
 * @return @ref DML_OP_DELTA_CREATE operation result.
 */
dml_status_t dml_create_delta_8u(const uint8_t *const source_first_ptr,
                                 const uint8_t *const source_second_ptr,
                                 uint8_t *const delta_ptr,
                                 const uint32_t max_delta_size,
                                 const uint32_t length,
                                 uint32_t *const delta_size_ptr,
                                 dml_job_t *const dml_job_ptr);

/**
 * @brief Apply delta operation wrapper.
 *
 * @param[in]     delta_ptr             pointer to delta record
 * @param[out]    destination_ptr       memory region, which will be updated with delta record
 * @param[in]     delta_size            size of delta record in bytes
 * @param[in]     destination_length    size in bytes of destination memory region
 * @param[in,out] dml_job_ptr           pointer to allocated @ref dml_job_t
 *
 * @return @ref DML_OP_DELTA_APPLY operation result
 */
dml_status_t dml_apply_delta_8u(const uint8_t *const delta_ptr,
                                uint8_t *const destination_ptr,
                                const uint32_t delta_size,
                                const uint32_t destination_length,
                                dml_job_t *const dml_job_ptr);

/**
 * @brief DIF update operation wrapper for 4096 byte data blocks:
 *
 * @param[in]     source_ptr        pointer to data to be be updated with new DIF
 * @param[out]    destination_ptr   pointer to memory, where updated data is stored
 * @param[in]     length            total byte size of DIFed blocks
 * @param[in,out] dml_job_ptr       pointer to allocated @ref dml_job_t
 *
 * @return @ref DML_OP_DIF_UPDATE operation result.
 *
 */
dml_status_t dml_dif_update_8u(const uint8_t *const source_ptr,
                               uint8_t *const destination_ptr,
                               const uint32_t length,
                               dml_job_t *const dml_job_ptr);

/**
 * @brief DIF check operation wrapper for 4096 byte data blocks:
 *
 * @param[in]     source_ptr        pointer to data to be be checked
 * @param[in]     length            total byte size of DIFed blocks
 * @param[in,out] dml_job_ptr       pointer to allocated @ref dml_job_t
 *
 * @return @ref DML_OP_DIF_CHECK operation result.
 *
 */
dml_status_t dml_dif_check_8u(const uint8_t *const source_ptr,
                              const uint32_t length,
                              dml_job_t *const dml_job_ptr);

/**
 * @brief DIF insert operation wrapper for 4096 byte data blocks:
 *
 * @param[in]     source_ptr        pointer to data
 * @param[out]    destination_ptr   pointer to memory for DIFed data
 * @param[in]     source_length     total byte size of source data
 * @param[in]     destination_size  total byte size of DIFed blocks
 * @param[in,out] dml_job_ptr       pointer to allocated @ref dml_job_t
 *
 * @return @ref DML_OP_DIF_INSERT operation result.
 *
 */
dml_status_t dml_dif_insert_8u(const uint8_t *const source_ptr,
                               uint8_t *const destination_ptr,
                               const uint32_t source_length,
                               const uint32_t destination_size,
                               dml_job_t *const dml_job_ptr);

/**
 * @brief DIF strip operation wrapper for 4096 byte data blocks:
 *
 * @param[in]     source_ptr        pointer to DIFed data
 * @param[out]    destination_ptr   pointer to memory for unDIFed data
 * @param[in]     source_length     total byte size of DIFed blocks
 * @param[in]     destination_size  total byte size of unDIFed data
 * @param[in,out] dml_job_ptr       pointer to allocated @ref dml_job_t
 *
 * @return @ref DML_OP_DIF_STRIP operation result.
 *
 */
dml_status_t dml_dif_strip_8u(const uint8_t *const source_ptr,
                              uint8_t *const destination_ptr,
                              const uint32_t source_length,
                              const uint32_t destination_size,
                              dml_job_t *const dml_job_ptr);

/**
 * @brief Wrapper for cache flush operation
 *
 * @param[out]    destination_ptr   pointer to memory to be flushed to
 * @param[in]     length            total byte size of flushed data
 * @param[in,out] dml_job_ptr       pointer to allocated @ref dml_job_t
 *
 * @return @ref DML_OP_CACHE_FLUSH operation result.
 *
 */
dml_status_t dml_cache_flush_8u(uint8_t *const destination_ptr,
                                const uint32_t length,
                                dml_job_t *const dml_job_ptr);

/**
 * @brief Wrapper for the batch operation
 *
 * @param[in]     source_first_ptr         pointer to the first source data
 * @param[out]    destination_first_ptr    pointer to the first destination data
 * @param[in]     first_length             length of the first chunk of data
 * @param[in]     source_second_ptr        pointer to the second source data
 * @param[out]    destination_second_ptr   pointer to the second destination data
 * @param[in]     second_length            length of the second chunk of data
 * @param[in,out] dml_job_ptr              pointer to allocated @ref dml_job_t
 *
 * @return @ref DML_OP_BATCH operation result.
 *
 */
dml_status_t dml_copy_batch_8u(uint8_t *source_first_ptr,
                               uint8_t *destination_first_ptr,
                               uint32_t first_length,
                               uint8_t *source_second_ptr,
                               uint8_t *destination_second_ptr,
                               uint32_t second_length,
                               dml_job_t *const dml_job_ptr);

/**
 * @brief Submit wrapper.
 *
 * @param[in]     source_first_ptr     pointer to first data
 * @param[in]     source_second_ptr    pointer to second data
 * @param[in]     source_first_length  length of the first source
 * @param[in]     source_second_length length of the second source
 * @param[in]     crc_first_ptr        the first value of the CRC seed
 * @param[in]     crc_second_ptr       the second value of the CRC seed
 * @param[in,out] dml_first_job_ptr    the first pointer to allocated @ref dml_job_t
 * @param[in,out] dml_second_job_ptr   the second pointer to allocated @ref dml_job_t
 *
 * @return One of @ref DML_OP_CRC operation result.
 */
dml_status_t dml_submit(const uint8_t *const source_first_ptr,
                        const uint8_t *const source_second_ptr,
                        const uint32_t source_first_length,
                        const uint32_t source_second_length,
                        uint32_t *const crc_first_ptr,
                        uint32_t *const crc_second_ptr,
                        dml_job_t *const dml_first_job_ptr,
                        dml_job_t *const dml_second_job_ptr);

#endif //DML_SAMPLES_H__

/** @} */
