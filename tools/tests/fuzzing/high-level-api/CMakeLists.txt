# ==========================================================================
# Copyright (C) 2023 Intel Corporation
#
# SPDX-License-Identifier: MIT
# ==========================================================================

project(dml_hl_fuzz_tests VERSION 1.0 LANGUAGES CXX)

file(GLOB CPP_FILES "*.cpp")
add_custom_target(hl_fuzz_tests)
foreach (FILE ${CPP_FILES})
	file(RELATIVE_PATH FILE_REL_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${FILE})
	string(REPLACE ".cpp" "" TARGET_NAME "${FILE_REL_PATH}")
	add_executable(${TARGET_NAME} ${FILE_REL_PATH})
	target_link_libraries(${TARGET_NAME} PRIVATE dml)
	add_dependencies(hl_fuzz_tests ${TARGET_NAME})
endforeach()
