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
#include <signal.h>
#include <errno.h>
#include <libpmem.h> 

/**
 * @brief job state, init, submitted and executed
 * 
 */
typedef enum {
    JOB_INIT = 0,
    JOB_SUBMITTED = 1,
    JOB_EXECUTED =2
}job_state_t;

/**
 * @brief dml operation parameters
 * 
 */
struct dml_operations {
    uint32_t job_num;
    uint32_t block_size;
    uint32_t repeat;
    uint32_t stream;
    dml_job_t* jobs;
    uint8_t * src;
    uint8_t * dest;
    job_state_t * job_status; 
    uint32_t job_size;
    uint8_t is_dest_pmem;
    uint8_t is_src_pmem;
};

/**
 * @brief poll the job status, async
 * 
 */
struct poll_operations {
    uint32_t job_num;
    uint32_t job_size;
    dml_job_t *jobs;
    job_state_t *job_status;
    uint8_t exit;
};

/**
 * @brief for time calculation
 * 
 * @return __always_inline 
 */
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

/**
 * @brief calculate the cycles per sec
 * 
 * @param cycles_per_sec 
 */
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

/**
 * @brief thread to handle the operations with sync/wait for complete
 * 
 * @param param 
 * @return void* 
 */
static void * 
submit_dml_operations_sync(void * param) {
    struct dml_operations *operater = (struct dml_operations *)param;
    dml_job_t *jobs = operater->jobs;
    uint8_t   *src  = operater->src;
    uint8_t   *dst  = operater->dest;
    uint32_t REPEAT = operater->repeat;
    uint32_t JOB_NUM = operater->job_num;
    uint32_t block_size = operater->block_size;
    uint32_t job_size = operater->job_size;
    //uint8_t is_src_pmem = operater->is_src_pmem;
    uint8_t is_dst_pmem = operater->is_dest_pmem;
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
            if (is_dst_pmem) current_job->flags |= DML_FLAG_DST1_DURABLE;
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
                    printf("Operation status is %d\n", status);
                }
            }
	    }
    }
    return NULL;
}

/**
 * @brief thread to operate dml jobs, only submit job and poll thread
 * check if the jobs are finished. 
 * 
 * @param param 
 * @return void* 
 */
static void * 
submit_dml_operations_async(void * param) {
    struct dml_operations *operater = (struct dml_operations *)param;
    dml_job_t *jobs = operater->jobs;
    uint8_t   *src  = operater->src;
    uint8_t   *dst  = operater->dest;
    uint32_t REPEAT = operater->repeat;
    uint32_t JOB_NUM = operater->job_num;
    uint32_t block_size = operater->block_size;
    uint32_t job_size = operater->job_size;
    volatile job_state_t *job_status = operater->job_status;
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
            job_status[i] = JOB_SUBMITTED; //job submitted. 
        }

        // wait all the job be  executed then go to the next iteration.
        for (i = 0; i < JOB_NUM; i++) {
            while (job_status[i] != JOB_EXECUTED) 
                ;
        }
    }
    return NULL;
}

/**
 * @brief poll all the jobs to see if the jobs finished. 
 * 
 * @param param 
 * @return void* 
 */
static void * 
poll_operations(void * param) {
    struct poll_operations *polljob = (struct poll_operations *)param;
    dml_job_t *jobs = polljob->jobs;
    uint32_t JOB_NUM = polljob->job_num;
    volatile job_state_t * job_status = polljob->job_status;
    uint32_t job_size = polljob->job_size;
    dml_status_t status;
    uint32_t i;
    while(!polljob->exit) {
        for (i=0;i<JOB_NUM;i++) {
            if(job_status[i] == JOB_SUBMITTED) {
                status = dml_check_job((dml_job_t *)((uint8_t*)jobs + (job_size * i)));
                if(status == DML_STATUS_OK) {
                    job_status[i] = JOB_EXECUTED;
                    //callback function;
                }
            }
        }
    }
}

