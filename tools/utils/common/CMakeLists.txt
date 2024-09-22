# ==========================================================================
# Copyright (C) 2024 Intel Corporation
#
# SPDX-License-Identifier: MIT
# ==========================================================================

# Build system for various utility functions used in testing.

enable_language(CXX)

add_library(tool_common INTERFACE)

target_include_directories(tool_common
        INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        INTERFACE $<TARGET_PROPERTY:tool_hw_dispatcher,INTERFACE_INCLUDE_DIRECTORIES>)

target_link_libraries(tool_common
        INTERFACE tool_hw_dispatcher)

