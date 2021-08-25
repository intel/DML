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
 * @brief Contains a
 * @date 3/23/2020
 *
 */
#if defined(linux)
    #include "libaccel_config.h"
    #include <dlfcn.h>
    #include <fcntl.h>
    #include <string.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
#endif

#include "hardware_definitions.h"
#include "own_dsa_accel_constants.h"
#include "own_hardware_definitions.h"
#include <stdbool.h>

#define OWN_PATH_MAX_LENGTH      (64u)
#define OWN_FIRST_DIGIT_CHAR     ('0')
#define OWN_DIGIT_TO_CHAR(digit) OWN_FIRST_DIGIT_CHAR + (digit)
#define ONW_GENCAP_ENABLED       (1)  // GENCAP is not available on Hypersim yet (but available on Wind River® Simics®)

#define dll_accfg_device_foreach(ctx, device)                      \
    for (device = dll_accfg_device_get_first(ctx); device != NULL; \
         device = dll_accfg_device_get_next(device))

#define dll_accfg_group_foreach(device, group)                     \
    for (group = dll_accfg_group_get_first(device); group != NULL; \
         group = dll_accfg_group_get_next(group))

#define dll_accfg_wq_foreach(device, wq) \
    for (wq = dll_accfg_wq_get_first(device); wq != NULL; wq = dll_accfg_wq_get_next(wq))

// General Capabilities Register unwrappers
#define OWN_GC_BLOCK_ON_FAULT_SUP(gen_cap)           ((gen_cap >> 0u) & 0x0000000000000001ull)
#define OWN_GC_OVERLAPPING_COPY_SUPPORT(gen_cap)     ((gen_cap >> 1u) & 0x0000000000000001ull)
#define OWN_GC_M_CACHE_CONTROL_SUPPORT(gen_cap)      ((gen_cap >> 2u) & 0x0000000000000001ull)
#define OWN_GC_F_CACHE_CONTROL_SUPPORT(gen_cap)      ((gen_cap >> 3u) & 0x0000000000000001ull)
#define OWN_GC_INTERRUPT_HANDLE_REQUEST(gen_cap)     ((gen_cap >> 7u) & 0x0000000000000001ull)
#define OWN_GC_DESTINATION_READBACK_SUPPORT(gen_cap) ((gen_cap >> 8u) & 0x0000000000000001ull)
#define OWN_GC_DESCRIPTOR_READBACK_SUPPORT(gen_cap)  ((gen_cap >> 9u) & 0x0000000000000001ull)
#define OWN_GC_MAX_TRANSFER_SIZE(gen_cap)            (1u << ((gen_cap >> 16u) & 0x000000000000001Full))
#define OWN_GC_MAX_BATCH_SIZE(gen_cap)               (1u << ((gen_cap >> 21u) & 0x000000000000000Full))
#define OWN_GC_MESSAGE_SIZE(gen_cap)                 (256u * ((gen_cap >> 25u) & 0x000000000000003Full))
#define OWN_GC_CONFIGURATION_SUPPORT(gen_cap)        ((gen_cap >> 31u) & 0x0000000000000001ull)
#define OWN_GC_MAX_DESCRIPTORS(gen_cap)              ((gen_cap >> 32u) & 0x00000000000000FFull)

typedef int (*FARPROC)();

static void *driver_ptr = NULL;
static int   first_time = 1;

static inline void *OWN_FUN(open_driver_library)(const uint8_t *library_name_ptr)
{
#if defined(linux)
    void *driver_library_ptr = dlopen(library_name_ptr, RTLD_LAZY);

    if (!driver_library_ptr)
    {
        /* This is needed for error handle. We need to call dlerror
        for emtying error message. Otherwise we will recieve error
        message during loading symbols from another library */
        dlerror();
    }

    return driver_library_ptr;
#else
    return NULL;
#endif
}

static inline void OWN_FUN(close_driver_library)(void *driver_library_ptr)
{
#if defined(linux)
    if (driver_library_ptr)
    {
        dlclose(driver_library_ptr);
    }
#endif
}

static inline dsahw_status_t OWN_FUN(load_driver_library)(void **driver_library_pptr)
{
#if defined(linux)
    void *       library_ptr = NULL;
    dml_status_t status      = DML_STATUS_OK;

    // Try to load the user interface library for IAX/DSA kernel driver
    (*driver_library_pptr) = NULL;

    library_ptr = OWN_FUN_CALL(open_driver_library)(DLL_NAME);
    if (library_ptr)
    {
        (*driver_library_pptr) = library_ptr;
        first_time             = 0u;
        status                 = DML_STATUS_OK;
    }
    else
    {
        first_time = 0u;
        status     = DML_STATUS_DRIVER_NOT_FOUND;
    }

    return status;
#else
    return DML_STATUS_DRIVER_NOT_FOUND;
#endif
}

