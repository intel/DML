/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_SOURCES_HW_PATH_INCLUDE_HW_CONFIGURATION_DRIVER_H_
#define DML_SOURCES_HW_PATH_INCLUDE_HW_CONFIGURATION_DRIVER_H_

#include "hardware_definitions.h"

#if defined(linux)

#include "libaccel_config.h"

#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Type of function that should be loaded from accelerator configuration driver
 */
typedef int (*library_function)();

/**
 * @brief Structure that maps function implementation to its name
 */
typedef struct
{
    library_function function;      /**< Function address */
    const char      *function_name; /**< Function name */

} dsa_desc_t;

/**
 * @brief Structure represents configuration driver used for access to accelerator instances and their properties
 */
typedef struct
{
    void *driver_instance_ptr; /**< Pointer to a loaded driver */
} hw_driver_t;

/**
 * @brief Initializes driver functions
 *
 * @note Should be called only once
 *
 * @return status of initialization
 */
dsahw_status_t DML_HW_API(initialize_accelerator_driver)(hw_driver_t *driver_ptr);

void DML_HW_API(finalize_accelerator_driver)(hw_driver_t *driver_ptr);

int32_t DML_HW_API(driver_new_context)(struct accfg_ctx **ctx);

struct accfg_device *DML_HW_API(context_get_first_device)(struct accfg_ctx *ctx);

struct accfg_ctx *DML_HW_API(context_close)(struct accfg_ctx *ctx);

const char *DML_HW_API(device_get_name)(struct accfg_device *device);

struct accfg_device *DML_HW_API(device_get_next)(struct accfg_device *device);

uint32_t DML_HW_API(device_get_version)(struct accfg_device *device);

enum accfg_device_state DML_HW_API(device_get_state)(struct accfg_device *device);

uint64_t DML_HW_API(device_get_gen_cap_register)(struct accfg_device *device);

uint64_t DML_HW_API(device_get_numa_node)(struct accfg_device *device);

struct accfg_wq *DML_HW_API(get_first_work_queue)(struct accfg_device *device);

struct accfg_wq *DML_HW_API(work_queue_get_next)(struct accfg_wq *wq);

const char * DML_HW_API(work_queue_get_device_name)(struct accfg_wq *wq);

int32_t DML_HW_API(work_queue_get_priority)(struct accfg_wq *wq);

enum accfg_wq_state DML_HW_API(work_queue_get_state)(struct accfg_wq *wq);

enum accfg_wq_mode DML_HW_API(work_queue_get_mode)(struct accfg_wq *wq);

struct accfg_group *DML_HW_API(group_get_first)(struct accfg_device *device);

struct accfg_group *DML_HW_API(group_get_next)(struct accfg_group *group);

int DML_HW_API(group_get_traffic_class_a)(struct accfg_group *group);

int DML_HW_API(group_get_traffic_class_b)(struct accfg_group *group);

struct accfg_group *DML_HW_API(work_queue_get_group)(struct accfg_wq *wq);

int DML_HW_API(work_queue_get_group_id)(struct accfg_wq *wq);

int DML_HW_API(group_get_id)(struct accfg_group *group);

int DML_HW_API(work_queue_get_device_path)(struct accfg_wq *wq, char *buf, size_t size);

#ifdef __cplusplus
}
#endif

#endif //DML_SOURCES_HW_PATH_INCLUDE_HW_CONFIGURATION_DRIVER_H_