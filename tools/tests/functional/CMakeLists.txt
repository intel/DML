# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT

# Full name -> short name mapping
set(c_api   "c")
set(cpp_api "cpp")

set(sw_path   "sw")
set(hw_path   "hw")
set(auto_path "auto")

set(mem_move_operation        "mem_move")
set(fill_operation            "fill")
set(compare_operation         "compare")
set(compare_pattern_operation "compare_pattern")
set(crc_operation             "crc")
set(copy_crc_operation        "copy_crc")
set(create_delta_operation    "create_delta")
set(apply_delta_operation     "apply_delta")
set(dualcast_operation        "dualcast")
set(cache_flush_operation     "cache_flush")

list(APPEND apis ${c_api} ${cpp_api})
list(APPEND paths ${sw_path} ${hw_path} ${auto_path})
list(APPEND operations
        ${mem_move_operation}
        ${fill_operation}
        ${compare_operation}
        ${compare_pattern_operation}
        ${create_delta_operation}
        ${apply_delta_operation}
        ${dualcast_operation}
        ${crc_operation}
        ${copy_crc_operation}
        ${cache_flush_operation}
        )

foreach (api ${apis})
    foreach (path ${paths})
        foreach (operation ${operations})

            # Skip hw path for Windows
            if (WIN32 AND "${path}" STREQUAL "${hw_path}")
                continue()
            endif()

            set(labels functional ${operation})
            set(executable_name dml_test_functional_${api}_${path}_${operation})

            add_executable(${executable_name} ${operation}.cpp)

            target_compile_options(${executable_name} PRIVATE ${DML_QUALITY_OPTIONS})
            target_link_libraries(${executable_name} PRIVATE gtest_main dml_testing_utils)

            if ("${api}" STREQUAL "${c_api}")
                target_link_libraries(${executable_name} PRIVATE dml)
                target_compile_definitions(${executable_name} PRIVATE C_API)
                list(APPEND labels c_api)
            endif ()

            if ("${api}" STREQUAL "${cpp_api}")
                target_link_libraries(${executable_name} PRIVATE dmlhl)
                target_compile_definitions(${executable_name} PRIVATE CPP_API)
                list(APPEND labels cpp_api)
            endif ()

            if ("${path}" STREQUAL "${sw_path}")
                target_compile_definitions(${executable_name} PRIVATE SW_PATH)
                list(APPEND labels sw_path)
            endif ()

            if ("${path}" STREQUAL "${hw_path}")
                target_compile_definitions(${executable_name} PRIVATE HW_PATH)
                list(APPEND labels hw_path)
            endif ()

            if ("${path}" STREQUAL "${auto_path}")
                target_compile_definitions(${executable_name} PRIVATE AUTO_PATH)
                list(APPEND labels auto_path)
            endif ()

            add_test(NAME ${executable_name} COMMAND ${executable_name})
            set_tests_properties(${executable_name} PROPERTIES LABELS "${labels}")

            install(TARGETS ${executable_name} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
        endforeach ()
    endforeach ()
endforeach ()
