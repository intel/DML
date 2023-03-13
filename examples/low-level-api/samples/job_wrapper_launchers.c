/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @brief Contains samples for DML_OP_MEM_MOVE
 * @date 3/10/2020
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "dml/dml.h"
#include "dml_examples.h"

#define SAMPLE_ARRAY_SIZE 128u /**< Array to directly allocation on the stack*/

#define DIF_SIZE              8u /**< Size of Data Integrity Field */
#define DIF_SAMPLE_BLOCK_SIZE 4096u /**< Source Block size            */

/**
 * @brief Array size with data blocks only
 */
#define DIF_SAMPLE_REFERENCE_ARRAY_SIZE DIF_SAMPLE_BLOCK_SIZE

/**
 * @brief Array size with DIF fields after each data block
 */
#define DIF_SAMPLE_PROTECTED_ARRAY_SIZE \
    (DIF_SAMPLE_REFERENCE_ARRAY_SIZE + (DIF_SAMPLE_REFERENCE_ARRAY_SIZE / DIF_SAMPLE_BLOCK_SIZE) * DIF_SIZE)

/**
 * @brief Array size with offset fields for each data block (delta)
 */
#define DELTA_SAMPLE_ARRAY_SIZE ((SAMPLE_ARRAY_SIZE / 8u) * 10u)

/**
 * @brief Resets job input/output stream fields
 *
 * @param[in,out] job_ptr   pointer to @ref dml_job_t
 */
static inline void reset_job_streams(dml_job_t *const job_ptr)
{
    job_ptr->source_first_ptr       = NULL;
    job_ptr->source_second_ptr      = NULL;
    job_ptr->destination_first_ptr  = NULL;
    job_ptr->destination_second_ptr = NULL;
    job_ptr->crc_checksum_ptr       = NULL;
    job_ptr->source_length          = 0;
    job_ptr->destination_length     = 0;
}

/* ------  SAMPLE LAUNCHERS  ------ */

/**
 * @brief Prepares parameters and runs the dml_nop wrapper
 *
 * @param[in,out] dml_job_ptr   pointer to @ref dml_job_t
 *
 * @return @ref dml_status_t
 */
dml_status_t nop_sample_launcher(dml_job_t *const dml_job_ptr)
{
    printf("\n\t Example of using Intel DML DML_OP_NOP operation \n"
           "\t --- Buffers size to DML_OP_NOP operation: %d\n"
           "\t --- DML_OP_NOP property: no any specific properties \n\n",
           SAMPLE_ARRAY_SIZE);

    return dml_nop(dml_job_ptr);
}

/**
 * @brief Prepares parameters and runs dml_fill_pattern_64u wrapper
 *
 * @param[in,out] dml_job_ptr   pointer to @ref dml_job_t
 *
 * @return @ref dml_status_t
 */
dml_status_t fill_sample_launcher(dml_job_t *const dml_job_ptr)
{
    printf("\n\t Example of using Intel DML DML_OP_FILL operation \n"
           "\t --- Buffers size to DML_OP_FILL operation: %d\n"
           "\t --- DML_OP_FILL property: no any specific properties \n\n",
           SAMPLE_ARRAY_SIZE);

    uint8_t destination_array[SAMPLE_ARRAY_SIZE];
    uint8_t pattern_8e[PATTERN_SIZE] = { 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u };

    dml_status_t status = dml_fill_pattern_64u(destination_array, pattern_8e, SAMPLE_ARRAY_SIZE, dml_job_ptr);
    reset_job_streams(dml_job_ptr);

    return status;
}

/**
 * @brief Prepares parameters and runs the dml_move_8u wrapper
 *
 * @param[in,out] dml_job_ptr   pointer to @ref dml_job_t
 *
 * @return @ref dml_status_t
 */
dml_status_t move_sample_launcher(dml_job_t *const dml_job_ptr)
{
    printf("\n\t Example of using Intel DML DML_OP_MEM_MOVE operation \n"
           "\t --- Buffers size to DML_OP_MEM_MOVE operation: %d\n"
           "\t --- DML_OP_MEM_MOVE property: none \n\n",
           SAMPLE_ARRAY_SIZE);

    uint8_t source_array[SAMPLE_ARRAY_SIZE];
    uint8_t destination_array[SAMPLE_ARRAY_SIZE];

    dml_status_t status = dml_move_8u(source_array, destination_array, SAMPLE_ARRAY_SIZE, dml_job_ptr);
    reset_job_streams(dml_job_ptr);

    return status;
}

