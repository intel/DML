#
# Copyright 2020-2021 Intel Corporation.
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

add_library(dml STATIC
    dml_batch.c
    dml_check_job.c
    dml_execute_job.c
    dml_finalize_job.c
    dml_get_job_size.c
    dml_get_library_version.c
    dml_init_job.c
    dml_submit_job.c
    dml_wait_job.c

    $<TARGET_OBJECTS:dml_core>
    )

target_compile_features(dml PUBLIC c_std_11)

# TODO: target_compile_options(dml PRIVATE ${DML_QUALITY_OPTIONS})

target_compile_definitions(dml PRIVATE DML_BADARG_CHECK)

target_include_directories(dml
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../include>
    PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE include
    PRIVATE ../cores/include # TODO: Remove
    )

# Pass git revision to get_library_version source file
get_git_revision()

set_property(
    SOURCE dml_get_library_version.c APPEND
    PROPERTY COMPILE_DEFINITIONS DML_GIT_REVISION="${GIT_REV}")

# TODO: Remove
if (DML_HW)
    target_sources(dml PRIVATE $<TARGET_OBJECTS:hw_path>)
    target_include_directories(dml PRIVATE ../hw-path/include)
    target_link_libraries(dml PRIVATE ${CMAKE_DL_LIBS})
    target_compile_definitions(dml PUBLIC DML_HW)
endif()

set_target_properties(dml PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_SOVERSION})

install(TARGETS dml
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# TODO: Remove (SW Path)
target_sources(dml PRIVATE
    sw-path/own_submit_software_update_dif.c
    sw-path/own_submit_software_path.c
    sw-path/own_software_data_tables.c
    sw-path/own_software_get_state_size.c
    sw-path/own_submit_software_check_dif.c
    sw-path/own_submit_software_insert_dif.c
    sw-path/own_submit_software_strip_dif.c
    )

target_include_directories(dml PRIVATE sw-path/include)