/**
 * @brief Contains information about libaccel functions
 */
typedef struct
{
    FARPROC  function_ptr;      /**< Pointer to the function */
    uint8_t *function_name_ptr; /**< Name of the function */
} own_lib_accel_functions_t;

static own_lib_accel_functions_t lib_accel_functions[] = {{NULL, "accfg_new"},
                                                          {NULL, "accfg_device_get_first"},
                                                          {NULL, "accfg_device_get_devname"},
                                                          {NULL, "accfg_device_get_next"},
                                                          {NULL, "accfg_wq_get_first"},
                                                          {NULL, "accfg_wq_get_next"},
                                                          {NULL, "accfg_wq_get_state"},
                                                          {NULL, "accfg_wq_get_mode"},
                                                          {NULL, "accfg_device_get_cdev_major"},
                                                          {NULL, "accfg_wq_get_cdev_minor"},
                                                          {NULL, "accfg_device_get_state"},
                                                          {NULL, "accfg_unref"},
                                                          {NULL, "accfg_group_get_first"},
                                                          {NULL, "accfg_group_get_next"},
                                                          {NULL, "accfg_device_get_first"},
                                                          {NULL, "accfg_device_get_next"},
                                                          {NULL, "accfg_group_get_traffic_class_a"},
                                                          {NULL, "accfg_group_get_traffic_class_b"},
                                                          {NULL, "accfg_wq_set_group_id"},
                                                          {NULL, "accfg_group_get_id"},
                                                          {NULL, "accfg_wq_get_group"},
                                                          {NULL, "accfg_device_enable"},
                                                          {NULL, "accfg_wq_get_group_id"},
#if ONW_GENCAP_ENABLED
                                                          {NULL, "accfg_device_get_gen_cap"},
#endif
#if defined(LIB_ACCEL_VERSION_3_2)
                                                          {NULL, "accfg_wq_get_user_dev_path"},
#endif
                                                          {NULL, NULL}};

#if defined(linux)
/* ------ FUNCTION RENAMING ------ */
/*             RETURN TYPE      | NAME                                 | ARGUMENTS         */
typedef int (*p_accfg_new)(own_accfg_ctx **ctx);
typedef int (*p_accfg_wq_get_cdev_minor)(own_accfg_wq *wq);
typedef unsigned int (*p_accfg_device_get_cdev_major)(own_accfg_device *device);
typedef const char *(*p_accfg_device_get_devname)(own_accfg_device *device);
typedef own_accfg_device *(*p_accfg_device_get_first)(own_accfg_ctx *ctx);
typedef own_accfg_device *(*p_accfg_device_get_next)(own_accfg_device *device);
typedef own_accfg_wq *(*p_accfg_wq_get_first)(own_accfg_device *device);
typedef own_accfg_wq *(*p_accfg_wq_get_next)(own_accfg_wq *wq);
typedef enum accfg_wq_state (*p_accfg_wq_get_state)(own_accfg_wq *wq);
typedef enum accfg_wq_mode (*p_accfg_wq_get_mode)(own_accfg_wq *wq);
typedef enum accfg_device_state (*p_accfg_device_get_state)(own_accfg_device *device);
typedef own_accfg_ctx *(*p_accfg_unref)(own_accfg_ctx *ctx);
typedef struct accfg_group *(*p_accfg_group_get_first)(struct accfg_device *device);
typedef struct accfg_group *(*p_accfg_group_get_next)(struct accfg_group *group);
typedef struct accfg_device *(*p_accfg_device_get_first)(struct accfg_ctx *ctx);
typedef struct accfg_device *(*p_accfg_device_get_next)(struct accfg_device *device);
typedef int (*p_accfg_group_get_traffic_class_a)(struct accfg_group *group);
typedef int (*p_accfg_group_get_traffic_class_b)(struct accfg_group *group);
typedef int (*p_accfg_wq_set_group_id)(struct accfg_wq *wq, int val);
typedef int (*p_accfg_group_get_id)(struct accfg_group *group);
typedef struct accfg_group *(*p_accfg_wq_get_group)(struct accfg_wq *wq);
typedef int (*p_accfg_device_enable)(struct accfg_device *device);
typedef int (*p_accfg_wq_get_group_id)(struct accfg_wq *wq);
#if ONW_GENCAP_ENABLED
typedef unsigned long (*p_accfg_device_get_gen_cap)(struct accfg_device *device);
#endif
#if defined(LIB_ACCEL_VERSION_3_2)
typedef int (*p_accfg_wq_get_user_dev_path)(struct accfg_wq *wq, char *buf, size_t size);
#endif
#endif  // linux