/**
 * @brief Prepares parameters and runs the dml_create_delta_8u wrapper
 *
 * @param[in,out] dml_job_ptr   pointer to @ref dml_job_t
 *
 * @return @ref dml_status_t
 */
dml_status_t create_delta_sample_launcher(dml_job_t *const dml_job_ptr)
{
    printf("\n\t Example of using Intel DML DML_OP_DELTA_CREATE operation \n"
           "\t --- Buffers size to DML_OP_DELTA_CREATE operation: %d\n"
           "\t --- DML_OP_DELTA_CREATE property: none \n\n",
           SAMPLE_ARRAY_SIZE);

    uint8_t source_first_array[SAMPLE_ARRAY_SIZE];
    uint8_t source_second_array[SAMPLE_ARRAY_SIZE];
    uint8_t destination_array[DELTA_SAMPLE_ARRAY_SIZE];

    uint32_t delta = 0u;

    dml_status_t status = dml_create_delta_8u(source_first_array,
                                              source_second_array,
                                              destination_array,
                                              DELTA_SAMPLE_ARRAY_SIZE,
                                              SAMPLE_ARRAY_SIZE,
                                              &delta,
                                              dml_job_ptr);
    reset_job_streams(dml_job_ptr);

    return status;
}

/**
 * @brief Prepares parameters and runs the dml_apply_delta_8u wrapper
 *
 * @param[in,out] dml_job_ptr   pointer to @ref dml_job_t
 *
 * @return @ref dml_status_t
 */
dml_status_t apply_delta_sample_launcher(dml_job_t *const dml_job_ptr)
{
    printf("\n\t Example of using Intel DML DML_OP_DELTA_APPLY operation \n"
           "\t --- Buffers size to DML_OP_DELTA_APPLY operation: %d\n"
           "\t --- DML_OP_DELTA_APPLY property: none \n\n",
           SAMPLE_ARRAY_SIZE);

    uint8_t source_first_array[SAMPLE_ARRAY_SIZE];
    uint8_t source_second_array[SAMPLE_ARRAY_SIZE];
    uint8_t destination_array[DELTA_SAMPLE_ARRAY_SIZE];

    uint32_t delta = 0u;

    dml_status_t status = dml_create_delta_8u(source_first_array,
                                              source_second_array,
                                              destination_array,
                                              DELTA_SAMPLE_ARRAY_SIZE,
                                              SAMPLE_ARRAY_SIZE,
                                              &delta,
                                              dml_job_ptr);

    if (DML_STATUS_OK != status)
    {
        status = dml_apply_delta_8u(destination_array,
                                    source_second_array,
                                    DELTA_SAMPLE_ARRAY_SIZE,
                                    SAMPLE_ARRAY_SIZE,
                                    dml_job_ptr);
    }
    reset_job_streams(dml_job_ptr);

    return status;
}

/**
 * @brief Prepares parameters and runs the dml_crc wrapper
 *
 * @param[in,out] dml_job_ptr   pointer to @ref dml_job_t
 *
 * @return @ref dml_status_t
 */
dml_status_t crc_sample_launcher(dml_job_t *const dml_job_ptr)
{
    printf("\n\t Example of using Intel DML DML_OP_CRC operation \n"
           "\t --- Buffers size to DML_OP_CRC operation: %d\n"
           "\t --- DML_OP_CRC property: none \n\n",
           SAMPLE_ARRAY_SIZE);

    uint8_t source_array[SAMPLE_ARRAY_SIZE];
    uint32_t crc_sum = 1u;

    dml_status_t status = dml_crc(source_array, SAMPLE_ARRAY_SIZE, &crc_sum, dml_job_ptr);
    reset_job_streams(dml_job_ptr);

    return status;
}

/**
 * @brief Prepares parameters and runs the dml_crc_copy_8u wrapper
 *
 * @param[in,out] dml_job_ptr   pointer to @ref dml_job_t
 *
 * @return @ref dml_status_t
 */
