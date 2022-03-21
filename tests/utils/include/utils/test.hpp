/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_ACTUAL_API_HPP
#define DML_TESTING_ACTUAL_API_HPP

#if !defined(OS_WINDOWS) && !defined(OS_UNIX)
#error "Operating system is undefined!"
#endif

#if !defined(C_API) && !defined(CPP_API)
#error "API is undefined!"
#endif

#if !defined(SW_PATH) && !defined(HW_PATH) && !defined(AUTO_PATH)
#error "Execution path is undefined!"
#endif

#ifdef C_API
#include <dml/dml.h>
#include <utils/job.hpp>

#ifdef SW_PATH
constexpr auto execution_path = DML_PATH_SW;
#endif

#ifdef HW_PATH
constexpr auto execution_path = DML_PATH_HW;
#endif

#ifdef AUTO_PATH
constexpr auto execution_path = DML_PATH_AUTO;
#endif
#endif

#ifdef CPP_API
#include <dml/dml.hpp>

#ifdef SW_PATH
using execution_path = dml::software;
#endif

#ifdef HW_PATH
using execution_path = dml::hardware;
#endif

#ifdef AUTO_PATH
using execution_path = dml::automatic;
#endif
#endif

#endif// ACTUAL_API_HPP
