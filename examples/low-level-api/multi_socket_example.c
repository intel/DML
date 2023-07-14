/*******************************************************************************
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml/dml.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOCKET_COUNT 4u

// 1 MB per socket
static const uint32_t transfer_size = 1024u * 1024 * SOCKET_COUNT;

int main()
{
    int exit_status = EXIT_SUCCESS;

    dml_job_t *jobs = NULL;
    uint8_t   *src  = NULL;
    uint8_t   *dst  = NULL;

    printf("Starting example for multi-socket memory move:\n");

    uint32_t job_size = 0;

    dml_status_t status = dml_get_job_size(DML_PATH_AUTO, &job_size);
    if (status != DML_STATUS_OK)
    {
        printf("Failed to get job size\n");
        exit_status = EXIT_FAILURE;
        goto finish;
    }

    jobs = (dml_job_t*)malloc(job_size * SOCKET_COUNT);
    if (jobs == NULL)
    {
        printf("Failed to allocate memory for jobs\n");
        exit_status = EXIT_FAILURE;
        goto finish;
    }

    for (uint32_t i = 0; i < SOCKET_COUNT; ++i)
    {
        status = dml_init_job(DML_PATH_AUTO, (dml_job_t*)((uint8_t*)jobs + (job_size * i)));
        if (status != DML_STATUS_OK)
        {
            printf("Failed to initialize job %d\n", i);
            exit_status = EXIT_FAILURE;
            goto finish;
        }
    }

    src = (uint8_t*)malloc(transfer_size);
    if (src == NULL)
    {
        exit_status = EXIT_FAILURE;
        printf("Failed to allocate source memory\n");
        goto cleanup;
    }

    dst = (uint8_t*)malloc(transfer_size);
    if (dst == NULL)
    {
        exit_status = EXIT_FAILURE;
        printf("Failed to allocate destination memory\n");
        goto cleanup;
    }

    memset(src, 1, transfer_size);
    memset(dst, 0, transfer_size);

    for (uint32_t i = 0; i < SOCKET_COUNT; ++i)
    {
        const uint32_t chunk_size = transfer_size / SOCKET_COUNT;

        dml_job_t* current_job = (dml_job_t*)((uint8_t*)jobs + (job_size * i));

        current_job->operation             = DML_OP_MEM_MOVE;
        current_job->source_first_ptr      = src + (chunk_size * i);
        current_job->destination_first_ptr = dst + (chunk_size * i);
        current_job->source_length         = chunk_size;
        current_job->flags                 = DML_FLAG_PREFETCH_CACHE;
        current_job->numa_id               = i;
    }

    for (uint32_t i = 0; i < SOCKET_COUNT; ++i)
    {
        status = dml_submit_job((dml_job_t*)((uint8_t*)jobs + (job_size * i)));
        if (status != DML_STATUS_OK)
        {
            printf("Failed to submit to node %d", i);
            exit_status = EXIT_FAILURE;
            goto cleanup;
        }
    }

    for (uint32_t i = 0; i < SOCKET_COUNT; ++i)
    {
        status = dml_wait_job((dml_job_t*)((uint8_t*)jobs + (job_size * i)), DML_WAIT_MODE_BUSY_POLL);
        if (status != DML_STATUS_OK)
        {
            printf("Operation status is %d", status);
            exit_status = EXIT_FAILURE;
            goto cleanup;
        }
    }

    if (memcmp(src, dst, transfer_size) != 0)
    {
        printf("Fail!");
        exit_status = EXIT_FAILURE;
    }
    else
    {
        printf("Success!");
        exit_status = EXIT_SUCCESS;
    }

cleanup:
    for (uint32_t i = 0; i < SOCKET_COUNT; ++i)
    {
        dml_finalize_job(jobs + SOCKET_COUNT);
    }

finish:
    if (jobs)
    {
        free(jobs);
    }
    if (src)
    {
        free(src);
    }
    if (dst)
    {
        free(dst);
    }

    return exit_status;
}