dml_status_t crc_copy_sample_launcher(dml_job_t *const dml_job_ptr)
{
    printf("\n\t Example of using Intel DML DML_OP_CRC_COPY operation \n"
           "\t --- Buffers size to DML_OP_CRC_COPY operation: %d\n"
           "\t --- DML_OP_CRC_COPY property: none \n\n",
           SAMPLE_ARRAY_SIZE);

    uint8_t source_array[SAMPLE_ARRAY_SIZE];
    uint8_t destination_array[SAMPLE_ARRAY_SIZE];
    uint32_t crc_sum = 1u;

    dml_status_t status = dml_crc_copy_8u(source_array,
                                          destination_array,
                                          SAMPLE_ARRAY_SIZE,
                                          &crc_sum,
                                          dml_job_ptr);
    reset_job_streams(dml_job_ptr);

    return status;
}

/**
 * @brief Prepares parameters and runs dml_compare_pattern_64u wrapper
 *
 * @param[in,out] dml_job_ptr   pointer to @ref dml_job_t
 *
 * @return @ref dml_status_t
 */
dml_status_t compare_pattern_sample_launcher(dml_job_t *const dml_job_ptr)
{
    printf("\n\t Example of using Intel DML DML_OP_COMPARE_PATTERN operation \n"
           "\t --- Buffers size to DML_OP_COMPARE_PATTERN operation: %d\n"
           "\t --- DML_OP_COMPARE_PATTERN property: none\n",
           SAMPLE_ARRAY_SIZE);

    dml_status_t status;
    uint8_t source_array[SAMPLE_ARRAY_SIZE];
    uint8_t pattern_8e[PATTERN_SIZE] = { 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u };

    // Fill test array with pattern
    for (uint32_t i = 0u; i < SAMPLE_ARRAY_SIZE; i++)
    {
        source_array[i] = pattern_8e[i % PATTERN_SIZE];
    }

    // Array is equal to pattern
    status = dml_compare_pattern_64u(source_array, pattern_8e, SAMPLE_ARRAY_SIZE, dml_job_ptr);

    printf("\n\t Array is equal to pattern \n"
           "\t --- Status : %d\n"
           "\t --- Result : %d\n"
           "\t --- Offset : %d\n",
           status,
           dml_job_ptr->result,
           dml_job_ptr->offset);

    // Array is NOT equal to pattern
    uint32_t mismatch_offset = SAMPLE_ARRAY_SIZE / 2u;

    source_array[mismatch_offset] = ~source_array[mismatch_offset]; // Invert value by offset

    status = dml_compare_pattern_64u(source_array, pattern_8e, SAMPLE_ARRAY_SIZE, dml_job_ptr);

    printf("\n\t Array is NOT equal to pattern \n"
           "\t --- Status : %d\n"
           "\t --- Result : %d\n"
           "\t --- Offset : %d\n\n",
           status,
           dml_job_ptr->result,
           dml_job_ptr->offset);

    reset_job_streams(dml_job_ptr);

    return status;
}

/**
 * @brief Prepares parameters and runs dml_dif_update_8u wrapper
 *
 * @param[in,out] dml_job_ptr   pointer to @ref dml_job_t
 *
 * @return @ref dml_status_t
 */
dml_status_t dif_update_sample_launcher(dml_job_t *const dml_job_ptr)
{
    printf("\n\t Example of using Intel DML DML_OP_DIF_UPDATE operation \n"
           "\t --- Buffers size to DML_OP_DIF_UPDATE operation: %d\n"
           "\t --- DML_OP_DIF_UPDATE property: BLOCK_SIZE is %d \n\n",
           DIF_SAMPLE_PROTECTED_ARRAY_SIZE,
           DIF_SAMPLE_BLOCK_SIZE);

    uint8_t source_array[DIF_SAMPLE_PROTECTED_ARRAY_SIZE];
    uint8_t destination_array[DIF_SAMPLE_PROTECTED_ARRAY_SIZE];

    dml_status_t status = dml_dif_update_8u(source_array,
                                            destination_array,
                                            DIF_SAMPLE_PROTECTED_ARRAY_SIZE,
                                            dml_job_ptr);
    reset_job_streams(dml_job_ptr);

    return status;
}

/**
 * @brief Prepares parameters and runs the dml_dif_check_8u wrapper
 *
 * @param[in,out] dml_job_ptr   pointer to @ref dml_job_t
 *
 * @return @ref dml_status_t
 */
