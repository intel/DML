/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <fcntl.h>

#include "legacy_headers/hardware_configuration_driver.h"

#if defined(linux)

#include <dlfcn.h>
#include <sys/mman.h>

const static char *accelerator_configuration_driver_name = "/usr/lib64/libaccel-config.so.1";

typedef int (*accfg_new_ptr)(struct accfg_ctx **ctx);

typedef struct accfg_device *(*accfg_device_get_first_ptr)(struct accfg_ctx *ctx);

typedef const char *(*accfg_device_get_devname_ptr)(struct accfg_device *device);

typedef struct accfg_device *(*accfg_device_get_next_ptr)(struct accfg_device *device);

typedef struct accfg_wq *(*accfg_wq_get_first_ptr)(struct accfg_device *device);

typedef struct accfg_wq *(*accfg_wq_get_next_ptr)(struct accfg_wq *wq);

typedef enum accfg_wq_state (*accfg_wq_get_state_ptr)(struct accfg_wq *wq);

typedef unsigned int (*accfg_device_get_version_ptr)(struct accfg_device *device);

typedef const char * (*accfg_wq_get_devname_ptr)(struct accfg_wq *wq);

typedef enum accfg_device_state (*accfg_device_get_state_ptr)(struct accfg_device *device);

typedef struct accfg_ctx *(*accfg_unref_ptr)(struct accfg_ctx *ctx);

typedef enum accfg_wq_mode (*accfg_wq_get_mode_ptr)(struct accfg_wq *wq);

typedef unsigned long (*accfg_device_get_gen_cap_ptr)(struct accfg_device *device);

typedef int (*accfg_group_get_traffic_class_ptr)(struct accfg_group *group);

typedef struct accfg_group *(*accfg_group_get_first_ptr)(struct accfg_device *device);

typedef struct accfg_group *(*accfg_group_get_next_ptr)(struct accfg_group *group);

typedef struct accfg_group *(*accfg_wq_get_group_ptr)(struct accfg_wq *wq);

typedef int (*accfg_wq_get_group_id_ptr)(struct accfg_wq *wq);

typedef int (*accfg_group_get_id_ptr)(struct accfg_group *group);

typedef int (*accfg_wq_get_user_dev_path_ptr)(struct accfg_wq *wq, char *buf, size_t size);

typedef int (*accfg_wq_get_priority_ptr)(struct accfg_wq *wq);

/**
 * @brief Table with functions required from accelerator configuration library
 */
static dsa_desc_t functions_table[] = { { NULL, "accfg_new" },
                                        { NULL, "accfg_device_get_first" },
                                        { NULL, "accfg_device_get_devname" },
                                        { NULL, "accfg_device_get_next" },
                                        { NULL, "accfg_wq_get_first" },
                                        { NULL, "accfg_wq_get_next" },
                                        { NULL, "accfg_wq_get_state" },
                                        { NULL, "accfg_wq_get_mode" },
                                        { NULL, "accfg_device_get_version" },
                                        { NULL, "accfg_wq_get_devname" },
                                        { NULL, "accfg_device_get_state" },
                                        { NULL, "accfg_unref" },
                                        { NULL, "accfg_device_get_gen_cap" },
                                        { NULL, "accfg_device_get_numa_node" },
                                        { NULL, "accfg_wq_get_priority" },
                                        { NULL, "accfg_group_get_first" },
                                        { NULL, "accfg_group_get_next" },
                                        { NULL, "accfg_group_get_traffic_class_a" },
                                        { NULL, "accfg_group_get_traffic_class_b" },
                                        { NULL, "accfg_wq_get_group" },
                                        { NULL, "accfg_wq_get_group_id" },
                                        { NULL, "accfg_group_get_id" },
                                        { NULL, "accfg_wq_get_user_dev_path" },
                                        // Terminate list/init
                                        { NULL, NULL } };

static inline dsahw_status_t own_load_accelerator_configuration_driver(void **driver_instance_pptr);

static inline bool own_load_configuration_functions(void *driver_instance_ptr);