#if defined(linux)
/*     | OLD NAME                           | NEW NAME                        */
static p_accfg_new                       dll_accfg_new;
static p_accfg_device_get_first          dll_accfg_device_get_first;
static p_accfg_device_get_devname        dll_accfg_device_get_devname;
static p_accfg_device_get_next           dll_accfg_device_get_next;
static p_accfg_wq_get_first              dll_accfg_wq_get_first;
static p_accfg_wq_get_next               dll_accfg_wq_get_next;
static p_accfg_wq_get_state              dll_accfg_wq_get_state;
static p_accfg_device_get_cdev_major     dll_accfg_device_get_cdev_major;
static p_accfg_wq_get_cdev_minor         dll_accfg_wq_get_cdev_minor;
static p_accfg_device_get_state          dll_accfg_device_get_state;
static p_accfg_unref                     dll_accfg_unref;
static p_accfg_wq_get_mode               dll_accfg_wq_get_mode;
static p_accfg_group_get_first           dll_accfg_group_get_first;
static p_accfg_group_get_next            dll_accfg_group_get_next;
static p_accfg_device_get_first          dll_accfg_device_get_first;
static p_accfg_device_get_next           dll_accfg_device_get_next;
static p_accfg_group_get_traffic_class_a dll_accfg_group_get_traffic_class_a;
static p_accfg_group_get_traffic_class_b dll_accfg_group_get_traffic_class_b;
static p_accfg_wq_set_group_id           dll_accfg_wq_set_group_id;
static p_accfg_group_get_id              dll_accfg_group_get_id;
static p_accfg_wq_get_group              dll_accfg_wq_get_group;
static p_accfg_device_enable             dll_accfg_device_enable;
static p_accfg_wq_get_group_id           dll_accfg_wq_get_group_id;
#if ONW_GENCAP_ENABLED
static p_accfg_device_get_gen_cap dll_accfg_device_get_gen_cap;
#endif
#if defined(LIB_ACCEL_VERSION_3_2)
static p_accfg_wq_get_user_dev_path dll_accfg_wq_get_user_dev_path;
#endif

#endif  // linux

static inline void OWN_FUN(set_functions)()
{
#if defined(linux)
    dll_accfg_new                = (p_accfg_new)(lib_accel_functions[0].function_ptr);
    dll_accfg_device_get_first   = (p_accfg_device_get_first)lib_accel_functions[1].function_ptr;
    dll_accfg_device_get_devname = (p_accfg_device_get_devname)lib_accel_functions[2].function_ptr;
    dll_accfg_device_get_next    = (p_accfg_device_get_next)lib_accel_functions[3].function_ptr;
    dll_accfg_wq_get_first       = (p_accfg_wq_get_first)lib_accel_functions[4].function_ptr;
    dll_accfg_wq_get_next        = (p_accfg_wq_get_next)lib_accel_functions[5].function_ptr;
    dll_accfg_wq_get_state       = (p_accfg_wq_get_state)lib_accel_functions[6].function_ptr;
    dll_accfg_wq_get_mode        = (p_accfg_wq_get_mode)lib_accel_functions[7].function_ptr;
    dll_accfg_device_get_cdev_major =
        (p_accfg_device_get_cdev_major)lib_accel_functions[8].function_ptr;
    dll_accfg_wq_get_cdev_minor = (p_accfg_wq_get_cdev_minor)lib_accel_functions[9].function_ptr;
    dll_accfg_device_get_state  = (p_accfg_device_get_state)lib_accel_functions[10].function_ptr;
    dll_accfg_unref             = (p_accfg_unref)lib_accel_functions[11].function_ptr;
    dll_accfg_group_get_first   = (p_accfg_group_get_first)lib_accel_functions[12].function_ptr;
    dll_accfg_group_get_next    = (p_accfg_group_get_next)lib_accel_functions[13].function_ptr;
    dll_accfg_device_get_first  = (p_accfg_device_get_first)lib_accel_functions[14].function_ptr;
    dll_accfg_device_get_next   = (p_accfg_device_get_next)lib_accel_functions[15].function_ptr;
    dll_accfg_group_get_traffic_class_a =
        (p_accfg_group_get_traffic_class_a)lib_accel_functions[16].function_ptr;
    dll_accfg_group_get_traffic_class_b =
        (p_accfg_group_get_traffic_class_b)lib_accel_functions[17].function_ptr;
    dll_accfg_wq_set_group_id = (p_accfg_wq_set_group_id)lib_accel_functions[18].function_ptr;
    dll_accfg_group_get_id    = (p_accfg_group_get_id)lib_accel_functions[19].function_ptr;
    dll_accfg_wq_get_group    = (p_accfg_wq_get_group)lib_accel_functions[20].function_ptr;
    dll_accfg_device_enable   = (p_accfg_device_enable)lib_accel_functions[21].function_ptr;
    dll_accfg_wq_get_group_id = (p_accfg_wq_get_group_id)lib_accel_functions[22].function_ptr;
    #if ONW_GENCAP_ENABLED
    dll_accfg_device_get_gen_cap = (p_accfg_device_get_gen_cap)lib_accel_functions[23].function_ptr;
    #endif
    #if LIB_ACCEL_VERSION_3_2
    dll_accfg_wq_get_user_dev_path = (p_accfg_wq_get_user_dev_path)lib_accel_functions[24].function_ptr;
    #endif
#endif  // linux
}