dml_status_t dif_check_sample_launcher(dml_job_t *const dml_job_ptr)
{
    printf("\n\t Example of using Intel DML DML_OP_DIF_CHECK operation \n"
           "\t --- Buffers size to DML_OP_DIF_CHECK operation: %d and %d\n"
           "\t --- DML_OP_DIF_CHECK property: BLOCK_SIZE is %d \n\n",
           DIF_SAMPLE_REFERENCE_ARRAY_SIZE,
           DIF_SAMPLE_PROTECTED_ARRAY_SIZE,
           DIF_SAMPLE_BLOCK_SIZE);

    uint8_t source_array[DIF_SAMPLE_REFERENCE_ARRAY_SIZE];
    uint8_t destination_array[DIF_SAMPLE_PROTECTED_ARRAY_SIZE];

    dml_status_t status = dml_dif_insert_8u(source_array,
                                            destination_array,
                                            DIF_SAMPLE_REFERENCE_ARRAY_SIZE,
                                            DIF_SAMPLE_PROTECTED_ARRAY_SIZE,
                                            dml_job_ptr);

    if (DML_STATUS_OK == status)
    {
        status = dml_dif_check_8u(destination_array, DIF_SAMPLE_PROTECTED_ARRAY_SIZE, dml_job_ptr);
    }

    reset_job_streams(dml_job_ptr);

    return status;
}

/**
 * @brief Prepares parameters and runs the dml_dif_insert_8u wrapper
 *
 * @param[in,out] dml_job_ptr   pointer to @ref dml_job_t
 *
 * @return @ref dml_status_t
 */
dml_status_t dif_insert_sample_launcher(dml_job_t *const dml_job_ptr)
{
    printf("\n\t Example of using Intel DML DML_OP_DIF_INSERT operation \n"
           "\t --- Buffers size to DML_OP_DIF_INSERT operation: %d and %d\n"
           "\t --- DML_OP_DIF_INSERT property: BLOCK_SIZE is %d \n\n",
           DIF_SAMPLE_REFERENCE_ARRAY_SIZE,
           DIF_SAMPLE_PROTECTED_ARRAY_SIZE,
           DIF_SAMPLE_BLOCK_SIZE);

    uint8_t source_array[DIF_SAMPLE_REFERENCE_ARRAY_SIZE];
    uint8_t destination_array[DIF_SAMPLE_PROTECTED_ARRAY_SIZE];

    dml_status_t status = dml_dif_insert_8u(source_array,
                                            destination_array,
                                            DIF_SAMPLE_REFERENCE_ARRAY_SIZE,
                                            DIF_SAMPLE_PROTECTED_ARRAY_SIZE,
                                            dml_job_ptr);

    reset_job_streams(dml_job_ptr);

    return status;
}

/**
 * @brief Prepares parameters and runs the dml_dif_strip_8u wrapper
 *
 * @param[in,out] dml_job_ptr   pointer to @ref dml_job_t
 *
 * @return @ref dml_status_t
 */
dml_status_t dif_strip_sample_launcher(dml_job_t *const dml_job_ptr)
{
    printf("\n\t Example of using Intel DML DML_OP_DIF_STRIP operation \n"
           "\t --- Buffers size to DML_OP_DIF_STRIP operation: %d and %d\n"
           "\t --- DML_OP_DIF_STRIP property: BLOCK_SIZE is %d \n\n",
           DIF_SAMPLE_PROTECTED_ARRAY_SIZE,
           DIF_SAMPLE_REFERENCE_ARRAY_SIZE,
           DIF_SAMPLE_BLOCK_SIZE);

    // The same size used because of DML_STATUS_DIF_STRIP_ADJACENT_ERROR
    uint8_t source_array[DIF_SAMPLE_PROTECTED_ARRAY_SIZE];
    uint8_t destination_array[DIF_SAMPLE_PROTECTED_ARRAY_SIZE];

    dml_status_t status = dml_dif_insert_8u(source_array,
                                            destination_array,
                                            DIF_SAMPLE_REFERENCE_ARRAY_SIZE,
                                            DIF_SAMPLE_PROTECTED_ARRAY_SIZE,
                                            dml_job_ptr);

    if (DML_STATUS_OK == status)
    {
        // Parameters are flipped.
        status = dml_dif_strip_8u(destination_array,
                                  source_array,
                                  DIF_SAMPLE_PROTECTED_ARRAY_SIZE,
                                  DIF_SAMPLE_REFERENCE_ARRAY_SIZE,
                                  dml_job_ptr);
    }

    reset_job_streams(dml_job_ptr);

    return status;
}

/**
 * @brief Prepares parameters and runs the dml_move_8u wrapper
 *
 * @param[in,out] dml_job_ptr   pointer to @ref dml_job_t
 *
 * @return @ref dml_status_t
 */
