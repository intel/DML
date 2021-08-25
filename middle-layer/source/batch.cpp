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

#include <dml_ml/batch.hpp>
#include <dml_ml/result.hpp>

#include <core_api.h>

namespace dml::ml
{
    DML_PACKED_STRUCT_DECLARATION_BEGIN(batch_descriptor)
    {
        uint32_t         privilege_control{};         /**< Unused */
        hw_option        general_flags{};             /**< Contains a common flags for different operations */
        uint8_t          operation_specific_flags{};  /**< Contains a specific flags for operation  */
        hw_operation     operation_type{};            /**< Contains an operation type @ref hw_operation */
        result *         completion_record_address{}; /**< Pointer to the completion record space */
        const operation *source{};                    /**< Pointer to the destination */
        byte_t           reserved_memory1[8]{};       /**< Not used bytes in the descriptor */
        size_t           operation_count{};           /**< Count of bytes to copy */
        byte_t           reserved_memory2[28]{};      /**< Not used bytes in the descriptor */
    };
    DML_PACKED_STRUCT_DECLARATION_END

    DML_PACKED_STRUCT_DECLARATION_BEGIN(batch_completion_record)
    {
        hw_status status{};                /**< Status of the executed task: success or some Error */
        uint8_t   reserved_1[3]{};         /**< Reserved bytes                                     */
        uint32_t  descriptors_completed{}; /**< Count of processed elements (bytes, words and etc.)*/
        byte_t *  fault_address{};         /**< Address of Page Fault */
        uint8_t   reserved_2[16]{};        /**< Reserved bytes        */
    };
    DML_PACKED_STRUCT_DECLARATION_END

    batch::batch(const operation *descriptors, size_t operation_count) noexcept: operation_()
    {
        auto &descriptor = *reinterpret_cast<batch_descriptor *>(operation_.data());

        descriptor.operation_type = hw_operation::batch;

        descriptor.source          = descriptors;
        descriptor.operation_count = operation_count;
    }

    void batch::operator()() const noexcept
    {
        auto dsc    = reinterpret_cast<const batch_descriptor *>(operation_.data());
        auto record = reinterpret_cast<batch_completion_record *>(dsc->completion_record_address);

        for (auto i = 0u; i < dsc->operation_count; ++i)
        {
            dsc->source[i].operator()();

            // It's fine to access completion record for  any operation through any type of descriptor
            auto op_record =
                reinterpret_cast<const batch_descriptor *>(dsc->source[i].data())->completion_record_address;
            if (!op_record->is_success())
            {
                record->status                = hw_status::batch_processing_error;
                record->descriptors_completed = i;
                return;
            }
        }

        record->status                = hw_status::success;
        record->descriptors_completed = dsc->operation_count;
    }

    result::operator batch_result() const noexcept
    {
        auto record = reinterpret_cast<const batch_completion_record *>(data_);

        auto status = (record->status == hw_status::success) ? status_code::ok : status_code::execution_failed;
        auto descriptors_completed = record->descriptors_completed;

        return {status, descriptors_completed};
    }
}  // namespace dml::ml