typedef enum
{
    OWN_DRIVER_STATUS_OK = DML_STATUS_OK,
    OWN_DRIVER_STATUS_ERROR
} own_driver_status_t;

static inline dsahw_status_t OWN_FUN(get_lib_accel_function_address)(void *      driver_library_ptr,
                                                                     const void *function_ptr,
                                                                     uint8_t *   function_name_ptr)
{
#if defined(linux)
    FARPROC *procedure_ptr = (FARPROC *)function_ptr;

    /* assign function pointers to static variable */
    (*procedure_ptr) = dlsym(driver_library_ptr, function_name_ptr);

    uint8_t *error_ptr = dlerror();

    return (error_ptr != NULL) ? DML_STATUS_DRIVER_ERROR : DML_STATUS_OK;
#else
    return DML_STATUS_DRIVER_NOT_FOUND;
#endif
}

static inline dsahw_status_t OWN_FUN(load_lib_accel_functions)(void *driver_library_ptr)
{
#if defined(linux)
    for (uint32_t i = 0u; lib_accel_functions[i].function_name_ptr; ++i)
    {
        own_driver_status_t status =
            OWN_FUN_CALL(get_lib_accel_function_address)(driver_library_ptr,
                                                         &lib_accel_functions[i].function_ptr,
                                                         lib_accel_functions[i].function_name_ptr);

        OWN_HARDWARE_RETURN_IF(status != OWN_DRIVER_STATUS_OK, DML_STATUS_DRIVER_ERROR)
    }

    // Set pointers to founded driver functions
    OWN_FUN_CALL(set_functions)();

    return DML_STATUS_OK;
#else
    return DML_STATUS_DRIVER_NOT_FOUND;
#endif
}

static inline dsahw_status_t OWN_FUN(connect_to_driver)()
{
#if defined(linux)
    dsahw_status_t status = OWN_FUN_CALL(load_driver_library)(&driver_ptr);

    if ((DML_STATUS_OK == status) && driver_ptr)
    {
        if (OWN_FUN_CALL(load_lib_accel_functions)(driver_ptr) != DML_STATUS_OK)
        {
            OWN_FUN_CALL(close_driver_library)(driver_ptr); /* free DLL */
            driver_ptr = NULL;
            status     = DML_STATUS_DRIVER_ERROR;
        }
        else
        {
            status = DML_STATUS_OK;
        }
    }
    else
    {
        OWN_FUN_CALL(close_driver_library)(driver_ptr);
        driver_ptr = NULL;
        status     = DML_STATUS_DRIVER_NOT_FOUND;
    }

    return status;
#else
    return DML_STATUS_DRIVER_NOT_FOUND;
#endif
}

/**
 * @brief Looks for "dsa" pattern in device name string;
 *
 * @param[in] src_ptr  pointer to the driver config string
 *
 * @return on success - return ptr to "dsa" or NULL in the other case
 */
