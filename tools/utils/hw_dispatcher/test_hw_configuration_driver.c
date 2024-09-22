/*******************************************************************************
 * Copyright (C) 2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/*
 * HW configuration functions for tests
 */

#if defined(__linux__)
#include "test_hw_configuration_driver.h"

#include <dlfcn.h>
#include <fcntl.h>
#include <string.h> // strncmp
#include <sys/mman.h>

static const char* accelerator_configuration_driver_name = "libaccel-config.so.1";

/**
 * @brief Table with functions required from accelerator configuration library
 */
static dml_test_desc_t dml_test_functions_table[] = {{NULL, "accfg_unref"},
                                                     {NULL, "accfg_new"},
                                                     {NULL, "accfg_device_get_first"},
                                                     {NULL, "accfg_device_get_next"},
                                                     {NULL, "accfg_device_get_devname"},
                                                     {NULL, "accfg_device_get_numa_node"},
                                                     {NULL, "accfg_device_get_state"},
                                                     {NULL, "accfg_device_get_version"},
                                                     {NULL, "accfg_wq_get_first"},
                                                     {NULL, "accfg_wq_get_next"},
                                                     {NULL, "accfg_wq_get_mode"},
                                                     {NULL, "accfg_wq_get_size"},
                                                     {NULL, "accfg_wq_get_priority"},
                                                     {NULL, "accfg_wq_get_state"},
                                                     {NULL, "accfg_wq_get_user_dev_path"},
                                                     {NULL, "accfg_wq_get_op_config"},
                                                     {NULL, "accfg_engine_get_first"},
                                                     {NULL, "accfg_engine_get_next"},
                                                     {NULL, "accfg_engine_get_group_id"},

                                                     // Terminate list/init
                                                     {NULL, NULL}};

static inline dml_test_hw_accelerator_status
dml_test_own_load_accelerator_configuration_driver(void** driver_instance_pptr);

static inline bool dml_test_own_load_configuration_functions(void* driver_instance_ptr);

dml_test_hw_accelerator_status dml_test_hw_initialize_accelerator_driver(dml_test_hw_driver_t* driver_ptr) {

    // Variables
    driver_ptr->driver_instance_ptr = NULL;

    // Load DLL
    dml_test_hw_accelerator_status status =
            dml_test_own_load_accelerator_configuration_driver(&driver_ptr->driver_instance_ptr);

    if (status || driver_ptr->driver_instance_ptr == NULL) {
        dml_test_hw_finalize_accelerator_driver(driver_ptr);

        return DML_TEST_HW_ACCELERATOR_LIBACCEL_NOT_FOUND;
    }

    // If DLL is loaded successfully
    if (!dml_test_own_load_configuration_functions(driver_ptr->driver_instance_ptr)) {
        dml_test_hw_finalize_accelerator_driver(driver_ptr);

        return DML_TEST_HW_ACCELERATOR_LIBACCEL_NOT_FOUND;
    }

    return DML_TEST_HW_ACCELERATOR_STATUS_OK;
}

void dml_test_hw_finalize_accelerator_driver(dml_test_hw_driver_t* driver_ptr) {
    if (driver_ptr->driver_instance_ptr) { dlclose(driver_ptr->driver_instance_ptr); }

    driver_ptr->driver_instance_ptr = NULL;
}

/*
 * DML wrappers for accel-config APIs
 */
accfg_ctx* dml_test_accfg_unref(accfg_ctx* ctx) {
    return ((accfg_unref_ptr)dml_test_functions_table[0].function)(ctx);
}

int32_t dml_test_accfg_new(accfg_ctx** ctx) {
    return ((accfg_new_ptr)dml_test_functions_table[1].function)(ctx);
}

accfg_dev* dml_test_accfg_device_get_first(accfg_ctx* ctx) {
    return ((accfg_device_get_first_ptr)dml_test_functions_table[2].function)(ctx);
}

accfg_dev* dml_test_accfg_device_get_next(accfg_dev* device) {
    return ((accfg_device_get_next_ptr)dml_test_functions_table[3].function)(device);
}

