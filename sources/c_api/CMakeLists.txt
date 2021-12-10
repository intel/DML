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
        dml.cpp
        dml_batch.cpp
        dml_get_library_version.cpp
        )

target_include_directories(dml
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../include>
        PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE include
        PRIVATE ../include
        )
target_sources(dml
        PRIVATE $<TARGET_OBJECTS:dml_middle_layer>
        PRIVATE $<TARGET_PROPERTY:dml_middle_layer,INTERFACE_SOURCES>
        )
target_compile_features(dml
        PUBLIC c_std_11
        PRIVATE cxx_std_17
        )
target_compile_options(dml
        PRIVATE ${DML_QUALITY_OPTIONS}
        PRIVATE ${DML_CPP_PRIVATE_OPTIONS}
        )

if(DML_HW)
    target_link_libraries(dml PRIVATE ${CMAKE_DL_LIBS})
endif()

# Pass git revision to get_library_version source file
get_git_revision()

set_property(
        SOURCE dml_get_library_version.cpp APPEND
        PROPERTY COMPILE_DEFINITIONS DML_GIT_REVISION="${GIT_REV}")

set_target_properties(dml PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_SOVERSION})

install(TARGETS dml
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
