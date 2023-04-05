# Copyright (C) 2023 Intel Corporation
#
# SPDX-License-Identifier: MIT

if (DML_BUILD_TESTS)
    # Dependencies
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # Links c runtimes dynamically for gtest, for fixing a bug with recent version of VS.
    set(BUILD_GMOCK OFF CACHE BOOL "Suppress gmock build" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "Suppress gtest installation" FORCE)
    add_subdirectory(third-party/google-test EXCLUDE_FROM_ALL)
    add_subdirectory(tests)
    if (UNIX)
        SET(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
        add_subdirectory(third-party/google-benchmark EXCLUDE_FROM_ALL)
        add_subdirectory(benchmarks)
    endif ()
endif ()

install(DIRECTORY        ${CMAKE_CURRENT_LIST_DIR}/configs
        DESTINATION      share/${CMAKE_PROJECT_NAME}
        COMPONENT        RUNTIME)

install(DIRECTORY        ${CMAKE_CURRENT_LIST_DIR}/scripts
        DESTINATION      share/${CMAKE_PROJECT_NAME}
        COMPONENT        RUNTIME
        FILE_PERMISSIONS USE_SOURCE_PERMISSIONS)