static inline uint8_t *OWN_FUN(search_dsa_device)(uint8_t *src_ptr)
{
    for (uint32_t i = 0u; ('\0' != src_ptr[i + DEVICE_NAME_LENGTH]); ++i)
    {
        // As search pattern is fixed (3 bytes) - will compare as int
        uint32_t *ptr = (uint32_t *)&src_ptr[i];

        /*
         * Convert the first 3 bytes to lower case and make the 4th 0xff
         * As IAX_DEVICE macro forms the next int: 0xff,'a','s','d' (LE notation)
         */
        if (DSA_DEVICE_ID == (*ptr | CHAR_MSK))
        {
            return (uint8_t *)ptr;
        }
    }

    return NULL;
}

// this function converts int to str representation and adds it to path string;
// success - returns idx of the next "free" char in the path, overflow otherwise
static inline uint32_t OWN_FUN(int_to_string)(uint8_t *string_ptr,
                                              uint32_t string_size,
                                              uint32_t string_offset,
                                              uint32_t number)
{
// Constants
#define DEC_BASE        10u
#define DEC_MAX_INT_BUF 16u

    // Variables
    uint8_t  char_buffer[DEC_MAX_INT_BUF];
    uint32_t last_char_index = 0u;

    do
    {
        uint8_t digit                  = number % DEC_BASE;
        char_buffer[last_char_index++] = OWN_DIGIT_TO_CHAR(digit);
        number /= DEC_BASE;  // next digit
    } while (0u < number);

    OWN_HARDWARE_RETURN_IF((string_size < string_offset + last_char_index), string_size + 1u)

    do
    {
        // Copy to "path" in correct order
        string_ptr[string_offset++] = char_buffer[--last_char_index];
    } while (0u < last_char_index);

    return string_offset;
}

// this function creates path_ptr string for retrieved char device - path_ptr + major + minor
// success - returns updated path_ptr and "ok" status, overflow otherwise
static inline dsahw_status_t OWN_FUN(get_mount_path)(uint8_t *         path_ptr,
                                                     uint32_t          path_size,
                                                     own_accfg_device *device_ptr,
                                                     own_accfg_wq *    work_queue_ptr)
{
#if defined(linux)
#if defined(LIB_ACCEL_VERSION_3_2)
    return dll_accfg_wq_get_user_dev_path(work_queue_ptr, path_ptr, path_size);
#else
    const uint32_t major = (uint32_t)dll_accfg_device_get_cdev_major(device_ptr);
    const uint32_t minor = dll_accfg_wq_get_cdev_minor(work_queue_ptr);

    OWN_HARDWARE_RETURN_IF(((0u > major) || (0u > minor)), DML_HW_STATUS_CHAR_DEVICE_NOT_AVAILABLE)

    uint32_t offset = 0;
    while (('\0' != path_ptr[offset]) && (offset < path_size))
    {
        offset++;
    }

    OWN_HARDWARE_RETURN_IF((path_size < offset), DML_HW_STATUS_BUFFER_OVERFLOW)

    offset = OWN_FUN_CALL(int_to_string)(path_ptr, path_size, offset, major);

    OWN_HARDWARE_RETURN_IF((path_size < offset + 1u), DML_HW_STATUS_BUFFER_OVERFLOW)
    path_ptr[offset++] = ':';

    offset = OWN_FUN_CALL(int_to_string)(path_ptr, path_size, offset, minor);

    OWN_HARDWARE_RETURN_IF((path_size < offset), DML_HW_STATUS_BUFFER_OVERFLOW)

    path_ptr[offset] = '\0';
    return DML_HW_STATUS_OK;
#endif
#else
    return DML_STATUS_DRIVER_NOT_FOUND;
#endif
}

static inline uint32_t OWN_FUN(open_available_devices)(own_accfg_ctx *    ctx_ptr,
                                                       own_accfg_device **devices_pptr)
{
#if defined(linux)
    uint32_t          device_count        = 0u;
    own_accfg_device *temporal_device_ptr = NULL;

    dll_accfg_device_foreach(ctx_ptr, temporal_device_ptr)
    {
        char *const device_name_ptr = (char *)dll_accfg_device_get_devname(temporal_device_ptr);
        char *const device_ptr      = OWN_FUN_CALL(search_dsa_device)(device_name_ptr);

        // If it is "dsa" device - p will point to "dsa" signature in the dev name; NULL otherwise
        if (NULL != device_ptr)
        {
            // Determine if the device state is DISABLED or ENABLED
            if (ACCFG_DEVICE_ENABLED == dll_accfg_device_get_state(temporal_device_ptr))
            {
                device_count++;
                devices_pptr[device_count - 1u] =
                    temporal_device_ptr;  // store enabled "iax" devices in the array
            }
        }
    }

    return device_count;
#else
    return 0u;
#endif
}

