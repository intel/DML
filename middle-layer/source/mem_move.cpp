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

#include "own/definitions.hpp"
#include "own/types.hpp"

#include <dml_ml/mem_move.hpp>
#include <dml_ml/result.hpp>

#include <core_api.h>

namespace dml::ml
{
    DML_PACKED_STRUCT_DECLARATION_BEGIN(mem_move_descriptor)
    {
        uint32_t      privilege_control{};        /**< Unused */
        hw_option     general_flags{};            /**< Contains a common flags for different operations */
        uint8_t       operation_specific_flags{}; /**< Contains a specific flags for operation  */
        hw_operation  operation_type{};           /**< Contains an operation type @ref dml_operation_t */
        result *      completion_record_ptr{};    /**< Pointer to the completion record space */
        const byte_t *source_ptr{};               /**< Pointer to the source */
        byte_t *      destination_ptr{};          /**< Pointer to the destination */
        size_t        transfer_size{};            /**< Count of bytes to copy */
        byte_t        reserved_memory[28]{};      /**< Not used bytes in the descriptor */
    };
    DML_PACKED_STRUCT_DECLARATION_END

    DML_PACKED_STRUCT_DECLARATION_BEGIN(mem_move_completion_record)
    {
        hw_status status{};            /**< Status of the executed task: success or some Error */
        result_t  result{};            /**< Result of completed operation                      */
        uint8_t   reserved_1[2]{};     /**< Reserved bytes                                     */
        uint32_t  bytes_completed{};   /**< Count of processed elements (bytes, words and etc.)*/
        uint8_t * fault_address_ptr{}; /**< Address of Page Fault */
        uint8_t   reserved_2[16]{};    /**< Reserved bytes        */
    };
    DML_PACKED_STRUCT_DECLARATION_END

    mem_move::mem_move(const byte_t *src, byte_t *dst, size_t size) noexcept: operation_()
    {
        auto &descriptor = *reinterpret_cast<mem_move_descriptor *>(operation_.data());

        descriptor.operation_type = hw_operation::mem_move;
        descriptor.general_flags  = hw_option::cache_control;

        descriptor.source_ptr      = src;
        descriptor.destination_ptr = dst;
        descriptor.transfer_size   = size;
    }

    void mem_move::operator()() const noexcept
    {
        auto dsc    = reinterpret_cast<const mem_move_descriptor *>(operation_.data());
        auto record = reinterpret_cast<mem_move_completion_record *>(dsc->completion_record_ptr);

        // No fail expected due to range check before
        auto status = dmlc_move_8u(dsc->source_ptr, dsc->destination_ptr, dsc->transfer_size);

        if (status != DML_STATUS_OK)
        {
            record->status = hw_status::internal_error;
        }
        else
        {
            record->status = hw_status::success;
        }
    }

    result::operator mem_move_result() const noexcept
    {
        auto record = reinterpret_cast<const mem_move_completion_record *>(data_);

        auto status    = (record->status == hw_status::success) ? status_code::ok : status_code::execution_failed;
        auto direction = record->result;

        return {status, direction};
    }
}  // namespace dml::ml
