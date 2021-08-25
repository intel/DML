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
 * @brief Contains a constant, which are used to connect with hardware
 * @date 3/23/2020
 *
 */

#include <stdint.h>

#ifndef DML_DSA_ACCEL_CONFIG_H__
#define DML_DSA_ACCEL_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#define DSA_DEVICE_ID ((uint32_t) (((uint32_t)0xFF << 24u)    \
                                   | ((uint32_t)('a') << 16u) \
                                   | ((uint32_t)('s') << 8u)  \
                                   | (uint32_t)('d')))

#define CHAR_MSK              0xFF202020
#define MAX_DEVICE_COUNT      100u
#define MAX_WORK_QUEUE_COUNT  100u
#define OWN_PAGE_MASK         0x0FFFllu     /**< Defines page mask for portal incrementing */

// General Capabilities Register unwrappers
#define GC_BLOCK_ON_FAULT_SUP(gen_cap)           ((gen_cap >> 0u) & 0x0000000000000001ull)
#define GC_OVERLAPPING_COPY_SUPPORT(gen_cap)     ((gen_cap >> 1u) & 0x0000000000000001ull)
#define GC_M_CACHE_CONTROL_SUPPORT(gen_cap)      ((gen_cap >> 2u) & 0x0000000000000001ull)
#define GC_F_CACHE_CONTROL_SUPPORT(gen_cap)      ((gen_cap >> 3u) & 0x0000000000000001ull)
#define GC_INTERRUPT_HANDLE_REQUEST(gen_cap)     ((gen_cap >> 7u) & 0x0000000000000001ull)
#define GC_DESTINATION_READBACK_SUPPORT(gen_cap) ((gen_cap >> 8u) & 0x0000000000000001ull)
#define GC_DESCRIPTOR_READBACK_SUPPORT(gen_cap)  ((gen_cap >> 9u) & 0x0000000000000001ull)
#define GC_MAX_TRANSFER_SIZE(gen_cap)            (1u << ((gen_cap >> 16u) & 0x000000000000001Full))
#define GC_MAX_BATCH_SIZE(gen_cap)               (1u << ((gen_cap >> 21u) & 0x000000000000000Full))
#define GC_MESSAGE_SIZE(gen_cap)                 (256u * ((gen_cap >> 25u) & 0x000000000000003Full))
#define GC_CONFIGURATION_SUPPORT(gen_cap)        ((gen_cap >> 31u) & 0x0000000000000001ull)
#define GC_MAX_DESCRIPTORS(gen_cap)              ((gen_cap >> 32u) & 0x00000000000000FFull)

static const char     *DLL_NAME          = "/usr/lib64/libaccel-config.so";
static const char     DEVICE_NAME[]      = "dsa";
static const uint32_t DEVICE_NAME_LENGTH =
                              sizeof(DEVICE_NAME) - 2u; //sizeof will return 4, position of terminating 0 is 3

#ifdef __cplusplus
}
#endif

#endif //DML_DSA_ACCEL_CONFIG_H__
