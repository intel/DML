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
 * @brief Contains definitions of @ref dml::ml::completion_record type
 */

#ifndef DML_ML_COMPLETION_RECORD_HPP
#define DML_ML_COMPLETION_RECORD_HPP

#include "types.hpp"
#include "awaiter.hpp"

#include <type_traits>

namespace dml::ml
{
    /**
     * @todo
     */
    struct alignas(32u) completion_record
    {
        byte_t bytes[32u]; /**< @todo */
    };

    /**
     * @todo
     */
    inline void wait(volatile completion_record &record) noexcept
    {
        awaiter wait_for(static_cast<volatile void *>(&record), 0);
    }

    /**
     * @todo
     */
    inline bool is_finished(const completion_record &record) noexcept
    {
        return 0 != record.bytes[0];
    }

}  // namespace dml::ml

#endif  //DML_ML_COMPLETION_RECORD_HPP
