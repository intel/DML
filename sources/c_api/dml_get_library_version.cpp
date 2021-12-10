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
 * @brief Contains an implementation of the @ref dml_get_library_version function
 * @date 3/13/2020
 *
 */

#include <dml/dmldefs.h>

/** Name of the library*/
#define DML_LIBRARY_NAME "Intel(R) DML Software Library"

/** Version of the library*/
#define DML_LIBRARY_VERSION "Beta"

/** Major version of the library*/
#define DML_LIBRARY_MAJOR_VERSION 0u

/** Minor version of the library*/
#define DML_LIBRARY_MINOR_VERSION 1u

/** Minor version of the library*/
#define DML_LIBRARY_PATCH 6u

/** Supported CPU ISA */
#define DML_LIBRARY_MINIMAL_CPU_ISA "N/A"

/** Minimal HW version that the library supports */
#define DML_LIBRARY_MIN_HW_VERSION "DML 1.0"

extern "C" const dml_library_version_t* dml_get_library_version()
{
    static const dml_library_version_t library_version = { DML_LIBRARY_NAME,          DML_LIBRARY_MINIMAL_CPU_ISA, __DATE__,
                                                           DML_LIBRARY_VERSION,       DML_LIBRARY_MIN_HW_VERSION,  DML_GIT_REVISION,
                                                           DML_LIBRARY_MAJOR_VERSION, DML_LIBRARY_MINOR_VERSION,   DML_LIBRARY_PATCH };

    return &library_version;
}
