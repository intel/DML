/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_COMMON_RESULT_HPP
#define DML_COMMON_RESULT_HPP

/**
 * @date 05/19/2021
 * @brief Contains definitions of result types
 */

#include <cstdint>
#include <dml/hl/status_code.hpp>
#include <dml/hl/types.hpp>

namespace dml
{
    /**
     * @defgroup dml_cpp_common_results Results
     * @ingroup dml_cpp_common
     * @brief Contains result types descriptions
     * @{
     */

    /**
     * @brief Result for @ref mem_move_operation
     */
    struct mem_move_result
    {
        status_code status{ status_code::error }; /**< Status of operation execution */
    };

    /**
     * @brief Result for @ref mem_copy_operation
     */
    struct mem_copy_result
    {
        status_code status{ status_code::error }; /**< Status of operation execution */
    };

    /**
     * @brief Result for @ref fill_operation
     */
    struct fill_result
    {
        status_code status{ status_code::error }; /**< Status of operation execution */
    };

    /**
     * @brief Result for @ref dualcast_operation
     */
    struct dualcast_result
    {
        status_code status{ status_code::error }; /**< Status of operation execution */
    };

    /**
     * @brief Result for @ref compare_operation
     */
    struct compare_result
    {
        status_code       status{ status_code::error }; /**< Status of operation execution */
        comparison_result result{};                     /**< Comparison result */
        size_t            mismatch{};                   /**< First mismatch byte position */
    };

    /**
     * @brief Result for @ref create_delta_operation
     */
    struct create_delta_result
    {
        status_code       status{ status_code::error }; /**< Status of operation execution */
        comparison_result result{};                     /**< Comparison result */
        size_t            bytes_completed{};            /**< Bytes completed, before delta overflowed */
        size_t            delta_record_size{};          /**< Delta record written size */
    };

    /**
     * @brief Result for @ref apply_delta_operation
     */
    struct apply_delta_result
    {
        status_code status{ status_code::error }; /**< Status of operation execution */
    };

    /**
     * @brief Result for @ref crc_operation
     */
    struct crc_result
    {
        status_code status{ status_code::error }; /**< Status of operation execution */
        uint32_t    crc_value{};                  /**< Calculated CRC value */
    };

    /**
     * @brief Result for @ref cache_flush_operation
     */
    struct cache_flush_result
    {
        status_code status{ status_code::error }; /**< Status of operation execution */
    };

    /**
     * @brief Result for @ref batch_operation
     */
    struct batch_result
    {
        status_code status{ status_code::error }; /**< Status of operation execution */
        size_t      operations_completed{};       /**< Number of operation successfully completed */
    };

    /**
     * @}
     */
}  // namespace dml

#endif  //DML_COMMON_RESULT_HPP