#endif

dsahw_status_t DML_HW_API(initialize_accelerator_driver)(hw_driver_t *driver_ptr)
{
#if defined(linux)
    // Variables
    driver_ptr->driver_instance_ptr = NULL;

    // Load DLL
    dsahw_status_t status = own_load_accelerator_configuration_driver(&driver_ptr->driver_instance_ptr);

    // If DLL is loaded successfully
    if (DML_STATUS_OK != status || !driver_ptr->driver_instance_ptr || !own_load_configuration_functions(driver_ptr->driver_instance_ptr))
    {
        // Free DLL
        if (driver_ptr->driver_instance_ptr)
        {
            dlclose(driver_ptr->driver_instance_ptr);
        }

        driver_ptr->driver_instance_ptr = NULL;
    }

    return status;
#else
    return DML_STATUS_LIBACCEL_NOT_FOUND;
#endif
}

void DML_HW_API(finalize_accelerator_driver)(hw_driver_t *driver_ptr)
{
#if defined(linux)
    if (driver_ptr->driver_instance_ptr)
    {
        dlclose(driver_ptr->driver_instance_ptr);
    }

    driver_ptr->driver_instance_ptr = NULL;
#endif
}

int32_t DML_HW_API(driver_new_context)(struct accfg_ctx **ctx)
{
#if defined(linux)
    return ((accfg_new_ptr)functions_table[0].function)(ctx);
#else
    return DML_STATUS_LIBACCEL_NOT_FOUND;
#endif
}

struct accfg_device *DML_HW_API(context_get_first_device)(struct accfg_ctx *ctx)
{
#if defined(linux)
    return ((accfg_device_get_first_ptr)functions_table[1].function)(ctx);
#else
    return NULL;
#endif
}

const char *DML_HW_API(device_get_name)(struct accfg_device *device)
{
#if defined(linux)
    return ((accfg_device_get_devname_ptr)functions_table[2].function)(device);
#else
    return NULL;
#endif
}

struct accfg_device *DML_HW_API(device_get_next)(struct accfg_device *device)
{
#if defined(linux)
    return ((accfg_device_get_next_ptr)functions_table[3].function)(device);
#else
    return NULL;
#endif
}

struct accfg_wq *DML_HW_API(get_first_work_queue)(struct accfg_device *device)
{
#if defined(linux)
    return ((accfg_wq_get_first_ptr)functions_table[4].function)(device);
#else
    return NULL;
#endif
}

struct accfg_wq *DML_HW_API(work_queue_get_next)(struct accfg_wq *wq)
{
#if defined(linux)
    return ((accfg_wq_get_next_ptr)functions_table[5].function)(wq);
#else
    return NULL;
#endif
}

enum accfg_wq_state DML_HW_API(work_queue_get_state)(struct accfg_wq *wq)
{
#if defined(linux)
    return ((accfg_wq_get_state_ptr)functions_table[6].function)(wq);
#else
    return -1;
#endif
}

enum accfg_wq_mode DML_HW_API(work_queue_get_mode)(struct accfg_wq *wq)
{
#if defined(linux)
    return ((accfg_wq_get_mode_ptr)functions_table[7].function)(wq);
#else
    return 2;
#endif
}

uint32_t DML_HW_API(device_get_version)(struct accfg_device *device)
{
#if defined(linux)
    return ((accfg_device_get_version_ptr) functions_table[8].function)(device);
#else
    return -1;
#endif
}

const char * DML_HW_API(work_queue_get_device_name)(struct accfg_wq *wq)
{
#if defined(linux)
    return ((accfg_wq_get_devname_ptr) functions_table[9].function)(wq);
#else
    return NULL;
#endif
}

enum accfg_device_state DML_HW_API(device_get_state)(struct accfg_device *device)
{
#if defined(linux)
    return ((accfg_device_get_state_ptr)functions_table[10].function)(device);
#else
    return -1;
#endif
}

