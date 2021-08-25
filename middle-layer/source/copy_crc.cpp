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

#include <dml_ml/copy_crc.hpp>
#include <dml_ml/result.hpp>

#include <core_api.h>

namespace dml::ml
{
    DML_PACKED_STRUCT_DECLARATION_BEGIN(copy_crc_descriptor)
    {
        uint32_t        privilege_control{};     /**< Unused */
        hw_option       general_flags{};         /**< Contains a common flags for different operations */
        crc_option      crc_options{};           /**< Contains a specific flags for operation  */
        hw_operation    operation_type{};        /**< Contains an operation type @ref dml_operation_t */
        result *        completion_record_ptr{}; /**< Pointer to the completion record space */
        const byte_t *  source_ptr{};            /**< Pointer to the source */
        byte_t *        destination_ptr{};       /**< Pointer to the destination */
        size_t          transfer_size{};         /**< Count of bytes to copy */
        byte_t          reserved_memory2[4]{};   /**< Not used bytes in the descriptor */
        uint32_t        copy_crc_seed{};         /**< CRC Seed value */
        byte_t          reserved_memory3[4]{};   /**< Not used bytes in the descriptor */
        const uint32_t *copy_crc_seed_address{}; /**< CRC Seed address */
        byte_t          reserved_memory[8]{};    /**< Not used bytes in the descriptor */
    };
    DML_PACKED_STRUCT_DECLARATION_END

    DML_PACKED_STRUCT_DECLARATION_BEGIN(copy_crc_completion_record)
    {
        hw_status status{};            /**< Status of the executed task: success or some Error */
        result_t  result{};            /**< Result of completed operation                      */
        uint8_t   reserved_1[2]{};     /**< Reserved bytes                                     */
        uint32_t  bytes_completed{};   /**< Count of processed elements (bytes, words and etc.)*/
        uint8_t * fault_address_ptr{}; /**< Address of Page Fault */
        uint32_t  copy_crc_value{};    /**< Computed CRC value*/
        uint8_t   reserved_2[12]{};    /**< Reserved bytes        */
    };
    DML_PACKED_STRUCT_DECLARATION_END

    copy_crc::copy_crc(const byte_t  *src,
                       byte_t        *dst,
                       const size_t   size,
                       const uint32_t copy_crc_seed,
                       crc_parameters parameters) noexcept:
        operation_()
    {
        auto &descriptor = *reinterpret_cast<copy_crc_descriptor *>(operation_.data());

        descriptor.operation_type = hw_operation::copy_crc;
        descriptor.general_flags  = hw_option::cache_control;

        descriptor.source_ptr      = src;
        descriptor.destination_ptr = dst;
        descriptor.transfer_size   = size;
        descriptor.copy_crc_seed   = copy_crc_seed;

        if (parameters.bypass_reflection)
        {
            descriptor.crc_options = descriptor.crc_options | crc_option::bypass_reflection;
        }

        if (parameters.bypass_data_reflection)
        {
            descriptor.crc_options = descriptor.crc_options | crc_option::bypass_data_reflection;
        }
    }

    void copy_crc::operator()() const noexcept
    {
        constexpr auto polynomial = 0x1EDC6F41u;
        auto           reverse    = [](uint32_t value)
        {
            value = (value & 0x55555555u) << 1u | (value & 0xAAAAAAAAu) >> 1u;
            value = (value & 0x33333333u) << 2u | (value & 0xCCCCCCCCu) >> 2u;
            value = (value & 0x0F0F0F0Fu) << 4u | (value & 0xF0F0F0F0u) >> 4u;
            value = (value & 0x00FF00FFu) << 8u | (value & 0xFF00FF00u) >> 8u;
            value = (value & 0x0000FFFFu) << 16u | (value & 0xFFFF0000u) >> 16u;

            return value;
        };

        auto dsc    = reinterpret_cast<const copy_crc_descriptor *>(operation_.data());
        auto record = reinterpret_cast<copy_crc_completion_record *>(dsc->completion_record_ptr);

        // No fail expected due to range check before
        auto status = dmlc_copy_8u(dsc->source_ptr, dsc->destination_ptr, dsc->transfer_size);


        if (status != DML_STATUS_OK)
        {
            record->status = hw_status::internal_error;
            return;  // Copy fail, skip CRC
        }

        auto bypass_reflection      = any(dsc->crc_options, crc_option::bypass_reflection);
        auto bypass_data_reflection = any(dsc->crc_options, crc_option::bypass_data_reflection);

        auto crc_value = dsc->copy_crc_seed;

        // Bypass inversion and use reverse bit order for CRC result
        if (!bypass_reflection)
        {
            crc_value = ~(crc_value);
            crc_value = reverse(crc_value);
        }

        // Bypass Data Reflection in case if DML_FLAG_DATA_REFLECTION set
        status = (!bypass_data_reflection)
                     ? dmlc_calculate_crc_reflected_32u(dsc->source_ptr, dsc->transfer_size, &crc_value, polynomial)
                     : dmlc_calculate_crc_32u(dsc->source_ptr, dsc->transfer_size, &crc_value, polynomial);

        // Bypass inversion and use reverse bit order for CRC result
        if (!bypass_reflection)
        {
            crc_value = reverse(crc_value);
            crc_value = ~(crc_value);
        }

        if (status != DML_STATUS_OK)
        {
            record->status         = hw_status::internal_error;
            record->copy_crc_value = crc_value;
        }
        else
        {
            record->status         = hw_status::success;
            record->copy_crc_value = crc_value;
        }
    }
}  // namespace dml::ml
