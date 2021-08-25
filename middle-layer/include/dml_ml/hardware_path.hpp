/*
 * Copyright 2021 Intel Corporation.
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

/**
 * @date 05/19/2021
 * @brief Contains definitions of @ref dml::ml::hardware_path type
 */

#ifndef DML_ML_HARDWARE_HPP
#define DML_ML_HARDWARE_HPP

#include <dml_ml/operation.hpp>
#include <dml_ml/result.hpp>

#include <type_traits>

namespace dml::ml
{
    /**
     * @ingroup dmlml_execution_paths
     * @brief Defines function related to hardware execution path
     */
    class hardware_path
    {
    public:
        /**
         * @brief Submits an operation onto a dedicated hardware
         *
         * @param op   Any operation
         * @param res  Reference to result instance
         *
         * @return
         *      - @ref status_code::ok if execution is started, an error code otherwise
         */
        static status_code submit(operation op, result& res) noexcept;
    };
}  // namespace dml::ml

#endif  //DML_HARDWARE_HPP
