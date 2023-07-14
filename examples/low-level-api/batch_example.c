/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "dml/dml.h"
#include "examples_utils.h"

#define BUFFER_SIZE  1024 // 1 KB
#define PADDING_SIZE 4096 // DML_OP_DUALCAST requirement "dst1 and dst2 address bits 11:0 must be the same"
#define BATCH_COUNT  7u   // 7 ops for this batch operation
#define PATTERN_SIZE 8u   // pattern size is always 8

/*
* This example demonstrates how to create and run a batch operation with
* multiple operations including the nop operation used as a fence.
*/

int main(int argc, char **argv)
{
    dml_path_t execution_path = DML_PATH_SW;

    // Get path from input argument
    int parse_ret = parse_execution_path(argc, argv, &execution_path);
    if (parse_ret != 0) {
        return 1;
    }

    printf("Starting batch job example.\n");

    dml_job_t *dml_job_ptr = NULL;
    uint32_t size = 0u;

    uint8_t buffer_one    [BUFFER_SIZE];
    uint8_t buffer_two    [BUFFER_SIZE];
    uint8_t buffer_three  [BUFFER_SIZE * 2 + PADDING_SIZE];

    dml_status_t status = dml_get_job_size(execution_path, &size);
    if (DML_STATUS_OK != status) {
        printf("An error (%u) occured during getting job size.\n", status);
        return 1;
    }

    dml_job_ptr = (dml_job_t *)malloc(size);

    status = dml_init_job(execution_path, dml_job_ptr);
    if (DML_STATUS_OK != status) {
        printf("An error (%u) occured during job initialization.\n", status);
        free(dml_job_ptr);
        return 1;
    }
    
    uint32_t batch_buffer_length = 0u;

    status = dml_get_batch_size(dml_job_ptr, BATCH_COUNT, &batch_buffer_length);
    if (DML_STATUS_OK != status) {
        printf("An error (%u) occured during getting batch size.\n", status);
        goto cleanup;
    }

    uint8_t * batch_buffer_ptr = (uint8_t *) malloc(batch_buffer_length);

    dml_job_ptr->operation              = DML_OP_BATCH;
    dml_job_ptr->destination_first_ptr  = batch_buffer_ptr;
    dml_job_ptr->destination_length     = batch_buffer_length;

    printf("Filling 1KB of data with 0x00ABCDEFABCDEF00 pattern\n");
    uint8_t pattern     [PATTERN_SIZE] = {0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u};

    status = dml_batch_set_fill_by_index(dml_job_ptr, 0, pattern, buffer_one, BUFFER_SIZE, DML_FLAG_PREFETCH_CACHE);
    if (DML_STATUS_OK != status) {
        printf("An error (%u) occured during setting of batch operation.\n", status);
        goto cleanup;
    }

    printf("Fencing in batch to ensure fill completes before copying.\n");
    status = dml_batch_set_nop_by_index(dml_job_ptr, 1, DML_FLAG_FENCE);
    if (DML_STATUS_OK != status) {
        printf("An error (%u) occured during setting of batch operation.\n", status);
        goto cleanup;
    }

    printf("Moving 1KB of data\n");
    status = dml_batch_set_mem_move_by_index(dml_job_ptr, 2, buffer_one, buffer_two, BUFFER_SIZE, DML_FLAG_PREFETCH_CACHE);
    if (DML_STATUS_OK != status) {
        printf("An error (%u) occured during setting of batch operation.\n", status);
        goto cleanup;
    }

    printf("Dualcasting 1KB of data\n");
    status = dml_batch_set_dualcast_by_index(dml_job_ptr, 3, buffer_one, buffer_three, buffer_three + PADDING_SIZE, BUFFER_SIZE, DML_FLAG_PREFETCH_CACHE);
    if (DML_STATUS_OK != status) {
        printf("An error (%u) occured during setting of batch operation.\n", status);
        goto cleanup;
    }

    printf("Fencing in batch to ensure all moving completes.\n");
    status = dml_batch_set_nop_by_index(dml_job_ptr, 4, DML_FLAG_FENCE);
    if (DML_STATUS_OK != status) {
        printf("An error (%u) occured during setting of batch operation.\n", status);
        goto cleanup;
    }

    printf("Comparing 1KB of data to 0x00ABCDEFABCDEF00 pattern\n");
    status = dml_batch_set_compare_pattern_by_index(dml_job_ptr, 5, buffer_three + PADDING_SIZE, pattern, BUFFER_SIZE, 0, 0x00);
    if (DML_STATUS_OK != status) {
        printf("An error (%u) occured during setting of batch operation.\n", status);
        goto cleanup;
    }

    printf("Comparing two buffers of 1KB of data\n");
    status = dml_batch_set_compare_by_index(dml_job_ptr, 6, buffer_three, buffer_two, BUFFER_SIZE, 0, 0x00);
    if (DML_STATUS_OK != status) {
        printf("An error (%u) occured during setting of batch operation.\n", status);
        goto cleanup;
    }

    status = dml_execute_job(dml_job_ptr, DML_WAIT_MODE_BUSY_POLL);
    if (DML_STATUS_OK != status) {
        printf("An error (%u) occured during job execution.\n", status);
        goto cleanup;
    }

    if(dml_job_ptr->result != 0){
        printf("Error: Operation result is incorrect.\n");
        goto cleanup;
    }

    status = dml_finalize_job(dml_job_ptr);
    if (DML_STATUS_OK != status) {
        printf("An error (%u) occured during job finalization.\n", status);
        free(dml_job_ptr);
        return 1;
    }
    free(dml_job_ptr);

    printf("Job Completed Successfully.\n");
    return 0;

    cleanup:
        dml_finalize_job(dml_job_ptr);
        free(dml_job_ptr);
        return 1;
}
