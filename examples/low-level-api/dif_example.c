/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "dml/dml.h"
#include "examples_utils.h"

#define BUFFER_SIZE     4096 // 4 KB
#define PROTECTED_SIZE  4104 // 4 KB data with DIF

/*
* This example demonstrates how to use the DIF operations.
* It shows how to insert, update, check, and strip a DIF.
*/

int main(int argc, char **argv)
{
    dml_path_t execution_path = DML_PATH_SW;

    // Get path from input argument
    int parse_ret = parse_execution_path(argc, argv, &execution_path);
    if (parse_ret != 0) {
        return 1;
    }

    printf("Starting DIF job example.\n");
    printf("Inserting DIF into data region of size 4KB.\n");

    dml_job_t *dml_job_ptr = NULL;
    uint32_t size = 0u;

    uint8_t data                    [PROTECTED_SIZE]; // Same size is used because of DML_STATUS_DIF_STRIP_ADJACENT_ERROR
    uint8_t protected_data          [PROTECTED_SIZE];
    uint8_t updated_protected_data  [PROTECTED_SIZE];

    for(int i = 0; i < BUFFER_SIZE; i++){
        data[i]  = i % 256;
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
    
    dml_job_ptr->operation              = DML_OP_DIF_INSERT;
    dml_job_ptr->source_first_ptr       = data;
    dml_job_ptr->source_length          = BUFFER_SIZE;
    dml_job_ptr->destination_first_ptr  = protected_data;
    dml_job_ptr->destination_length     = PROTECTED_SIZE;

    dml_job_ptr->dif_config.block_size  = DML_DIF_BLOCK_SIZE_4096;

    dml_job_ptr->dif_config.destination_application_tag_mask = 0x0u;
    dml_job_ptr->dif_config.destination_application_tag_seed = 0x0100;
    dml_job_ptr->dif_config.destination_reference_tag_seed   = 0u;

    dml_job_ptr->dif_config.flags = DML_DIF_FLAG_INVERT_CRC_SEED
                                    | DML_DIF_FLAG_INVERT_CRC_RESULT
                                    | DML_DIF_FLAG_DST_INC_APP_TAG
                                    | DML_DIF_FLAG_DST_FIX_REF_TAG;

    status = dml_execute_job(dml_job_ptr, DML_WAIT_MODE_BUSY_POLL);
    if (DML_STATUS_OK != status) {
        printf("An error (%u) occured during job execution.\n", status);
        dml_finalize_job(dml_job_ptr);
        free(dml_job_ptr);
        return 1;
    }

    if(dml_job_ptr->result != 0){
        printf("Error: Operation result is incorrect.\n");
        dml_finalize_job(dml_job_ptr);
        free(dml_job_ptr);
        return 1;
    }

    printf("Completed inserting DIF.\n");
    printf("Checking DIF on protected region of size 4104 bytes\n");
    
    dml_job_ptr->operation              = DML_OP_DIF_CHECK;
    dml_job_ptr->source_first_ptr       = protected_data;
    dml_job_ptr->source_length          = PROTECTED_SIZE;


    dml_job_ptr->dif_config.block_size  = DML_DIF_BLOCK_SIZE_4096;

    dml_job_ptr->dif_config.source_application_tag_mask = 0x0u;
    dml_job_ptr->dif_config.source_application_tag_seed = 0x0100;
    dml_job_ptr->dif_config.source_reference_tag_seed   = 0u;

    dml_job_ptr->dif_config.flags = DML_DIF_FLAG_INVERT_CRC_SEED
                                    | DML_DIF_FLAG_INVERT_CRC_RESULT
                                    | DML_DIF_FLAG_DST_INC_APP_TAG
                                    | DML_DIF_FLAG_DST_FIX_REF_TAG;

    status = dml_execute_job(dml_job_ptr, DML_WAIT_MODE_BUSY_POLL);
    if (DML_STATUS_OK != status) {
        printf("An error (%u) occured during job execution.\n", status);
        dml_finalize_job(dml_job_ptr);
        free(dml_job_ptr);
        return 1;
    }

    if(dml_job_ptr->result != 0){
        printf("Error: Operation result is incorrect.\n");
        dml_finalize_job(dml_job_ptr);
        free(dml_job_ptr);
        return 1;
    }

    printf("Completed checking DIF.\n");
    printf("Updating DIF on protected region of size 4104 bytes\n");
    
    dml_job_ptr->operation              = DML_OP_DIF_UPDATE;
    dml_job_ptr->source_first_ptr       = protected_data;
    dml_job_ptr->source_length          = PROTECTED_SIZE;
    dml_job_ptr->destination_first_ptr  = updated_protected_data;
    dml_job_ptr->destination_length     = PROTECTED_SIZE;
    


    dml_job_ptr->dif_config.block_size  = DML_DIF_BLOCK_SIZE_4096;

    dml_job_ptr->dif_config.source_application_tag_mask = 0x0u;
    dml_job_ptr->dif_config.source_application_tag_seed = 0x0100;
    dml_job_ptr->dif_config.source_reference_tag_seed   = 0u;

    dml_job_ptr->dif_config.destination_application_tag_mask = 0xFF00u;
    dml_job_ptr->dif_config.destination_application_tag_seed = 0x0200u;
    dml_job_ptr->dif_config.destination_reference_tag_seed   = 0u;

    dml_job_ptr->dif_config.flags = DML_DIF_FLAG_INVERT_CRC_SEED
                                    | DML_DIF_FLAG_INVERT_CRC_RESULT
                                    | DML_DIF_FLAG_SRC_GUARD_CHECK_DISABLE
                                    | DML_DIF_FLAG_SRC_REF_TAG_CHECK_DISABLE
                                    | DML_DIF_FLAG_DST_INC_APP_TAG
                                    | DML_DIF_FLAG_DST_FIX_REF_TAG;

    status = dml_execute_job(dml_job_ptr, DML_WAIT_MODE_BUSY_POLL);
    if (DML_STATUS_OK != status) {
        printf("An error (%u) occured during job execution.\n", status);
        dml_finalize_job(dml_job_ptr);
        free(dml_job_ptr);
        return 1;
    }

    if(dml_job_ptr->result != 0){
        printf("Error: Operation result is incorrect.\n");
        dml_finalize_job(dml_job_ptr);
        free(dml_job_ptr);
        return 1;
    }

    printf("Completed updating DIF.\n");
    printf("Stripping DIF from protected region of size 4104 bytes\n");
    
    dml_job_ptr->operation              = DML_OP_DIF_STRIP;
    dml_job_ptr->source_first_ptr       = updated_protected_data;
    dml_job_ptr->source_length          = PROTECTED_SIZE;
    dml_job_ptr->destination_first_ptr  = data;
    dml_job_ptr->destination_length     = BUFFER_SIZE;

    dml_job_ptr->dif_config.block_size  = DML_DIF_BLOCK_SIZE_4096;

    dml_job_ptr->dif_config.source_application_tag_mask = 0xFF00u;
    dml_job_ptr->dif_config.source_application_tag_seed = 0x0200u;
    dml_job_ptr->dif_config.source_reference_tag_seed   = 0u;

    dml_job_ptr->dif_config.flags = DML_DIF_FLAG_INVERT_CRC_SEED
                                    | DML_DIF_FLAG_INVERT_CRC_RESULT
                                    | DML_DIF_FLAG_DST_INC_APP_TAG
                                    | DML_DIF_FLAG_DST_FIX_REF_TAG;

    status = dml_execute_job(dml_job_ptr, DML_WAIT_MODE_BUSY_POLL);
    if (DML_STATUS_OK != status) {
        printf("An error %i occured during job execution.\n", status);
        dml_finalize_job(dml_job_ptr);
        free(dml_job_ptr);
        return 1;
    }

    for(int i = 0; i < BUFFER_SIZE; i++){
        if(data[i] != i % 256){
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
