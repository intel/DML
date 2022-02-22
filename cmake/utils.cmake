# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT

macro(get_git_revision)
    # Find git revision
    execute_process(COMMAND git rev-parse --short HEAD
                    OUTPUT_VARIABLE GIT_REV
                    ERROR_QUIET)

    if ("${GIT_REV}" STREQUAL "")
        set(GIT_REV "N/A")
    else()
        string(STRIP "${GIT_REV}" GIT_REV)
    endif()

    add_compile_definitions(DML_GIT_REVISION="${GIT_REV}")
endmacro()
