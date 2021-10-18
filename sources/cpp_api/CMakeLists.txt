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

add_library(dmlhl STATIC
    $<TARGET_OBJECTS:dml_middle_layer>
    $<TARGET_OBJECTS:dml_core> # TODO: Remove
    )

target_include_directories(dmlhl
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../include>
    PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUEDIR}>
    )

target_compile_features(dmlhl PUBLIC cxx_std_17)

# TODO: Remove
if (DML_HW)
    target_sources(dmlhl PRIVATE $<TARGET_OBJECTS:hw_path>)
    target_link_libraries(dmlhl PRIVATE ${CMAKE_DL_LIBS})
    target_compile_definitions(dmlhl PUBLIC DML_HW)
endif()

set_target_properties(dmlhl PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_SOVERSION})

install(TARGETS dmlhl
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
