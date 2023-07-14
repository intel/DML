/*******************************************************************************
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef EXAMPLE_UTILS_H
#define EXAMPLE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dml/dml.h"

static inline int parse_execution_path(int argc, char **argv, dml_path_t* path_ptr) {

    if (argc < 2) {
        printf("Error: Missing the execution path as the first parameter.\n");
        printf("Use hardware_path, software_path or automatic_path.\n");
        return 1;
    }

    char *execution_path_str = argv[1];
    if (!strcmp(execution_path_str, "software_path")) {
        *path_ptr = DML_PATH_SW;
        printf("The example will be run on the software path.\n");
    }
    else if (!strcmp(execution_path_str, "hardware_path")) {
        *path_ptr = DML_PATH_HW;
        printf("The example will be run on the hardware path.\n");
    }
    else if (!strcmp(execution_path_str, "automatic_path")) {
        *path_ptr = DML_PATH_AUTO;
        printf("The example will be run on the automatic path.\n");
    }
    else {
        printf("Error: Unrecognized value for parameter.\n");
        printf("Use hardware_path, software_path or automatic_path.\n");
        return 1;
    }

    return 0;
}

#endif
