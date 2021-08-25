/*
 * Copyright 2020-2021 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they
 * were provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit
 * this software or the related documents without Intel's prior written
 * permission.
 *
 * This software and the related documents are provided as is, with no
 * express or implied warranties, other than those that are expressly
 * stated in the License.
 *
 */

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

#include <dml/data_view.hpp>
#include <dml/execute.hpp>
#include <dml/execution_interface.hpp>
#include <dml/execution_path.hpp>
#include <dml/operations.hpp>
#include <dml/sequence.hpp>
#include <dml/submit.hpp>

#endif  //DML_DML_HPP
