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

#include <dml_ml/operation.hpp>

#include <dml_ml/apply_delta.hpp>
#include <dml_ml/cache_flush.hpp>
#include <dml_ml/compare.hpp>
#include <dml_ml/compare_pattern.hpp>
#include <dml_ml/copy_crc.hpp>
#include <dml_ml/crc.hpp>
#include <dml_ml/create_delta.hpp>
#include <dml_ml/dualcast.hpp>
#include <dml_ml/fill.hpp>
#include <dml_ml/mem_move.hpp>

namespace dml::ml
{
    DML_PACKED_STRUCT_DECLARATION_BEGIN(any_operation_descriptor)
    {
        byte_t       reserved_memory1[4]{};   /**< Not used bytes in the descriptor */
        hw_option    general_flags{};         /**< Contains a common flags for different operations */
        byte_t       reserved_memory2[1]{};   /**< Not used bytes in the descriptor */
        hw_operation operation_type{};        /**< Contains an operation type @ref dml_operation_t */
        result      *completion_record_ptr{}; /**< Pointer to the completion record space */
        byte_t       reserved_memory3[48]{};  /**< Not used bytes in the descriptor */
    };
    DML_PACKED_STRUCT_DECLARATION_END

    void operation::operator()() const noexcept
    {
        auto dsc = reinterpret_cast<const any_operation_descriptor *>(data_);

        switch (dsc->operation_type)
        {
            case hw_operation::nop:
                break;
            case hw_operation::batch:
                break;
            case hw_operation::drain:
                break;
            case hw_operation::mem_move:
                reinterpret_cast<const mem_move *>(data_)->operator()();
                break;
            case hw_operation::fill:
                reinterpret_cast<const fill *>(data_)->operator()();
                break;
            case hw_operation::compare:
                reinterpret_cast<const compare *>(data_)->operator()();
                break;
            case hw_operation::compare_pattern:
                reinterpret_cast<const compare_pattern *>(data_)->operator()();
                break;
            case hw_operation::create_delta:
                reinterpret_cast<const create_delta *>(data_)->operator()();
                break;
            case hw_operation::apply_delta:
                reinterpret_cast<const apply_delta *>(data_)->operator()();
                break;
            case hw_operation::dualcast:
                reinterpret_cast<const dualcast *>(data_)->operator()();
                break;
            case hw_operation::crc:
                reinterpret_cast<const crc *>(data_)->operator()();
                break;
            case hw_operation::copy_crc:
                reinterpret_cast<const copy_crc *>(data_)->operator()();
                break;
            case hw_operation::dif_check:
                break;
            case hw_operation::dif_insert:
                break;
            case hw_operation::dif_strip:
                break;
            case hw_operation::dif_update:
                break;
            case hw_operation::cache_flush:
                reinterpret_cast<const cache_flush *>(data_)->operator()();
                break;
        }
    }

    void operation::associate(result &record) noexcept
    {
        // Associate record with this descriptor
        auto dsc                   = reinterpret_cast<any_operation_descriptor *>(this);
        dsc->completion_record_ptr = &record;

        // Set first byte of completion record as 0
        auto record_first_byte = reinterpret_cast<byte_t *>(dsc->completion_record_ptr);
        *record_first_byte     = 0u;

        // Set flags to request completion record
        dsc->general_flags = dsc->general_flags | hw_use_completion_record;
    }
}  // namespace dml::ml
