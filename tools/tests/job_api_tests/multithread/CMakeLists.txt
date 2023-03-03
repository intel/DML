# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)
project(tests_mt CXX)

# Finding DML tests sources
file(GLOB SOURCES *.cpp)

# Adding DML tests executable target
add_executable(tests_mt ${SOURCES})

set_target_properties(tests_mt PROPERTIES CXX_STANDARD 17)

target_link_libraries(tests_mt
                      PRIVATE dml
                      PRIVATE gtest_main
                      PRIVATE ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(tests_mt
                           PRIVATE ../../common)

target_compile_options(tests_mt PUBLIC
                       $<$<CONFIG:Debug>:>
                       $<$<CONFIG:Release>:>
                       PRIVATE $<$<CXX_COMPILER_ID:GNU>:-m64 -Werror>
                       PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/WX>
                       PRIVATE "$<$<C_COMPILER_ID:GNU>:${DML_GNU_SDL_COMPILE_OPTIONS}>"
                       PRIVATE "$<$<C_COMPILER_ID:MSVC>:${DML_MSVS_SDL_COMPILE_OPTIONS}>")

if (WIN32)
    target_compile_definitions(tests_mt PUBLIC
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
    target_compile_definitions(tests_mt PUBLIC
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
install(TARGETS tests_mt RUNTIME DESTINATION bin)
