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
 * @date 2/25/2020
 *
 * @defgroup core_public_cpu CPU Features
 * @ingroup core_public_features
 * @{
 * @brief Wrappers of CPU features.
 *
 * @details CPU features group includes the following functions:
 *      - Functions that get CPU info;
 *      - Wrappers for cache manipulation instructions;
 *      - etc.
 *
 */

#include "core_definitions.h"

#ifndef DML_KERNEL_CPU_FEATURES_H__
#define DML_KERNEL_CPU_FEATURES_H__

#ifdef __cplusplus
extern "C" {
#endif



#ifdef _WIN32

#include "intrin.h"

/**
 * @brief Return informantion about CPU based on arguments
 *
 * @param[out] info        - 4-int buffer containing result of CPUID (registers EAX, EBX, ECX, EDX)
 * @param[in] info_type    - value of EAX register, setting type of resulting information
 * @param[in] info_subtype - value of ECX register, setting subtype of resulting information
 *
 * @return
 *      Nothing
 *
 */
DML_CORE_OWN_INLINE(void, cpuid, (int info[4], int info_type, int info_subtype))
{
    __cpuidex(info, info_type, info_subtype);
}
#else

//  GCC Intrinsics 
#include <cpuid.h>
#include <dlfcn.h>

/**
 * @brief Return informantion about CPU based on arguments
 *
 * @param[out] info        - 4-int buffer containing result of CPUID (registers EAX, EBX, ECX, EDX)
 * @param[in] info_type    - value of EAX register, setting type of resulting information
 * @param[in] info_subtype - value of ECX register, setting subtype of resulting information
 *
 * @return
 *      Nothing
 *
 */
DML_CORE_OWN_INLINE(void, cpuid, (int info[4], int info_type, int info_subtype))
{
    __cpuid_count(info_type, info_subtype, info[0], info[1], info[2], info[3]);
}
#endif

/**
 * @brief Flushes the processor caches at the destination address with сache line invalidation from all cache hierarchy.
 *
 * @param[in] memory_region_ptr   memory region address to update from cache
 * @param[in] bytes_to_flush      memory region size, in bytes, to flush
 *
 * @return
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *
 */
DML_CORE_API(dmlc_status_t, move_cache_to_memory_8u, (const uint8_t  *memory_region_ptr,
                                                     const uint32_t bytes_to_flush));

/**
 * @brief Flushes the processor caches at the destination address without cache line invalidation.
 *
 * @param[in] memory_region_ptr - memory region address to update from cache
 * @param[in] bytes_to_flush    - memory region size, in bytes, to flush
 *
 * @return
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_NULL_POINTER_ERROR;
 *
 */
DML_CORE_API(dmlc_status_t, copy_cache_to_memory_8u, (const uint8_t  *memory_region_ptr,
                                                     const uint32_t bytes_to_flush));

/**
 * @brief Maximum cache size
 */
static int32_t max_cache_size = -1;

/**
 * @brief Returns max available cache size
 *
 * @param[out] size - pointer on resulting max cache size
 *
 * @return
 *      - @ref DML_STATUS_OK;
 *      - @ref DML_STATUS_UNKNOWN_CACHE_SIZE_ERROR;
 *
 */
DML_CORE_OWN_INLINE(dml_status_t, get_max_cache_size, (int32_t * size))
{
    if (max_cache_size > 0)
    {
        *size = max_cache_size;
        return DML_STATUS_OK;
    }
    int32_t tmp_max_size = 0;
    int32_t info[4] = {0, 0, 0, 0};

    for (int32_t n = 0; n < 32; n++) {
        dmlc_own_cpuid(info, 4, n);
        if (!(info[0] & 0x1f))
        {
            break;
        }
        if ((info[0] & 0x1f) != 2)
        {
            // Cache Size in Bytes = (Ways + 1) * (Partitions + 1) * (Line_Size + 1) * (Sets + 1)
            int32_t tmp_cache_size = info[2] + 1;              // Sets = ECX
            tmp_cache_size *= (info[1] & 0xfff) + 1;           // Line_Size = EBX[11:0]
            tmp_cache_size *= ((info[1] >> 12) & 0x3ff) + 1;   // Partitions = EBX[21:12]
            tmp_cache_size *= ((info[1] >> 22) & 0x3ff) + 1;   // Ways = EBX[31:22]
            if (tmp_cache_size > tmp_max_size) { tmp_max_size = tmp_cache_size; }
        }
    }
    if (tmp_max_size) 
    {
        max_cache_size = tmp_max_size;
        *size = tmp_max_size;
        return DML_STATUS_OK;
    }
    else 
    {
        *size = 0;
        return DML_STATUS_UNKNOWN_CACHE_SIZE_ERROR;
    }
}

#ifdef __cplusplus
}
#endif

#endif //DML_KERNEL_CPU_FEATURES_H__

/** @} */
