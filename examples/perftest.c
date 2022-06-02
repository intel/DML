/*******************************************************************************
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml/dml.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <x86intrin.h>
#include <malloc.h>
#include <pthread.h>

//4k datablock for transfer
struct dml_operations {
    uint32_t job_num;
    uint32_t block_size;
    uint32_t repeat;
    uint32_t stream;
    dml_job_t* jobs;
    uint8_t * src;
    uint8_t * dest;
    uint32_t job_size;
};

static __always_inline uint64_t
rdtsc(void)
{
	uint64_t tsc;
	unsigned int dummy;

	/*
	 * https://www.felixcloutier.com/x86/rdtscp
	 * The RDTSCP instruction is not a serializing instruction, but it
	 * does wait until all previous instructions have executed and all
	 * previous loads are globally visible
	 *
	 * If software requires RDTSCP to be executed prior to execution of
	 * any subsequent instruction (including any memory accesses), it can
	 * execute LFENCE immediately after RDTSCP
	 */
	tsc = __rdtscp(&dummy);
	__builtin_ia32_lfence();

	return tsc;
}

static inline uint64_t
get_ms(void)
{
	struct timeval tp;

	gettimeofday(&tp, NULL);

	return tp.tv_sec*1000+tp.tv_usec/1000;
}

static void
calibrate(uint64_t *cycles_per_sec)
{
	uint64_t  start;
	uint64_t  end;
	uint64_t starttick, endtick;
	uint64_t ms_diff, cycle_diff;

	endtick = get_ms();

	while (endtick == (starttick = get_ms()))
		;

	/* Measure cycle diff for 500 ms */
	start = rdtsc();
	while ((endtick = get_ms())  < (starttick + 500))
		;
	end = rdtsc();

	cycle_diff = end - start;
	ms_diff = endtick - starttick;

	/* ms * cycles_per_sec = cycle_diff * 1000 */

	*cycles_per_sec = (cycle_diff * (uint64_t)1000)/ms_diff;
}

static void * 
submit_dml_operations(void * param) {
    struct dml_operations *operater = (struct dml_operations *)param;
    dml_job_t *jobs = operater->jobs;
    uint8_t   *src  = operater->src;
    uint8_t   *dst  = operater->dest;
    uint32_t REPEAT = operater->repeat;
    uint32_t JOB_NUM = operater->job_num;
    uint32_t block_size = operater->block_size;
    uint32_t job_size = operater->job_size;
    uint32_t i,j;
    printf("Starting example for multi-job memory move jobs=%p:\n",jobs);

    uint32_t transfer_size = block_size * JOB_NUM;
    dml_status_t status;

    for (j = 0; j < REPEAT; j++) {
        
        for (i = 0; i < JOB_NUM; ++i) 
        {
            const uint32_t chunk_size = transfer_size / JOB_NUM;
            dml_job_t* current_job = (dml_job_t*)((uint8_t*)jobs + (job_size * i));
            current_job->operation             = DML_OP_MEM_MOVE;
            current_job->source_first_ptr      = src + (chunk_size * i);
            current_job->destination_first_ptr = dst + (chunk_size * i);
            current_job->source_length         = chunk_size;
            current_job->flags                 = 12|DML_FLAG_PREFETCH_CACHE;
            //current_job->numa_id               = i;
        }

        for (i = 0; i < JOB_NUM; ++i)
        {
            status = dml_submit_job((dml_job_t*)((uint8_t*)jobs + (job_size * i)));
            if (status != DML_STATUS_OK)
            {
                printf("Failed to submit to node %d", i);
            }
        }

        if(!operater->stream) {
            for (i = 0; i < JOB_NUM; ++i)
            {
                status = dml_wait_job((dml_job_t*)((uint8_t*)jobs + (job_size * i)), DML_WAIT_MODE_UMWAIT);
                if (status != DML_STATUS_OK)
                {
                    printf("Operation status is %d", status);
                }
            }
	    }
    }
    return NULL;
}

