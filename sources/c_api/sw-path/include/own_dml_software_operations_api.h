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
 * @date 3/3/2020
 *
 * @defgroup SW_PATH_OPS Operations API
 * @ingroup SW_PATH
 * @{
 * @brief Contains Intel(R) Data Mover Library (Intel® DML) operation declarations for software path of DML library.
 */

#include "own_dml_api.h"
#include "core_api.h"

#ifndef DML_OWN_SOFTWARE_API_H__
#define DML_OWN_SOFTWARE_API_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The Memory Move operation copies memory from the source address into destination one.
 *
 * @param [in,out] dml_job_ptr pointer onto user specified @ref dml_job_t
 *
 * @details For operation execution, you must set the following parameters in `dml_job_ptr`:
 *          - @ref dml_job_t.source_first_ptr      - start address of memory region to copy
 *          - @ref dml_job_t.destination_first_ptr - start address of memory region to fill by coping values
 *          - @ref dml_job_t.source_length         - number of bytes copied
 *
 * @note If the source and destination regions overlap, the behavior depends on the value of @ref DML_FLAG_COPY_ONLY
 * If @ref DML_FLAG_COPY_ONLY is 0, the memory copy is done as if the entire source buffer is copied
 * to temporary space and then copied to the destination buffer. If Overlapping Copy Support is 0, it is an error.
 *
 * @note There are no alignment requirements for the memory addresses or the transfer size.
 *
 * @return
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_OVERLAPPING_BUFFER_ERROR
 *
 */
OWN_API_INLINE(dml_status_t, sw_memory_move, (dml_job_t *const dml_job_ptr))


/**
 * @brief The Memory Fill operation fills memory at the Destination Address with the value in the pattern field.
 *
 * @param [in,out] dml_job_ptr pointer onto user specified @ref dml_job_t
 *
 * @details For operation execution, you must set the following parameters in `dml_job_ptr`:
 *          - @ref dml_job_t.destination_first_ptr - start address of memory region to fill
 *          - @ref dml_job_t.destination_length    - number of bytes written is given by
 *          - @ref dml_job_t.pattern               - 8-byte pattern to fill
 *
 * @note The @ref dml_job_t.pattern size is always 8 bytes.
 *
 * @note The @ref dml_job_t.source_length does not need to be a multiple of the @ref dml_job_t.pattern size.
 *
 * @note There are no alignment requirements for the @ref dml_job_t.destination_first_ptr or
 * the @ref dml_job_t.destination_length.
 *
 * @return
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *
 */
OWN_API_INLINE(dml_status_t, sw_fill, (dml_job_t *const dml_job_ptr))


/**
 * @brief The Compare operation compares memory at Source1 Address with memory at Source2 Address.
 *
 * @param [in,out] dml_job_ptr pointer onto user specified @ref dml_job_t
 *
 * @details For operation execution, you must set the following parameters in `dml_job_ptr`:
 *          - @ref dml_job_t.source_first_ptr    - Source1 Address to compare
 *          - @ref dml_job_t.source_second_ptr   - Source2 Address to compare
 *          - @ref dml_job_t.source_length field - number of bytes to compare
 *
 * @note There are no alignment requirements.
 *
 * @note Operation result can be controlled with the @ref DML_FLAG_CHECK_RESULT flag.
 *       For more info about this behaviour see the @ref idml_sw_check_result function description.
 * @note The @ref dml_job_t.result field will contain comparison result after comparison
 * @note The @ref dml_job_t.offset field will contain the first mismatch offset in case of not equal streams
 *
 *
 * @return
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_FALSE_PREDICATE_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *
 */
OWN_API_INLINE(dml_status_t, sw_compare, (dml_job_t *const dml_job_ptr))


/**
 * @brief The Compare with pattern operation compares memory at Source Address with 8-byte pattern.
 *
 * @param [in,out] dml_job_ptr pointer onto user specified @ref dml_job_t
 *
 * @details For operation execution, you must set the following parameters in `dml_job_ptr`:
 *          - @ref dml_job_t.source_first_ptr - Source Address to compare
 *          - @ref dml_job_t.pattern          - 8-byte pattern to compare
 *          - @ref dml_job_t.source_length    - number of bytes to compare
 *
 * @note Operation result can be controlled with the @ref DML_FLAG_CHECK_RESULT flag.
 *       For more info about this behaviour see the @ref idml_sw_check_result function description.
 * @note The @ref dml_job_t.result field will contain comparison result after comparison
 * @note The @ref dml_job_t.offset field will contain the first mismatch offset in case of not equal streams
 *
 * @return
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_FALSE_PREDICATE_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *
 */
OWN_API_INLINE(dml_status_t, sw_compare_with_pattern, (dml_job_t *const dml_job_ptr))


/**
 * @brief The Create Delta Record operation compares memory at Source1 Address with memory at Source2
 * Address and generates a delta record that contains the information needed to update Source1 to match
 * Source 2.
 *
 * @param [in,out] dml_job_ptr pointer onto user specified @ref dml_job_t
 *
 * @details For operation execution, you must set the following parameters in `dml_job_ptr`:
 *          - @ref dml_job_t.source_first_ptr           - Source Address 1 to compare
 *          - @ref dml_job_t.source_second_ptr          - Source Address 2 to compare
 *          - @ref dml_job_t.destination_first_ptr      - Delta Record address to write
 *          - @ref dml_job_t.source_length              - number of bytes to compare
 *          - @ref dml_job_t.destination_length         - number of available bytes in delta record
 *
 * @note The @ref dml_job_t.destination_length field will contain size of created delta record.
 *
 * @warning The Source Address 1 and Source Address 2 must be aligned to a multiple of 8
 * @warning The Source Size must be a multiple of 8. Also it is limited by maximum offset,
 * what can be sored in the delta record (524,280 bytes)
 * @warning The Delta Record Size must be a multiple of 10
 *
 * @return
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_DELTA_ALIGN_ERROR in case if vector address don’t been align to be a
 *             multiple of 8;
 *      - @ref DML_STATUS_DELTA_INPUT_SIZE_ERROR in case if input vector size is not multiple of 8 or
 *             in case if input vector size greater then max delta offset supported;
 *      - @ref DML_STATUS_DELTA_RECORD_SIZE_ERROR in case if max_delta_record_size is not enough to delta
 *             record creating or max_delta_record_size is not multiple of 10u;
 *
 */
OWN_API_INLINE(dml_status_t, sw_create_delta, (dml_job_t *const dml_job_ptr))


/**
 * @brief The Apply Delta Record operation applies a delta record to the contents of memory at the Destination Address
 *
 * @param [in,out] dml_job_ptr pointer onto user specified @ref dml_job_t
 *
 * @details For operation execution, you must set the following parameters in `dml_job_ptr`:
 *          - @ref dml_job_t.source_first_ptr field      - delta record to apply
 *          - @ref dml_job_t.source_length field         - delta record size
 *          - @ref dml_job_t.destination_first_ptr field - destination to apply
 *          - @ref dml_job_t.destination_length field    - available bytes in destination address
 *
 * @warning The Destination Address must be aligned to be a multiple of 8
 * @warning The Destination Size must be a multiple of 8. Also it is limited by maximum offset
 *          that can be sored in the delta record (524,280 bytes)
 * @warning The Delta Record Size must be a multiple of 10
 * @warning Operation does not support an overlap between Destination Address and Delta Record
 *
 * @return
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_DELTA_ALIGN_ERROR in case if memory_region_ptr address is not aligned to be a multiple of 8;
 *      - @ref DML_STATUS_DELTA_INPUT_SIZE_ERROR in case if memory region size is not a multiple of 8
 *      - @ref DML_STATUS_DELTA_RECORD_SIZE_ERROR  in case if delta record size is not a multiple of 10;
 *      - @ref DML_STATUS_OVERLAPPING_BUFFER_ERROR in case if vectors overlap
 *      - @ref DML_STATUS_MEMORY_OVERFLOW_ERROR in case when the offset is greater than the memory region size
 *
 */
OWN_API_INLINE(dml_status_t, sw_apply_delta, (dml_job_t *const dml_job_ptr))


/**
 * @brief The Memory Copy with Dualcast operation copies memory from the Source Address to both
 * Destination Address 1 and Destination Address 2.
 *
 * @param [in,out] dml_job_ptr pointer onto user specified @ref dml_job_t
 *
 * @details For operation execution, you must set the following parameters in `dml_job_ptr`:
 *          - @ref dml_job_t.source_first_ptr       - Source Address to copy
 *          - @ref dml_job_t.destination_first_ptr  - Destination Address 1 to fill by coping values
 *          - @ref dml_job_t.destination_second_ptr - Destination Address 2 to fill by coping values
 *          - @ref dml_job_t.source_length field    - number of bytes copied
 *
 * @note There are no alignment requirements for the source address or the transfer size.
 *
 * @warning Bits 11:0 of the two destination addresses must be the same.
 * @warning Overlapping is not supported
 *
 * @return
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_DUALCAST_ALIGN_ERROR;
 *      - @ref DML_STATUS_OVERLAPPING_BUFFER_ERROR;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 *
 */
OWN_API_INLINE(dml_status_t, sw_dualcast_copy, (dml_job_t *const dml_job_ptr))


/**
 * @brief The CRC Generation operation computes the CRC on memory at the Source Address.
 *
 * @param [in,out] dml_job_ptr pointer onto user specified @ref dml_job_t
 *
 * @details For operation execution, you must set the following parameters in `dml_job_ptr`:
 *          - @ref dml_job_t.source_first_ptr      - start address of memory region to copy
 *          - @ref dml_job_t.source_length         - number of bytes to copy and hash
 *          - @ref dml_job_t.crc_checksum_ptr      - pointer to CRC seed/result address
 *
 * @note There are no alignment requirements for the source address or the transfer size.
 * @note Operation can be configured with the follow flags:
 *     - @ref DML_FLAG_CRC_READ_SEED                Load value from @ref dml_job_t.crc_checksum_ptr as seed
 *     - @ref DML_FLAG_CRC_BYPASS_REFLECTION        Do not use reverse bit order and reflection for CRC seed and result
 *     - @ref DML_FLAG_CRC_BYPASS_DATA_REFLECTION   Set bit 7 of each data byte as the MSB in the CRC computation
 *
 *
 * @return
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 *
 */
OWN_API_INLINE(dml_status_t, sw_crc, (dml_job_t *const dml_job_ptr))


/**
 * @brief The Copy with CRC Generation operation copies memory from the Source Address to the Destination
 * Address and computes the CRC on the data copied.
 *
 * @param [in,out] dml_job_ptr pointer onto user specified @ref dml_job_t
 *
 * @details For operation execution, you must set the following parameters in `dml_job_ptr`:
 *          - @ref dml_job_t.source_first_ptr      - start address of memory region to copy
 *          - @ref dml_job_t.destination_first_ptr - start address of memory region to fill and crc calculation
 *          - @ref dml_job_t.source_length         - number of bytes to copy and hash
 *          - @ref dml_job_t.crc_checksum_ptr      - pointer to CRC seed/result address
 *
 * @note There are no alignment requirements for the source address or the transfer size.
 * @note CRC calculation can be configured with the follow flags:
 *     - @ref DML_FLAG_CRC_READ_SEED                Load value from @ref dml_job_t.crc_checksum_ptr as seed
 *     - @ref DML_FLAG_CRC_BYPASS_REFLECTION        Do not use reverse bit order and reflection for CRC seed and result
 *     - @ref DML_FLAG_CRC_BYPASS_DATA_REFLECTION   Set bit 7 of each data byte as the MSB in the CRC computation
 *
 * @warning Overlapping is not supported.
 * @warning In case of @ref DML_STATUS_OVERLAPPING_BUFFER_ERROR operation status crc_checksum_ptr will not be modified
 *
 * @return
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *      - @ref DML_STATUS_OVERLAPPING_BUFFER_ERROR.
 *
 */
OWN_API_INLINE(dml_status_t, sw_copy_with_crc, (dml_job_t *const dml_job_ptr))


/**
 * @brief The Cache Flush operation flushes modified cache lines that belong to the specified Memory Region.
 *
 * @param [in,out] dml_job_ptr pointer onto user specified @ref dml_job_t
 *
 * @details For operation execution, you must set the following parameters in `dml_job_ptr`:
 *          - @ref dml_job_t.destination_first_ptr - the Memory Region Address to flush cache lines
 *          - @ref dml_job_t.destination_length    - the Memory Region Size to flush
 *
 * @note There are no alignment requirements for the source address or the transfer size.
 * @note Flush operation can be configured with the @ref DML_FLAG_DONT_INVALIDATE_CACHE flag:
 *     - If the flag is 0, affected cache lines are invalidated from every level of the cache hierarchy.
 *     - If the flag is 1, affected cache lines are not evicted from the caches.
 *
 * @warning Any cache line that is partially covered by the destination region is flushed.
 *
 * @return
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR.
 *
 */
OWN_API_INLINE(dml_status_t, sw_cache_flush, (dml_job_t *const dml_job_ptr))


/**
 * @brief The DIF Check operation computes the Data Integrity Field (DIF) on the source data and compares the
 * computed DIF to the DIF contained in the source data.
 *
 * @param[in,out] dml_job_ptr pointer onto user specified @ref dml_job_t
 *
 * @details For operation execution, you must set the following parameters in `dml_job_ptr`:
 *          - @ref dml_job_t.source_first_ptr - source Address to compare
 *          - @ref dml_job_t.source_length    - number of bytes to check
 *          - @ref dml_job_t.dif_config       - configuration for DIF calculation
 *
 * @note There are no alignment requirements.
 * @note It optionally computes the DIF on the source data and compares the computed DIF to the DIF
 *       contained in the source data. To make this just use one of the following flags:
 *          - @ref DML_DIF_FLAG_SRC_GUARD_CHECK_DISABLE
 *          - @ref DML_DIF_FLAG_SRC_REF_TAG_CHECK_DISABLE
 *       The @ref dml_job_t.offset field is set to the number of source bytes successfully processed,
 *       if operation returns the @ref DML_STATUS_DIF_CHECK_ERROR status.
 *
 *
 * @warning Overlapping of memory is not supported.
 * @warning Source Size must be a multiple of @ref dml_dif_config_t.block_size plus 8 bytes
 *          for each source block.
 *
 * @return
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_JOB_LENGTH_ERROR
 *      - @ref DML_STATUS_OVERLAPPING_BUFFER_ERROR
 *      - @ref DML_STATUS_DIF_CHECK_ERROR
 *
 */
OWN_API(dml_status_t, sw_check_dif, (dml_job_t *const dml_job_ptr))


/**
 * @brief The DIF Insert operation copies memory from the Source Address to the Destination Address,
 * while computing the Data Integrity Field (DIF) on the source data and inserting the DIF into the output data.
 *
 * @param[in,out] dml_job_ptr pointer onto user specified @ref dml_job_t
 *
 * @details For operation execution, you must set the following parameters in `dml_job_ptr`:
 *          - @ref dml_job_t.source_first_ptr      - The Source Address for DIF calculation
 *          - @ref dml_job_t.source_length         - The number of bytes to compute DIF
 *          - @ref dml_job_t.destination_first_ptr - The Destination Address to fill with protected data
 *          - @ref dml_job_t.dif_config            - configuration for DIF calculation
 *
 * @note There are no alignment requirements.
 *
 * @warning Overlapping of memory doesn't supported.
 * @warning Source Size must be a multiple of the @ref dml_dif_config_t.block_size.
 * @warning Destination Size is the Source Size plus 8 bytes for each source block.
 *
 * @return
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_JOB_LENGTH_ERROR
 *      - @ref DML_STATUS_OVERLAPPING_BUFFER_ERROR
 *
 */
OWN_API(dml_status_t, sw_insert_dif, (dml_job_t *const dml_job_ptr))


/**
 * @brief The DIF Strip operation copies memory from the Source Address to the Destination Address, removing the
 * Data Integrity Field (DIF).
 *
 * @param[in,out] dml_job_ptr pointer onto user specified @ref dml_job_t
 *
 * @details For operation execution, you must set the following parameters in `dml_job_ptr`:
 *          - @ref dml_job_t.source_first_ptr      - Source Address to compare
 *          - @ref dml_job_t.source_length         - number of bytes in the stream
 *          - @ref dml_job_t.destination_first_ptr - The Destination Address to fill with striped DIF
 *          - @ref dml_job_t.dif_config            - configuration for DIF calculation
 *
 * @note There are no alignment requirements.
 * @note It optionally computes the DIF on the source data and compares the computed DIF to the DIF
 *       contained in the source data. To make this just use one of the following flags:
 *          - @ref DML_DIF_FLAG_SRC_GUARD_CHECK_DISABLE
 *          - @ref DML_DIF_FLAG_SRC_REF_TAG_CHECK_DISABLE
 *       The @ref dml_job_t.offset field is set to the number of source bytes successfully processed,
 *       in case if operation returned the @ref DML_STATUS_DIF_CHECK_ERROR status.
 *
 * @warning Overlapping of memory doesn't supported.
 * @warning Source Size must be a multiple of the @ref dml_dif_config_t.block_size plus 8 bytes
 *          for each source block.
 * @warning Destination Size must be a multiple of the @ref dml_dif_config_t.block_size
 *
 * @return
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_JOB_LENGTH_ERROR
 *      - @ref DML_STATUS_OVERLAPPING_BUFFER_ERROR
 *      - @ref DML_STATUS_DIF_CHECK_ERROR
 *
 */
OWN_API(dml_status_t, sw_strip_dif, (dml_job_t *const dml_job_ptr))


/**
 * @brief The DIF Update operation copies memory from the Source Address to the Destination Address and
 *        updates DIFs in accordance with destination flags.
 *
 * @param[in,out] dml_job_ptr pointer onto user specified @ref dml_job_t
 *
 * @details For operation execution, you must set the following parameters in `dml_job_ptr`:
 *          - @ref dml_job_t.source_first_ptr      - Source Address to compare
 *          - @ref dml_job_t.destination_first_ptr - Destination Address to fill with striped DIF
 *          - @ref dml_job_t.source_length         - number of bytes to check
 *          - @ref dml_job_t.dif_config            - configuration for DIF calculation
 *
 * @note There are no alignment requirements.
 *
 * @note It optionally computes the DIF on the source data and compares the computed DIF to the DIF
 *       contained in the source data. To make this just use one of the following flags:
 *          - @ref DML_DIF_FLAG_SRC_GUARD_CHECK_DISABLE
 *          - @ref DML_DIF_FLAG_SRC_REF_TAG_CHECK_DISABLE
 *       The @ref dml_job_t.offset field is set to the number of source bytes successfully processed,
 *       in case if operation returned the @ref DML_STATUS_DIF_CHECK_ERROR status.
 *
 * @note It simultaneously computes the DIF on the source data using Destination DIF fields in the
 *       descriptor and inserts the computed DIF into the output data. To make this just use one of
 *       the following flags:
 *          - @ref DML_DIF_FLAG_DST_PASS_APP_TAG
 *          - @ref DML_DIF_FLAG_DST_PASS_GUARD
 *          - @ref DML_DIF_FLAG_DST_PASS_REF_TAG
 *
 * @warning Overlapping of memory doesn't supported.
 * @warning Source and Destination sizes must be a multiple of the @ref dml_dif_config_t.block_size
 *          plus 8 bytes for each source block.
 *
 * @return
 *      - @ref DML_STATUS_OK
 *      - @ref DML_STATUS_NULL_POINTER_ERROR
 *      - @ref DML_STATUS_JOB_LENGTH_ERROR
 *      - @ref DML_STATUS_OVERLAPPING_BUFFER_ERROR
 *      - @ref DML_STATUS_DIF_CHECK_ERROR
 *
 */
OWN_API(dml_status_t, sw_update_dif, (dml_job_t *const dml_job_ptr))


/**
 * @brief The Batch operation processes multiple jobs at once.
 *
 * @param [in,out] dml_job_ptr pointer onto user specified @ref dml_job_t
 *
 * @details For operation execution, you must set the following parameters in `dml_job_ptr`:
 *          - @ref dml_job_t.destination_first_ptr  - start address of memory region with jobs
 *          - @ref dml_job_t.destination_length     - number of bytes allocated for jobs
 *
 * @note There are no alignment requirements for the source address or the transfer size.
 *
 *
 * @return
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_BATCH_ERROR.
 *
 */
OWN_API_INLINE(dml_status_t, sw_batch, (dml_job_t *const dml_job_ptr))


#ifdef __cplusplus
}
#endif

#endif //DML_OWN_SOFTWARE_API_H__

/** @} */
