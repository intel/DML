# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)
project(tests CXX)

add_subdirectory(test_utils)

# We need pthreads library in order to compile DML tests
find_package(Threads)

# Finding DML tests sources
file(GLOB TEST_COMMON_SRC common/*.cpp)
file(GLOB DML_CORE_TESTS core_tests/*.cpp)
file(GLOB DML_JOB_API_TESTS job_api_tests/*.cpp)
file(GLOB DML_JOB_API_UNIT_TESTS job_api_tests/unit/*.cpp)

# Path to reference functionality
file(GLOB DML_REFERENCE_INC job_api_tests/reference/include
                            job_api_tests/reference)

# Adding DML tests executable target
add_executable(tests ${DML_CORE_TESTS} ${DML_JOB_API_TESTS} ${TEST_COMMON_SRC} ${DML_JOB_API_UNIT_TESTS})

target_sources(tests PRIVATE $<TARGET_OBJECTS:dml_hl_tests>)

# Setting external and internal interfaces for DML tests executable
get_target_property(DML_SOURCE_DIR dml SOURCE_DIR)
target_include_directories(tests
                           PRIVATE common
                           PRIVATE ${DML_REFERENCE_INC}
                           PRIVATE $<TARGET_PROPERTY:dml,INTERFACE_INCLUDE_DIRECTORIES>
                           PRIVATE $<TARGET_PROPERTY:dml_core,INTERFACE_INCLUDE_DIRECTORIES>)

target_link_libraries(tests
                      PRIVATE dml
                      PRIVATE dml_test_utils
                      PRIVATE dmlhl
                      PRIVATE dml_sw_dispatcher
                      PRIVATE dml_dif_impl
                      PRIVATE gtest_main
                      PRIVATE ${CMAKE_THREAD_LIBS_INIT})


target_compile_features(tests PRIVATE cxx_std_17)

if (WIN32)
    target_compile_definitions(tests PUBLIC
                               DML
                               DEBUG
                               _MBCS
                               _LIB
                               _LIB_BUILD
                               _ARCH_EM64T)

    modify_standard_language_flag(LANGUAGE_NAME "CXX"
                                  FLAG_NAME "/EH"
                                  NEW_FLAG_VALUE "a")
else ()
    target_compile_definitions(tests PUBLIC
                               DML
                               _MBCS
                               _LIB
                               _LIB_BUILD
                               _ARCH_EM64T
                               unix
                               Linux
                               LINUX32E
                               _linux32e
                               _LINUX32E)
endif ()

# Install rules
install(TARGETS tests RUNTIME DESTINATION bin)


add_subdirectory(job_api_tests/multithread)
add_subdirectory(high-level-api)
add_subdirectory(utils)
add_subdirectory(functional)
