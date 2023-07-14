/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "dml/dml.h"
#include "examples_utils.h"

#define BUFFER_SIZE       1024 // 1 KB
#define DELTAS_PER_RECORD 8u   // 8 Deltas Per record
#define DELTA_RECORD_SIZE 10u  // Delta Record size 2 bytes (offset) + 8 bytes (Deltas)

/*
* This example demonstrates how to use the delta operations.
* It shows how to create and apply a delta.
*/


int main(int argc, char **argv)
{
    dml_path_t execution_path = DML_PATH_SW;

    // Get path from input argument
    int parse_ret = parse_execution_path(argc, argv, &execution_path);
    if (parse_ret != 0) {
        return 1;
    }

    printf("Starting delta job example.\n");
    printf("Creating delta comparing data regions of size 1KB.\n");

    dml_job_t *dml_job_ptr = NULL;
    uint32_t size = 0u;

    uint32_t delta_size = BUFFER_SIZE / DELTAS_PER_RECORD * DELTA_RECORD_SIZE;

    uint8_t source1     [BUFFER_SIZE];
    uint8_t source2     [BUFFER_SIZE];
    uint8_t delta       [delta_size];

    for(int i = 0; i < BUFFER_SIZE; i++){
        source1[i]  = i % 256;
        source2[i]  = 0;
    }

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
    
    dml_job_ptr->operation              = DML_OP_DELTA_CREATE;
    dml_job_ptr->source_first_ptr       = source1;
    dml_job_ptr->source_second_ptr      = source2;
    dml_job_ptr->destination_first_ptr  = delta;
    dml_job_ptr->source_length          = BUFFER_SIZE;
    dml_job_ptr->destination_length     = delta_size;

    status = dml_execute_job(dml_job_ptr, DML_WAIT_MODE_BUSY_POLL);
    if (DML_STATUS_OK != status) {
        printf("An error (%u) occured during job execution.\n", status);
        dml_finalize_job(dml_job_ptr);
        free(dml_job_ptr);
        return 1;
    }

    if(dml_job_ptr->result != 1){
        printf("Error: Operation result is incorrect.\n");
        dml_finalize_job(dml_job_ptr);
        free(dml_job_ptr);
        return 1;
    }
    
    printf("Completed creating delta.\n");
    printf("Applying created delta to data region of size 1KB\n");
    
    dml_job_ptr->operation              = DML_OP_DELTA_APPLY;
    dml_job_ptr->source_first_ptr       = delta;
    dml_job_ptr->source_length          = dml_job_ptr->destination_length;
    dml_job_ptr->destination_first_ptr  = source1;
    dml_job_ptr->destination_length     = BUFFER_SIZE;

    status = dml_execute_job(dml_job_ptr, DML_WAIT_MODE_BUSY_POLL);
    if (DML_STATUS_OK != status) {
        printf("An error (%u) occured during job execution.\n", status);
        dml_finalize_job(dml_job_ptr);
        free(dml_job_ptr);
        return 1;
    }

    for(int i = 0; i < BUFFER_SIZE; i++){
        if(source1[i] != source2[i]){
            printf("Error: Operation result is incorrect.\n");
            dml_finalize_job(dml_job_ptr);
            free(dml_job_ptr);
            return 1;
        }
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
}
