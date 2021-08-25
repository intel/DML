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

#include <dml_ml/create_delta.hpp>
#include <dml_ml/result.hpp>

#include <core_api.h>

namespace dml::ml
{
    DML_PACKED_STRUCT_DECLARATION_BEGIN(create_delta_descriptor)
    {
        uint32_t      privilege_control{};        /**< Unused */
        hw_option     general_flags{};            /**< Contains a common flags for different operations */
        uint8_t       operation_specific_flags{}; /**< Contains a specific flags for operation  */
        hw_operation  operation_type{};           /**< Contains an operation type @ref dml_operation_t */
        result *      completion_record_ptr{};    /**< Pointer to the completion record space */
        const byte_t *source_ptr1{};              /**< Pointer to the source 1 */
        const byte_t *source_ptr2{};              /**< Pointer to the source 2 */
        size_t        transfer_size{};            /**< Count of bytes to copy */
        byte_t        reserved_memory1[4]{};      /**< Not used bytes in the descriptor */
        byte_t *      delta_record{};             /**< Pointer to the delta record */
        size_t        delta_max_size{};           /**< Max size for delta record */
        byte_t        reserved_memory2[4]{};      /**< Not used bytes in the descriptor */
        result_t      expected_result{};          /**< Expected result */
        byte_t        reserved_memory3[7]{};      /**< Not used bytes in the descriptor */
    };
    DML_PACKED_STRUCT_DECLARATION_END

    DML_PACKED_STRUCT_DECLARATION_BEGIN(create_delta_completion_record)
    {
        hw_status status{};            /**< Status of the executed task: success or some Error */
        result_t  result{};            /**< Result of completed operation                      */
        uint8_t   reserved_1[2]{};     /**< Reserved bytes                                     */
        uint32_t  bytes_completed{};   /**< Count of processed elements (bytes, words and etc.)*/
        uint8_t * fault_address_ptr{}; /**< Address of Page Fault */
        size_t    delta_record_size{}; /**< Calculated delta record size*/
        uint8_t   reserved_2[16]{};    /**< Reserved bytes        */
    };
    DML_PACKED_STRUCT_DECLARATION_END

    create_delta::create_delta(const byte_t *src1,
                               const byte_t *src2,
                               size_t        size,
                               byte_t *      delta_record,
                               size_t        delta_max_size) noexcept:
        operation_()
    {
        auto &descriptor = *reinterpret_cast<create_delta_descriptor *>(operation_.data());

        descriptor.operation_type = hw_operation::create_delta;
        descriptor.general_flags  = hw_option::cache_control;

        descriptor.source_ptr1    = src1;
        descriptor.source_ptr2    = src2;
        descriptor.transfer_size  = size;
        descriptor.delta_record   = delta_record;
        descriptor.delta_max_size = delta_max_size;
    }

    void create_delta::operator()() const noexcept
    {
        auto dsc    = reinterpret_cast<const create_delta_descriptor *>(operation_.data());
        auto record = reinterpret_cast<create_delta_completion_record *>(dsc->completion_record_ptr);

        auto delta_record_size = uint32_t();

        // No fail expected due to range check before
        // Flip src1 and src2 due to core differs from hardware spec
        auto status = dmlc_create_delta_record_8u(dsc->source_ptr2,
                                                  dsc->source_ptr1,
                                                  dsc->transfer_size,
                                                  dsc->delta_max_size,
                                                  dsc->delta_record,
                                                  &delta_record_size);
        record->delta_record_size = delta_record_size;

        if (status == DML_STATUS_MEMORY_OVERFLOW_ERROR)
        {
            record->result          = 2;
            record->bytes_completed = 0;  // Core does not report the value
            record->status          = hw_status::success;
        }
        else if (record->delta_record_size != 0)
        {
            record->result = 1;
            record->status = hw_status::success;
        }
        else if (status != DML_STATUS_OK)
        {
            record->status = hw_status::internal_error;
        }
        else
        {
            record->status = hw_status::success;
        }
    }

    result::operator create_delta_result() const noexcept
    {
        auto record = reinterpret_cast<const create_delta_completion_record *>(data_);

        auto status          = (record->status == hw_status::success) ? status_code::ok : status_code::execution_failed;
        auto result          = record->result;
        auto bytes_processed = record->bytes_completed;
        auto size            = record->delta_record_size;

        return {status, result, bytes_processed, size};
    }
}  // namespace dml::ml
