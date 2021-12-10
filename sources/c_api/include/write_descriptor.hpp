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

#ifndef DML_MAKE_DESCRIPTOR_HPP
#define DML_MAKE_DESCRIPTOR_HPP

#include <dml/detail/ml/operation.hpp>

#include "utils.hpp"

namespace dml
{
    inline auto write_descriptor(job_view job) noexcept
    {
        switch (job.operation())
        {
            case DML_OP_NOP:
                job.state().dsc = detail::ml::make_nop_operation(detail::ml::nop_options(job.flags()));
                break;
            case DML_OP_BATCH:
                job.state().dsc = detail::ml::make_batch_operation(
                    reinterpret_cast<const dml::detail::ml::operation*>(dml::align(job.destination_first())),
                    job.destination_length() / (sizeof(dml::detail::ml::operation) + sizeof(dml::detail::ml::result)),
                    detail::ml::batch_options(job.flags()));
                break;
            case DML_OP_DRAIN:
                job.state().dsc = detail::ml::make_drain_operation(reinterpret_cast<detail::address_t>(job.destination_first()),
                                                                   reinterpret_cast<detail::address_t>(job.destination_second()),
                                                                   detail::ml::drain_options(job.flags()),
                                                                   detail::ml::drain_specific_options(job.specific_flags()));
                break;
            case DML_OP_MEM_MOVE:
                job.state().dsc = detail::ml::make_mem_move_operation(job.source_first(),
                                                                      job.destination_first(),
                                                                      job.source_length(),
                                                                      detail::ml::mem_move_options(job.flags()));
                break;
            case DML_OP_FILL:
                job.state().dsc = detail::ml::make_fill_operation(job.pattern(),
                                                                  job.destination_first(),
                                                                  job.destination_length(),
                                                                  detail::ml::fill_options(job.flags()));
                break;
            case DML_OP_DUALCAST:
                job.state().dsc = detail::ml::make_dualcast_operation(job.source_first(),
                                                                      job.destination_first(),
                                                                      job.destination_second(),
                                                                      job.source_length(),
                                                                      detail::ml::dualcast_options(job.flags()),
                                                                      detail::ml::dualcast_specific_options(job.specific_flags()));
                break;
            case DML_OP_COMPARE:
                job.state().dsc = detail::ml::make_compare_operation(job.source_first(),
                                                                     job.source_second(),
                                                                     job.source_length(),
                                                                     detail::ml::compare_options(job.flags()),
                                                                     detail::compare_result(job.expected_result()));
                break;
            case DML_OP_COMPARE_PATTERN:
                job.state().dsc = detail::ml::make_compare_pattern_operation(job.pattern(),
                                                                             job.source_first(),
                                                                             job.source_length(),
                                                                             detail::ml::compare_pattern_options(job.flags()),
                                                                             detail::compare_result(job.expected_result()));
                break;
            case DML_OP_CRC:
                {
                    uint32_t crc_seed    = 0;
                    auto     crc_options = detail::ml::crc_specific_options(job.specific_flags());

                    if (intersects(job.specific_flags(), detail::crc_specific_flag::read_crc_seed))
                    {
                        crc_seed = *job.crc_ptr();
                    }

                    // Erase read_crc_seed flag from enabled options
                    uint8_t mask = 0xFF ^ to_underlying(detail::crc_specific_flag::read_crc_seed);
                    crc_options  = detail::ml::crc_specific_options(static_cast<uint8_t>(crc_options) & mask);

                    job.state().dsc = detail::ml::make_crc_operation(job.source_first(),
                                                                     job.source_length(),
                                                                     crc_seed,
                                                                     detail::ml::crc_options(job.flags()),
                                                                     crc_options);
                    break;
                }
            case DML_OP_COPY_CRC:
                {
                    uint32_t crc_seed    = 0;
                    auto     crc_options = detail::ml::copy_crc_specific_options(job.specific_flags());

                    if (intersects(job.specific_flags(), detail::crc_specific_flag::read_crc_seed))
                    {
                        crc_seed = *job.crc_ptr();
                    }

                    // Erase read_crc_seed flag from enabled options
                    uint8_t mask = 0xFF ^ to_underlying(detail::crc_specific_flag::read_crc_seed);
                    crc_options  = detail::ml::copy_crc_specific_options(static_cast<uint8_t>(crc_options) & mask);

                    job.state().dsc = detail::ml::make_copy_crc_operation(job.source_first(),
                                                                          job.destination_first(),
                                                                          job.source_length(),
                                                                          crc_seed,
                                                                          detail::ml::copy_crc_options(job.flags()),
                                                                          crc_options);
                }
                break;
            case DML_OP_DELTA_CREATE:
                job.state().dsc = detail::ml::make_create_delta_operation(job.source_first(),
                                                                          job.source_second(),
                                                                          job.source_length(),
                                                                          job.destination_first(),
                                                                          job.destination_length(),
                                                                          detail::ml::create_delta_options(job.flags()),
                                                                          detail::create_delta_result(job.expected_result()));
                break;
            case DML_OP_DELTA_APPLY:
                job.state().dsc = detail::ml::make_apply_delta_operation(job.source_first(),
                                                                         job.source_length(),
                                                                         job.destination_first(),
                                                                         job.destination_length(),
                                                                         detail::ml::apply_delta_options(job.flags()));
                break;
            case DML_OP_DIF_CHECK:
                job.state().dsc = detail::ml::make_dif_check_operation(job.source_first(),
                                                                       job.source_length(),
                                                                       { job.src_ref_tag(), job.src_app_tag_mask(), job.src_app_tag() },
                                                                       detail::ml::dif_check_options(job.flags()),
                                                                       detail::ml::dif_specific_options(job.dif_flags()),
                                                                       detail::ml::dif_source_options(job.dif_src_flags()));
                break;
            case DML_OP_DIF_INSERT:
                job.state().dsc = detail::ml::make_dif_insert_operation(job.source_first(),
                                                                        job.destination_first(),
                                                                        job.source_length(),
                                                                        { job.dst_ref_tag(), job.dst_app_tag_mask(), job.dst_app_tag() },
                                                                        detail::ml::dif_insert_options(job.flags()),
                                                                        detail::ml::dif_specific_options(job.dif_flags()),
                                                                        detail::ml::dif_destination_options(job.dif_dst_flags()));
                break;
            case DML_OP_DIF_STRIP:
                job.state().dsc = detail::ml::make_dif_strip_operation(job.source_first(),
                                                                       job.destination_first(),
                                                                       job.source_length(),
                                                                       { job.src_ref_tag(), job.src_app_tag_mask(), job.src_app_tag() },
                                                                       detail::ml::dif_strip_options(job.flags()),
                                                                       detail::ml::dif_specific_options(job.dif_flags()),
                                                                       detail::ml::dif_source_options(job.dif_src_flags()));
                break;
            case DML_OP_DIF_UPDATE:
                job.state().dsc = detail::ml::make_dif_update_operation(job.source_first(),
                                                                        job.destination_first(),
                                                                        job.source_length(),
                                                                        { job.src_ref_tag(), job.src_app_tag_mask(), job.src_app_tag() },
                                                                        { job.dst_ref_tag(), job.dst_app_tag_mask(), job.dst_app_tag() },
                                                                        detail::ml::dif_update_options(job.flags()),
                                                                        detail::ml::dif_specific_options(job.dif_flags()),
                                                                        detail::ml::dif_source_options(job.dif_src_flags()),
                                                                        detail::ml::dif_destination_options(job.dif_dst_flags()));
                break;
            case DML_OP_CACHE_FLUSH:
                job.state().dsc = detail::ml::make_cache_flush_operation(job.destination_first(),
                                                                         job.destination_length(),
                                                                         detail::ml::cache_flush_options(job.flags()));
                break;
            default:
                job.state().dsc = dml::detail::ml::operation{};
        }
    }
}  // namespace dml

#endif  // DML_MAKE_DESCRIPTOR_HPP