static inline uint32_t OWN_FUN(open_available_work_queues)(own_accfg_device *device_ptr,
                                                           own_accfg_wq **   work_queues_pptr)
{
#if defined(linux)
    uint32_t      work_queue_count        = 0u;
    own_accfg_wq *temporal_work_queue_ptr = NULL;

    dll_accfg_wq_foreach(device_ptr, temporal_work_queue_ptr)
    {
        // Returns the work_queues_ptr state of the given work_queues_ptr; it can be "enabled", "disabled" and "quiescing"
        if (ACCFG_WQ_ENABLED == dll_accfg_wq_get_state(temporal_work_queue_ptr))
        {
            // Returns the work_queues_ptr mode based on the given work_queues_ptr; it can be shared or dedicated.
            if (ACCFG_WQ_SHARED == dll_accfg_wq_get_mode(temporal_work_queue_ptr))
            {
                work_queue_count++;
                work_queues_pptr[work_queue_count - 1u] = temporal_work_queue_ptr;
            }
        }
    }

    return work_queue_count;
#else
    return 0u;
#endif
}


static inline dsahw_status_t OWN_FUN(split_wqs)(own_accfg_wq **   wq_pptr,
                                                uint32_t          wq_count,
                                                own_accfg_device *device,
                                                own_accfg_wq **   tc_a_pptr,
                                                own_accfg_wq **   tc_b_pptr)
{
#if defined(linux)
    struct accfg_group *group = NULL;

    uint32_t tc_a_count = 0u;
    uint32_t tc_b_count = 0u;

    tc_a_pptr[0] = NULL;
    tc_b_pptr[0] = NULL;

    for (uint i = 0u; i < wq_count; ++i)
    {
        // Workaround because dll_accfg_wq_get_group
        // always returns NULL
        const int group_id = dll_accfg_wq_get_group_id(wq_pptr[i]);

        dll_accfg_group_foreach(device, group)
        {
            if (group_id == dll_accfg_group_get_id(group))
            {
                tc_a_pptr[tc_a_count++] = wq_pptr[i];

                if (dll_accfg_group_get_traffic_class_b(group))
                {
                    tc_b_pptr[tc_b_count++] = wq_pptr[i];
                }

                break;
            }
        }
    }

    return DML_STATUS_OK;
#else
    return DML_STATUS_DRIVER_NOT_FOUND;
#endif
}


static inline dsahw_status_t OWN_FUN(equal_strings)(const uint8_t *first_ptr,
                                                    const uint8_t *second_ptr,
                                                    uint32_t       length)
{
    for (uint32_t i = 0u; i < length; ++i)
    {
        if (first_ptr[i] != second_ptr[i])
        {
            return DML_STATUS_FALSE_PREDICATE_OK;
        }
    }

    return DML_STATUS_OK;
}

static inline dsahw_status_t OWN_FUN(mmap_portals)(const uint8_t *path_ptr, void **portals_ptr)
{
#if defined(linux)
    int32_t file_descriptor = open(path_ptr, O_RDWR);
    DML_BAD_ARGUMENT_RETURN((0 > file_descriptor), DML_HW_STATUS_DEVICE_NOT_OPENED)

    (*portals_ptr) =
        mmap(NULL, OWN_OS_PAGE_SIZE, PROT_WRITE, MAP_SHARED | MAP_POPULATE, file_descriptor, 0u);
    DML_BAD_ARGUMENT_RETURN(MAP_FAILED == portals_ptr, DML_HW_STATUS_PORTAL_MAP_FAILED)

    close(file_descriptor);
    return DML_STATUS_OK;
#else
    return DML_STATUS_DRIVER_NOT_FOUND;
#endif
}