//int main(int argc, char * argv[])
int main(int argc, char * argv[])
{
    int exit_status = EXIT_SUCCESS;
    int ret;
    dml_job_t *jobs = NULL;
    uint8_t   *src  = NULL;
    uint8_t   *dst  = NULL;
    uint64_t tstart, tend;
	
    uint32_t JOB_NUM, REPEAT, block_size,stream;
    uint64_t cycles_per_sec;
    uint32_t thread_num;
    pthread_t *threads;
    uint32_t i;

    struct dml_operations *operater;

    calibrate(&cycles_per_sec);

    if(argc < 6) {
    	printf("usage: perf_test job_num repeat blsize stream(1 or 0) thread_num \n");
        return -1;
    } else {
    	JOB_NUM = atoi(argv[1]);
        REPEAT = atoi(argv[2]);
        block_size= atoi(argv[3]);	
	    stream=atoi(argv[4]);
        thread_num = atoi(argv[5]);
    }
    
    uint32_t transfer_size = block_size * JOB_NUM;
    uint32_t job_size = 0;
    dml_status_t status = dml_get_job_size(DML_PATH_AUTO, &job_size);
    if (status != DML_STATUS_OK)
    {
        printf("Failed to get job size\n");
        exit_status = EXIT_FAILURE;
        goto finish;
    }

    jobs = (dml_job_t*)malloc(job_size * JOB_NUM * thread_num);
    if (jobs == NULL)
    {
        printf("Failed to allocate memory for jobs\n");
        exit_status = EXIT_FAILURE;
        goto finish;
    }

    for (uint32_t i = 0; i < JOB_NUM * thread_num; ++i)
    {
        status = dml_init_job(DML_PATH_AUTO, (dml_job_t*)((uint8_t*)jobs + (job_size * i)));
        if (status != DML_STATUS_OK)
        {
            printf("Failed to initialize job %d\n", i);
            exit_status = EXIT_FAILURE;
            goto finish;
        }
    }

    src = (uint8_t*)memalign(4096, transfer_size * thread_num);
    if (src == NULL)
    {
        exit_status = EXIT_FAILURE;
        printf("Failed to allocate source memory\n");
        goto cleanup;
    }

    dst = (uint8_t*)memalign(4096, transfer_size * thread_num);
    if (dst == NULL)
    {
        exit_status = EXIT_FAILURE;
        printf("Failed to allocate destination memory\n");
        goto cleanup;
    }
    printf("allocatate the 4096 aligned src=%p, dst=%p\n",src, dst);

    memset(src, 1, transfer_size);
    memset(dst, 0, transfer_size);

    //create threads for operation
    
    threads = malloc(thread_num * sizeof(pthread_t));
    operater = malloc(thread_num * sizeof(struct dml_operations));
    
    tstart = rdtsc();
    for(i=0;i<thread_num;i++) {
        operater[i].block_size = block_size;
        operater[i].repeat = REPEAT;
        operater[i].job_num = JOB_NUM;
        operater[i].src = src + JOB_NUM;
        operater[i].dest = dst + JOB_NUM;
        operater[i].job_size = job_size;
        operater[i].stream = stream;
        operater[i].jobs =  (dml_job_t*)((uint8_t*)jobs + (job_size * JOB_NUM * i));

        printf("jobs=%p\n",operater[i].jobs);
        ret = pthread_create(&threads[i],NULL, submit_dml_operations, &operater[i]);
        if(ret !=0) {
            printf("%d thread create failed\n",i);
            return 0;
        }
     }

    for(i=0;i<thread_num;i++) {
        pthread_join(threads[i],NULL);
    }

    tend = rdtsc();
    printf("time=%ld, cycles=%ld, cycles_per_sec=%ld\n",(tend-tstart), (tend-tstart)/REPEAT/JOB_NUM/thread_num, cycles_per_sec);
    float time_c = (float)(tend-tstart)/(float)(cycles_per_sec);
    float trans_data = (float)(block_size/1024*REPEAT*JOB_NUM*thread_num)/1024.0;
    printf("time_c=%f, trans_data=%f MB\n", time_c, trans_data);
    printf("w cache control flag, job_num=%d, Block_size=%d, REPEAT=%d, thread=%d, BW = %f MB/s \n", 
		    JOB_NUM, block_size, REPEAT, thread_num, trans_data/time_c);

    if (memcmp(src, dst, transfer_size) != 0)
    {
        printf("Fail!\n");
        exit_status = EXIT_FAILURE;
    }
    else
    {
        printf("Success!\n");
        exit_status = EXIT_SUCCESS;
    }

cleanup:
    for (i = 0; i < JOB_NUM * thread_num; ++i)
    {
        dml_finalize_job(jobs + i);
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
