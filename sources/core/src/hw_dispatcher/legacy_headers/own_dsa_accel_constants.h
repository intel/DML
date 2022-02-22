/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

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

#define DSA_DEVICE_ID ((uint32_t)(((uint32_t)0xFF << 24u) | ((uint32_t)('a') << 16u) | ((uint32_t)('s') << 8u) | (uint32_t)('d')))

#define CHAR_MSK             0xFF202020
#define MAX_DEVICE_COUNT     8u
#define MAX_WORK_QUEUE_COUNT 8u
#define OWN_PAGE_MASK        0x0FFFllu /**< Defines page mask for portal incrementing */

// General Capabilities Register unwrappers
#define GC_BLOCK_ON_FAULT(GENCAP)           (((GENCAP))    &0x01)       /**< GENCAP bit 0      - block on fault support                    */
#define GC_OVERLAPPING(GENCAP)              (((GENCAP)>>1) &0x01)       /**< GENCAP bit 1      - overlapping copy support                  */
#define GC_CACHE_WRITE(GENCAP)              (((GENCAP)>>2) &0x01)       /**< GENCAP bit 2      - cache control support (memory)            */
#define GC_CACHE_FLUSH(GENCAP)              (((GENCAP)>>3) &0x01)       /**< GENCAP bit 3      - cache control support (cache flush)       */
#define GC_COM_CAP(GENCAP)                  (((GENCAP)>>4) &0x01)       /**< GENCAP bit 4      - command capabilities support              */
#define GC_DST_READBACK(GENCAP)             (((GENCAP)>>8) &0x01)       /**< GENCAP bit 8      - destination readback support              */
#define GC_DRAIN_READBACK(GENCAP)           (((GENCAP)>>9) &0x01)       /**< GENCAP bit 9      - drain descriptor readback address support */
#define GC_MAX_TRANSFER_SIZE(GENCAP)  (1 << (((GENCAP)>>16)&0x1F))      /**< GENCAP 20-16 bits - maximum supported transfer size           */
#define GC_MAX_BATCH_SIZE(GENCAP)     (1 << (((GENCAP)>>21)&0x0F))      /**< GENCAP 24-21 bits - maximum supported batch size              */
#define GC_INTERRUPT_STORAGE(GENCAP)       ((((GENCAP)>>25)&0x3F)*256u) /**< GENCAP 30-25 bits - interrupt message storage size            */
#define GC_CONF_SUPPORT(GENCAP)             (((GENCAP)>>31)&0x01)       /**< GENCAP bit 31     - configuration support                     */

static const char     DEVICE_NAME[]      = "dsa";
static const uint32_t DEVICE_NAME_LENGTH = sizeof(DEVICE_NAME) - 2u;  //sizeof will return 4, position of terminating 0 is 3

#ifdef __cplusplus
}
#endif

#endif  //DML_DSA_ACCEL_CONFIG_H__