static inline dsahw_status_t OWN_FUN(init)(dsahw_context_t *context_ptr)
{
#if defined(linux)
    {
        // Variables
        own_accfg_ctx *   ctx;
        own_accfg_device *devices_ptr[MAX_DEVICE_COUNT];
        own_accfg_wq *    work_queues_ptr[MAX_WORK_QUEUE_COUNT];
        own_accfg_wq *    tc_a_pptr[MAX_WORK_QUEUE_COUNT];
        own_accfg_wq *    tc_b_pptr[MAX_WORK_QUEUE_COUNT];
        dsahw_status_t    status;
        unsigned long     dsa_features;  // DSA GENCAP register value

        context_ptr->guard = OWN_HW_STATE_GUARD;

        status = OWN_FUN_CALL(connect_to_driver)();
        OWN_HARDWARE_RETURN_IF((DML_STATUS_OK != status), status)

        // Create new context for libaccel_config library instance
        status = dll_accfg_new(&ctx);
        OWN_HARDWARE_RETURN_IF((0u != status), DML_HW_STATUS_LIB_ACCEL_ERROR)

        context_ptr->dsa_context_ptr = ctx;

        const uint32_t device_count = OWN_FUN_CALL(open_available_devices)(ctx, devices_ptr);
        OWN_HARDWARE_RETURN_IF((0u >= device_count), DML_HW_STATUS_DEVICE_NOT_AVAILABLE)

        //dsa_features = dll_accfg_device_get_gen_cap(devices_ptr[0]);

        const uint32_t work_queue_count =
            OWN_FUN_CALL(open_available_work_queues)(devices_ptr[0], work_queues_ptr);
        OWN_HARDWARE_RETURN_IF((0u >= work_queue_count),
                               DML_HW_STATUS_DEVICE_WORK_QUEUE_NOT_AVAILABLE)

        status = OWN_FUN_CALL(split_wqs)(
            work_queues_ptr, work_queue_count, devices_ptr[0], tc_a_pptr, tc_b_pptr);
        OWN_HARDWARE_RETURN_IF((DML_STATUS_OK != status), status);

        if ((NULL != tc_a_pptr[0]) && (NULL != tc_b_pptr[0]))
        {
            // Mount DSA device
            uint8_t path_a[OWN_PATH_MAX_LENGTH] = "/dev/char/";
            uint8_t path_b[OWN_PATH_MAX_LENGTH] = "/dev/char/";

            // Get mount path in the next format: "/devices_ptr/char/major:minor"
            status = OWN_FUN_CALL(get_mount_path)(
                &path_a[0], OWN_PATH_MAX_LENGTH - 1u, devices_ptr[0], tc_a_pptr[0]);
            DML_BAD_ARGUMENT_RETURN(status, DML_HW_STATUS_DEVICE_PATH_OVERFLOW)

            status = OWN_FUN_CALL(get_mount_path)(
                &path_b[0], OWN_PATH_MAX_LENGTH - 1u, devices_ptr[0], tc_b_pptr[0]);
            DML_BAD_ARGUMENT_RETURN(status, DML_HW_STATUS_DEVICE_PATH_OVERFLOW)

            // WQ is working with both TCs
            if (DML_STATUS_OK ==
                OWN_FUN_CALL(equal_strings)(&path_a[0], &path_b[0], sizeof(path_a)))
            {
                void *portals_ptr = NULL;

                status = OWN_FUN_CALL(mmap_portals)(&path_a[0], &portals_ptr);
                DML_BAD_ARGUMENT_RETURN((DML_STATUS_OK != status), status)

                context_ptr->portal_table.tc_a_portals.portals_ptr = portals_ptr;
                context_ptr->portal_table.tc_b_portals.portals_ptr = portals_ptr;
            }
            else
            {
                void *portals_a_ptr = NULL;
                void *portals_b_ptr = NULL;

                status = OWN_FUN_CALL(mmap_portals)(&path_a[0], &portals_a_ptr);
                DML_BAD_ARGUMENT_RETURN((DML_STATUS_OK != status), status)

                status = OWN_FUN_CALL(mmap_portals)(&path_b[0], &portals_b_ptr);
                DML_BAD_ARGUMENT_RETURN((DML_STATUS_OK != status), status)

                context_ptr->portal_table.tc_a_portals.portals_ptr = portals_a_ptr;
                context_ptr->portal_table.tc_b_portals.portals_ptr = portals_b_ptr;
            }
        }
        else
        {
            // Mount DSA device
            uint8_t path[OWN_PATH_MAX_LENGTH] = "/dev/char/";
            void *  portals_ptr               = NULL;

            // Get mount path in the next format: "/devices_ptr/char/major:minor"
            status = OWN_FUN_CALL(get_mount_path)(
                &path[0], sizeof(path) - 1u, devices_ptr[0], work_queues_ptr[0]);
            DML_BAD_ARGUMENT_RETURN(status, DML_HW_STATUS_DEVICE_PATH_OVERFLOW)

            status = OWN_FUN_CALL(mmap_portals)(&path[0], &portals_ptr);
            DML_BAD_ARGUMENT_RETURN((DML_STATUS_OK != status), status)

            if (NULL != tc_a_pptr[0])
            {
                context_ptr->portal_table.tc_a_portals.portals_ptr = portals_ptr;
            }
            else
            {
                context_ptr->portal_table.tc_b_portals.portals_ptr = portals_ptr;
            }
        }

    #if ONW_GENCAP_ENABLED
        // General Capabilities Register unpacking
        unsigned long gen_cap = dll_accfg_device_get_gen_cap(devices_ptr[0]);

        context_ptr->gen_cap.block_on_fault_support   = OWN_GC_BLOCK_ON_FAULT_SUP(gen_cap);
        context_ptr->gen_cap.overlapping_copy_support = OWN_GC_OVERLAPPING_COPY_SUPPORT(gen_cap);
        context_ptr->gen_cap.memory_cache_control_support = OWN_GC_M_CACHE_CONTROL_SUPPORT(gen_cap);
        context_ptr->gen_cap.flush_cache_control_support  = OWN_GC_F_CACHE_CONTROL_SUPPORT(gen_cap);
        context_ptr->gen_cap.interrupt_handle_request = OWN_GC_INTERRUPT_HANDLE_REQUEST(gen_cap);
        context_ptr->gen_cap.destination_readback_support =
            OWN_GC_DESTINATION_READBACK_SUPPORT(gen_cap);
        context_ptr->gen_cap.descriptor_readback_support =
            OWN_GC_DESCRIPTOR_READBACK_SUPPORT(gen_cap);
        context_ptr->gen_cap.max_transfer_size     = OWN_GC_MAX_TRANSFER_SIZE(gen_cap);
        context_ptr->gen_cap.max_batch_size        = OWN_GC_MAX_BATCH_SIZE(gen_cap);
        context_ptr->gen_cap.message_size          = OWN_GC_MESSAGE_SIZE(gen_cap);
        context_ptr->gen_cap.configuration_support = OWN_GC_CONFIGURATION_SUPPORT(gen_cap);
        context_ptr->gen_cap.max_descriptors       = OWN_GC_MAX_DESCRIPTORS(gen_cap);
    #endif

        return DML_HW_STATUS_OK;
    }
#else
    {
        return DML_STATUS_DRIVER_NOT_FOUND;
    }
#endif
}