/**
 * @brief perf test main function
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
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
    uint32_t i, j;
    pthread_t poll_thread;
    struct poll_operations polljob;
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

    char *pmemaddr;
	size_t mapped_len;
	int is_pmem;
    /* create a pmem file and memory map it */
	if ((pmemaddr = pmem_map_file("/mnt/pmem0/dml_test", transfer_size * thread_num,
				PMEM_FILE_CREATE,
				0666, &mapped_len, &is_pmem)) == NULL) {
		printf("pmem_map_file failed\n");
        exit_status = EXIT_FAILURE;
		goto cleanup;
	}

    printf("=======================================================================================\n");
    //create threads for operation
    threads = malloc(thread_num * sizeof(pthread_t));
    operater = malloc(thread_num * sizeof(struct dml_operations));
    tstart = rdtsc();
    for (i=0;i<thread_num;i++) {
        operater[i].block_size = block_size;
        operater[i].repeat = REPEAT;
        operater[i].job_num = JOB_NUM;
        operater[i].src = src + JOB_NUM * i;
        operater[i].dest = dst + JOB_NUM * i;
        operater[i].job_size = job_size;
        operater[i].stream = stream;
        operater[i].jobs =  (dml_job_t*)((uint8_t*)jobs + (job_size * JOB_NUM * i));
        operater[i].is_src_pmem = 0;
        operater[i].is_dest_pmem = 0;

        printf("jobs=%p\n",operater[i].jobs);
        ret = pthread_create(&threads[i],NULL, submit_dml_operations_sync, &operater[i]);
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
    printf("dram->dram, multi-thread sync, job_num=%d, Block_size=%d, REPEAT=%d, thread=%d, BW = %f MB/s \n", 
		    JOB_NUM, block_size, REPEAT, thread_num, trans_data/time_c);
    
    //if stream, need wait all job finished avoid affect the next perf test
    if(stream) {
        for (i = 0; i < JOB_NUM * thread_num; ++i)
        {
            status = dml_wait_job((dml_job_t*)((uint8_t*)jobs + (job_size * i)), DML_WAIT_MODE_UMWAIT);
            if (status != DML_STATUS_OK)
            {
                printf("Operation status is %d\n", status);
            }
        }
	}
    printf("=======================================================================================\n");
    
    tstart = rdtsc();
    for (i=0;i<thread_num;i++) {
        operater[i].block_size = block_size;
        operater[i].repeat = REPEAT;
        operater[i].job_num = JOB_NUM;
        operater[i].src = src + JOB_NUM * i;
        operater[i].dest = pmemaddr + JOB_NUM * i;
        operater[i].job_size = job_size;
        operater[i].stream = stream;
        operater[i].jobs =  (dml_job_t*)((uint8_t*)jobs + (job_size * JOB_NUM * i));
        operater[i].is_src_pmem = 0;
        operater[i].is_dest_pmem = 1;

        printf("jobs=%p\n",operater[i].jobs);
        ret = pthread_create(&threads[i],NULL, submit_dml_operations_sync, &operater[i]);
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
    time_c = (float)(tend-tstart)/(float)(cycles_per_sec);
    trans_data = (float)(block_size/1024*REPEAT*JOB_NUM*thread_num)/1024.0;
    printf("time_c=%f, trans_data=%f MB\n", time_c, trans_data);
    printf("dram->pmem, multi-thread sync, job_num=%d, Block_size=%d, REPEAT=%d, thread=%d, BW = %f MB/s \n", 
		    JOB_NUM, block_size, REPEAT, thread_num, trans_data/time_c);
    
    //if stream, need wait all job finished avoid affect the next perf test
    if(stream) {
        for (i = 0; i < JOB_NUM * thread_num; ++i)
        {
            status = dml_wait_job((dml_job_t*)((uint8_t*)jobs + (job_size * i)), DML_WAIT_MODE_UMWAIT);
            if (status != DML_STATUS_OK)
            {
                printf("Operation status is %d\n", status);
            }
        }
	}
    printf("=======================================================================================\n");
    
    job_state_t *job_status = malloc(JOB_NUM * thread_num *sizeof(job_state_t));
    memset(job_status, JOB_INIT, JOB_NUM * thread_num);

    polljob.job_num = JOB_NUM * thread_num;
    polljob.jobs = jobs;
    polljob.job_size = job_size;
    polljob.job_status = job_status;
    polljob.exit = 0;

    ret = pthread_create(&poll_thread,NULL, poll_operations, &polljob);
    if(ret !=0) {
        printf("%d thread create failed\n",i);
        return 0;
    }

    tstart = rdtsc();
    for (i=0;i<thread_num;i++) {
        operater[i].block_size = block_size;
        operater[i].repeat = REPEAT;
        operater[i].job_num = JOB_NUM;
        operater[i].src = src + JOB_NUM;
        operater[i].dest = dst + JOB_NUM;
        operater[i].job_size = job_size;
        operater[i].stream = stream;
        operater[i].jobs =  (dml_job_t*)((uint8_t*)jobs + (job_size * JOB_NUM * i));
        operater[i].job_status = (job_state_t *)((uint8_t *)job_status  + sizeof(job_state_t) * JOB_NUM * i);

        printf("jobs=%p\n",operater[i].jobs);
        ret = pthread_create(&threads[i],NULL, submit_dml_operations_async, &operater[i]);
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
    time_c = (float)(tend-tstart)/(float)(cycles_per_sec);
    trans_data = (float)(block_size/1024*REPEAT*JOB_NUM*thread_num)/1024.0;
    printf("time_c=%f, trans_data=%f MB\n", time_c, trans_data);
    printf("dram->dram, multi-thread async, job_num=%d, Block_size=%d, REPEAT=%d, thread=%d, BW = %f MB/s \n", 
		    JOB_NUM, block_size, REPEAT, thread_num, trans_data/time_c);

    polljob.exit = 1;
    int kill_rc;
    while((kill_rc = pthread_kill(poll_thread,0)) != ESRCH) 
        ;
    printf("the poll_thread did not exists or already quit\n");
    printf("=======================================================================================\n");
    
    memset(job_status, JOB_INIT, JOB_NUM * thread_num);
    polljob.job_num = JOB_NUM * thread_num;
    polljob.jobs = jobs;
    polljob.job_size = job_size;
    polljob.job_status = job_status;
    polljob.exit = 0;

    ret = pthread_create(&poll_thread,NULL, poll_operations, &polljob);
    if(ret !=0) {
        printf("%d thread create failed\n",i);
        return 0;
    }

    tstart = rdtsc();
    for (i=0;i<thread_num;i++) {
        operater[i].block_size = block_size;
        operater[i].repeat = REPEAT;
        operater[i].job_num = JOB_NUM;
        operater[i].src = src + JOB_NUM * i;
        operater[i].dest = pmemaddr + JOB_NUM * i;
        operater[i].job_size = job_size;
        operater[i].stream = stream;
        operater[i].jobs =  (dml_job_t*)((uint8_t*)jobs + (job_size * JOB_NUM * i));
        operater[i].job_status = (job_state_t *)((uint8_t *)job_status  + sizeof(job_state_t) * JOB_NUM * i);
        operater[i].is_src_pmem = 0;
        operater[i].is_dest_pmem = 1;

        printf("jobs=%p\n",operater[i].jobs);
        ret = pthread_create(&threads[i],NULL, submit_dml_operations_async, &operater[i]);
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
    time_c = (float)(tend-tstart)/(float)(cycles_per_sec);
    trans_data = (float)(block_size/1024*REPEAT*JOB_NUM*thread_num)/1024.0;
    printf("time_c=%f, trans_data=%f MB\n", time_c, trans_data);
    printf("dram->pmem, multi-thread async, job_num=%d, Block_size=%d, REPEAT=%d, thread=%d, BW = %f MB/s \n", 
		    JOB_NUM, block_size, REPEAT, thread_num, trans_data/time_c);

    polljob.exit = 1;
    while((kill_rc = pthread_kill(poll_thread,0)) != ESRCH) 
        ;
    printf("the poll_thread did not exists or already quit\n");
    printf("=======================================================================================\n");   
    
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
    pmem_unmap(pmemaddr, mapped_len);
    return exit_status;
}
