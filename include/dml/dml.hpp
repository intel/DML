/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_DML_HPP
#define DML_DML_HPP

/**
 * @date 11/25/2020
 * @defgroup dmlhl_api DML High Level API
 * @brief High Level C++ API for Intel(R) Data Mover Library (Intel® DML)
 */

/**
 * @defgroup dmlhl_aux Auxiliary
 * @ingroup dmlhl_api
 * @brief Contains supporting entities to use the library
 */

/**
 * @defgroup dmlhl_ops Operations
 * @ingroup dmlhl_api
 * @brief Contains operations used in API
 */

/**
 * @defgroup dmlhl_submit Asynchronous API
 * @ingroup dmlhl_api
 * @brief Contains asynchronous submit interfaces
 */

/**
 * @defgroup dmlhl_execute Sequential API
 * @ingroup dmlhl_api
 * @brief Contains synchronous execute interfaces
 */

/**
 * @ingroup dmlhl_api
 * @brief Global DML High Level API namespace
 */
namespace dml
{
}

#include <dml/hl/data_view.hpp>
#include <dml/hl/execute.hpp>
#include <dml/hl/execution_interface.hpp>
#include <dml/hl/execution_path.hpp>
#include <dml/hl/operations.hpp>
#include <dml/hl/sequence.hpp>
#include <dml/hl/submit.hpp>

#endif  //DML_DML_HPP
