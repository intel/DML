#
# Copyright 2018-2021 Intel Corporation.
#
# This software and the related documents are Intel copyrighted materials,
# and your use of them is governed by the express license under which they
# were provided to you ("License"). Unless the License provides otherwise,
# you may not use, modify, copy, publish, distribute, disclose or transmit
# this software or the related documents without Intel's prior written
# permission.
#
# This software and the related documents are provided as is, with no
# express or implied warranties, other than those that are expressly
# stated in the License.
#

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