struct accfg_ctx *DML_HW_API(context_close)(struct accfg_ctx *ctx)
{
#if defined(linux)
    return ((accfg_unref_ptr)functions_table[11].function)(ctx);
#else
    return NULL;
#endif
}

uint64_t DML_HW_API(device_get_gen_cap_register)(struct accfg_device *device)
{
#if defined(linux)
    return ((accfg_device_get_gen_cap_ptr)functions_table[12].function)(device);
#else
    return 0;
#endif
}

uint64_t DML_HW_API(device_get_numa_node)(struct accfg_device *device)
{
#if defined(linux)
    return ((accfg_device_get_gen_cap_ptr)functions_table[13].function)(device);
#else
    return -1;
#endif
}

int32_t DML_HW_API(work_queue_get_priority)(struct accfg_wq *wq)
{
#if defined(linux)
    return ((accfg_wq_get_priority_ptr) functions_table[14].function)(wq);
#else
    return -1;
#endif
}

struct accfg_group *DML_HW_API(group_get_first)(struct accfg_device *device)
{
#if defined(linux)
    return ((accfg_group_get_first_ptr)functions_table[15].function)(device);
#else
    return NULL;
#endif
}

struct accfg_group *DML_HW_API(group_get_next)(struct accfg_group *group)
{
#if defined(linux)
    return ((accfg_group_get_next_ptr)functions_table[16].function)(group);
#else
    return NULL;
#endif
}

int DML_HW_API(group_get_traffic_class_a)(struct accfg_group *group)
{
#if defined(linux)
    return ((accfg_group_get_traffic_class_ptr)functions_table[17].function)(group);
#else
    return 0;
#endif
}

int DML_HW_API(group_get_traffic_class_b)(struct accfg_group *group)
{
#if defined(linux)
    return ((accfg_group_get_traffic_class_ptr)functions_table[18].function)(group);
#else
    return 0;
#endif
}

struct accfg_group *DML_HW_API(work_queue_get_group)(struct accfg_wq *wq)
{
#if defined(linux)
    return ((accfg_wq_get_group_ptr)functions_table[19].function)(wq);
#else
    return NULL;
#endif
}

int DML_HW_API(work_queue_get_group_id)(struct accfg_wq *wq)
{
#if defined(linux)
    return ((accfg_wq_get_group_id_ptr)functions_table[20].function)(wq);
#else
    return -1;
#endif
}

int DML_HW_API(group_get_id)(struct accfg_group *group)
{
#if defined(linux)
    return ((accfg_group_get_id_ptr)functions_table[21].function)(group);
#else
    return -1;
#endif
}

int DML_HW_API(work_queue_get_device_path)(struct accfg_wq *wq, char *buf, size_t size)
{
#if defined(linux)
    return ((accfg_wq_get_user_dev_path_ptr)functions_table[22].function)(wq, buf, size);
#else
    return -1;
#endif
}

#if defined(linux)

/* ------ Internal functions implementation ------ */

bool own_load_configuration_functions(void *driver_instance_ptr)
{
    uint32_t i = 0u;

    // Clear error log
    (void)dlerror();
    while (functions_table[i].function_name)
    {
        functions_table[i].function = (library_function)dlsym(driver_instance_ptr, functions_table[i].function_name);

        char *err_message = dlerror();

        if (err_message || !functions_table[i].function)
        {
            return false;
        }

        i++;
    }

    return true;
}

dsahw_status_t own_load_accelerator_configuration_driver(void **driver_instance_pptr)
{
    DIAG("loading driver: %s\n", accelerator_configuration_driver_name);
    // Try to load the user interface library for IAX/DSA kernel driver
    void *driver_instance_ptr = dlopen(accelerator_configuration_driver_name, RTLD_LAZY);

    if (!driver_instance_ptr)
    {
        // This is needed for error handle. We need to call dlerror
        // for emptying error message. Otherwise we will receive error
        // message during loading symbols from another library
        dlerror();

        return DML_STATUS_LIBACCEL_NOT_FOUND;
    }

    *driver_instance_pptr = driver_instance_ptr;

    return DML_STATUS_OK;
}

#endif