static bool context_initialized = false;

dsahw_status_t DML_HW_API(get_context)(const dsahw_context_t **const context_ptr)
{
    static dsahw_context_t context;
    if (context_initialized)
    {
        *context_ptr = &context;
        return DML_STATUS_OK;
    }
    else
    {
        dsahw_status_t status = OWN_FUN_CALL(init)(&context);
        if (status != DML_STATUS_OK)
        {
            return status;
        }

        *context_ptr          = &context;
        context_initialized   = true;
        return status;
    }
}

dsahw_status_t DML_HW_API(finalize)(dsahw_context_t *context_ptr)
{
    if (context_initialized)
    {
        return DML_STATUS_OK;
    }
    else
    {
        context_initialized = false;
    }

#if defined(linux)
    OWN_CHECK_DML_HARDWARE_STATE(context_ptr)

    if (NULL != context_ptr->portal_table.tc_a_portals.portals_ptr)
    {
        const int32_t status =
            munmap(context_ptr->portal_table.tc_a_portals.portals_ptr, OWN_OS_PAGE_SIZE);
        OWN_HARDWARE_RETURN_IF((0 != status), DML_STATUS_HARDWARE_DISCONNECTION_ERROR)
    }

    if (context_ptr->portal_table.tc_a_portals.portals_ptr !=
        context_ptr->portal_table.tc_b_portals.portals_ptr)
    {
        const int32_t status =
            munmap(context_ptr->portal_table.tc_b_portals.portals_ptr, OWN_OS_PAGE_SIZE);
        OWN_HARDWARE_RETURN_IF((0 != status), DML_STATUS_HARDWARE_DISCONNECTION_ERROR)
    }

    if (NULL != context_ptr->dsa_context_ptr)
    {
        context_ptr->dsa_context_ptr = dll_accfg_unref(context_ptr->dsa_context_ptr);
    }

    context_ptr->portal_table.tc_a_portals.portals_ptr = NULL;
    context_ptr->portal_table.tc_b_portals.portals_ptr = NULL;
    context_ptr->dsa_context_ptr                       = NULL;
#endif
    return DML_STATUS_OK;
}

int DML_HW_API(get_overlapping_copy_support)(dsahw_context_t *hw_context_ptr)
{
    return hw_context_ptr->gen_cap.overlapping_copy_support;
}
