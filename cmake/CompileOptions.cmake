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

function(modify_standard_language_flag)
    # Declaring function parameters
    set(OPTIONS "")
    set(ONE_VALUE_ARGS
        LANGUAGE_NAME
        FLAG_NAME
        NEW_FLAG_VALUE)
    set(MULTI_VALUE_ARGS "")

    # Parsing function parameters
    cmake_parse_arguments(MODIFY
                          "${OPTIONS}"
                          "${ONE_VALUE_ARGS}"
                          "${MULTI_VALUE_ARGS}"
                          ${ARGN})

    # Variables
    set(FLAG_REGULAR_EXPRESSION "${MODIFY_FLAG_NAME}.*[ ]*")
    set(NEW_VALUE "${MODIFY_FLAG_NAME}${MODIFY_NEW_FLAG_VALUE}")

    # Replacing specified flag with new value
    string(REGEX REPLACE
           ${FLAG_REGULAR_EXPRESSION} ${NEW_VALUE}
           NEW_COMPILE_FLAGS
           "${CMAKE_${MODIFY_LANGUAGE_NAME}_FLAGS}")

    # Returning the value
    set(CMAKE_${MODIFY_LANGUAGE_NAME}_FLAGS ${NEW_COMPILE_FLAGS} PARENT_SCOPE)
endfunction()
