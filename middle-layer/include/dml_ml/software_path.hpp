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
 * @brief Contains definitions of @ref dml::ml::software_path type
 */

#ifndef DML_ML_SOFTWARE_HPP
#define DML_ML_SOFTWARE_HPP

#include <dml_ml/result.hpp>

namespace dml::ml
{
    /**
     * @defgroup dmlml_execution_paths Execution Paths
     * @ingroup dmlml
     * @brief Contains descriptions of supported execution paths
     */

    /**
     * @ingroup dmlml_execution_paths
     * @brief Defines function related to software execution path
     */
    struct software_path
    {
        /**
         * @brief Executes operation on CPU immediately
         *
         * @tparam operation_t Type of operation
         * @param op  Operation
         * @param res Reference to result instance
         *
         * @return Status code which is always successful
         */
        template <typename operation_t>
        static status_code submit(operation_t op, result& res) noexcept
        {
            static_cast<operation&>(op).associate(res);
            op();

            return status_code::ok;
        }
    };
}  // namespace dml::ml

#endif  //DML_SOFTWARE_HPP