dml_status_t cache_flush_sample_launcher(dml_job_t *const dml_job_ptr)
{
    printf("\n\t Example of using Intel DML DML_OP_CACHE_FLUSH operation \n"
           "\t --- Buffers size to DML_OP_CACHE_FLUSH operation: %d\n"
           "\t --- DML_OP_CACHE_FLUSH property: none \n\n",
           SAMPLE_ARRAY_SIZE);

    uint8_t destination_array[SAMPLE_ARRAY_SIZE];

    dml_status_t status = dml_cache_flush_8u(destination_array, SAMPLE_ARRAY_SIZE, dml_job_ptr);

    reset_job_streams(dml_job_ptr);

    return status;
}


/**
 * @brief Prepares parameters and runs the dml_batch wrapper
 *
 * @param[in,out] dml_job_ptr   pointer to @ref dml_job_t
 *
 * @return @ref dml_status_t
 */
dml_status_t batch_sample_launcher(dml_job_t *const dml_job_ptr)
{
    printf("\n\t Example of using Intel DML DML_OP_BATCH operation \n"
           "\t --- Buffers size to DML_OP_BATCH operation: %d\n"
           "\t --- DML_OP_BATCH property: none \n\n",
           SAMPLE_ARRAY_SIZE);

    dml_status_t status       = DML_STATUS_OK;
    uint8_t source_array_1[SAMPLE_ARRAY_SIZE];
    uint8_t source_array_2[SAMPLE_ARRAY_SIZE];
    uint8_t destination_array_1[SAMPLE_ARRAY_SIZE];
    uint8_t destination_array_2[SAMPLE_ARRAY_SIZE];

    status = dml_copy_batch_8u(source_array_1,
                               destination_array_1,
                               SAMPLE_ARRAY_SIZE,
                               source_array_2,
                               destination_array_2,
                               SAMPLE_ARRAY_SIZE,
                               dml_job_ptr);

    reset_job_streams(dml_job_ptr);

    return status;
}


/**
 * @brief Prepares parameters and runs the dml_submit wrapper
 *
 * @param[in,out] dml_job_ptr   pointer to @ref dml_job_t
 *
 * @return @ref dml_status_t
 */
dml_status_t submit_sample_launcher(dml_job_t *const dml_job_ptr)
{
    printf("\n\t Example of using Intel DML DML_OP_CRC operation with dml_submit_job \n"
           "\t --- Buffers size to DML_OP_CRC operation: %d\n"
           "\t --- DML_OP_CRC property: no any specific properties \n\n",
           SAMPLE_ARRAY_SIZE);

    dml_job_t *additional_job_ptr = init_dml_job(DML_PATH_SW);

    if (!additional_job_ptr)
    {
        return DML_STATUS_NULL_POINTER_ERROR;
    }

    uint32_t crc_first_seed = 1u;
    uint32_t crc_second_seed = 2u;

    uint8_t source_first_data[SAMPLE_ARRAY_SIZE];
    uint8_t source_second_data[SAMPLE_ARRAY_SIZE];

    dml_status_t status = dml_submit(source_first_data,
                                     source_second_data,
                                     SAMPLE_ARRAY_SIZE,
                                     SAMPLE_ARRAY_SIZE,
                                     &crc_first_seed,
                                     &crc_second_seed,
                                     dml_job_ptr,
                                     additional_job_ptr);

    reset_job_streams(dml_job_ptr);

    if (DML_STATUS_OK != status)
    {
        // Do not check status because the dml_submit already failed
        (void) finalize_dml_job(additional_job_ptr);
        return status;
    }

    return finalize_dml_job(additional_job_ptr);
}

/* ------ LAUNCHERS ARRAY ------ */
dml_status_t (*wrapper_launchers[SAMPLES_COUNT])(dml_job_t *const dml_job_ptr) = {
        nop_sample_launcher,          move_sample_launcher,
        fill_sample_launcher,         compare_pattern_sample_launcher,
        dif_update_sample_launcher,   dif_insert_sample_launcher,
        dif_check_sample_launcher,    dif_strip_sample_launcher,
        cache_flush_sample_launcher,  batch_sample_launcher,
        crc_sample_launcher,          crc_copy_sample_launcher,
        submit_sample_launcher,       create_delta_sample_launcher,
        apply_delta_sample_launcher
};