const char* dml_test_accfg_device_get_devname(accfg_dev* device) {
    return ((accfg_device_get_devname_ptr)dml_test_functions_table[4].function)(device);
}

int dml_test_accfg_device_get_numa_node(accfg_dev* device) {
    return ((accfg_device_get_numa_node_ptr)dml_test_functions_table[5].function)(device);
}

enum accfg_device_state dml_test_accfg_device_get_state(accfg_dev* device) {
    return ((accfg_device_get_state_ptr)dml_test_functions_table[6].function)(device);
}

unsigned int dml_test_accfg_device_get_version(accfg_dev* device) {
    return ((accfg_device_get_version_ptr)dml_test_functions_table[7].function)(device);
}

accfg_wq* dml_test_accfg_wq_get_first(accfg_dev* device) {
    return ((accfg_wq_get_first_ptr)dml_test_functions_table[8].function)(device);
}

accfg_wq* dml_test_accfg_wq_get_next(accfg_wq* wq) {
    return ((accfg_wq_get_next_ptr)dml_test_functions_table[9].function)(wq);
}

enum accfg_wq_mode dml_test_accfg_wq_get_mode(accfg_wq* wq) {
    return ((accfg_wq_get_mode_ptr)dml_test_functions_table[10].function)(wq);
}

uint64_t dml_test_accfg_wq_get_size(accfg_wq* wq) {
    return ((accfg_wq_get_size_ptr)dml_test_functions_table[11].function)(wq);
}

int dml_test_accfg_wq_get_priority(accfg_wq* wq) {
    return ((accfg_wq_get_priority_ptr)dml_test_functions_table[12].function)(wq);
}

enum accfg_wq_state dml_test_accfg_wq_get_state(accfg_wq* wq) {
    return ((accfg_wq_get_state_ptr)dml_test_functions_table[13].function)(wq);
}

int dml_test_accfg_wq_get_user_dev_path(accfg_wq* wq, char* buf, size_t size) {
    return ((accfg_wq_get_user_dev_path_ptr)dml_test_functions_table[14].function)(wq, buf, size);
}

int dml_test_accfg_wq_get_op_config(accfg_wq* wq, accfg_opcfg* op_config) {
    if (dml_test_functions_table[15].function == NULL) return 1;
    return ((accfg_wq_get_op_config_ptr)dml_test_functions_table[15].function)(wq, op_config);
}

/* ------ Internal functions implementation ------ */

bool dml_test_own_load_configuration_functions(void* driver_instance_ptr) {
    uint32_t i = 0U;

    while (dml_test_functions_table[i].function_name) {
        dml_test_functions_table[i].function =
                (dml_test_library_function)dlsym(driver_instance_ptr, dml_test_functions_table[i].function_name);

        char* err_message = dlerror();

        if (err_message) {
            // @todo this is a workaround to optionally load accfg_wq_get_op_config
            char*  get_op_config_func_name     = "accfg_wq_get_op_config";
            size_t get_op_config_func_name_len = strlen(get_op_config_func_name);
           if (strlen(dml_test_functions_table[i].function_name) == get_op_config_func_name_len &&
                       strncmp(dml_test_functions_table[i].function_name, get_op_config_func_name,
                               get_op_config_func_name_len) == 0) {
            } else {
                return false;
            }
        }

        i++;
    }

    return true;
}

dml_test_hw_accelerator_status dml_test_own_load_accelerator_configuration_driver(void** driver_instance_pptr) {

    // Try to load the accelerator configuration library
    void* driver_instance_ptr = dlopen(accelerator_configuration_driver_name, RTLD_LAZY);

    if (!driver_instance_ptr) {
        // This is needed for error handle. We need to call dlerror
        // for emptying error message. Otherwise we will receive error
        // message during loading symbols from another library
        dlerror();

        return DML_TEST_HW_ACCELERATOR_LIBACCEL_NOT_FOUND;
    }

    *driver_instance_pptr = driver_instance_ptr;

    return DML_TEST_HW_ACCELERATOR_STATUS_OK;
}

#endif //__linux__
