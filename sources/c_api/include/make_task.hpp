/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_MAKE_DESCRIPTOR_HPP
#define DML_MAKE_DESCRIPTOR_HPP

#include <dml/detail/ml/make_task.hpp>
#include <dml/detail/ml/view.hpp>

#include "utils.hpp"

namespace dml
{
    inline auto make_task(job_view job) noexcept
    {
        switch (job.operation())
        {
            case DML_OP_NOP:
                return detail::ml::make_nop_task(detail::ml::nop_options(job.flags()));
                break;
            case DML_OP_BATCH:
                return detail::ml::make_batch_task(
                    reinterpret_cast<const dml::detail::descriptor*>(dml::align(job.destination_first())),
                    job.destination_length() / (sizeof(dml::detail::descriptor) + sizeof(dml::detail::completion_record)),
                    detail::ml::batch_options(job.flags()));
                break;
            case DML_OP_DRAIN:
                return detail::ml::make_drain_task(reinterpret_cast<detail::address_t>(job.destination_first()),
                                                   reinterpret_cast<detail::address_t>(job.destination_second()),
                                                   detail::ml::drain_options(job.flags()),
                                                   detail::ml::drain_specific_options(job.specific_flags()));
                break;
            case DML_OP_MEM_MOVE:
                return detail::ml::make_mem_move_task(job.source_first(),
                                                      job.destination_first(),
                                                      job.source_length(),
                                                      detail::ml::mem_move_options(job.flags()));
                break;
            case DML_OP_FILL:
                return detail::ml::make_fill_task(job.pattern(),
                                                  job.destination_first(),
                                                  job.destination_length(),
                                                  detail::ml::fill_options(job.flags()));
                break;
            case DML_OP_DUALCAST:
                return detail::ml::make_dualcast_task(job.source_first(),
                                                      job.destination_first(),
                                                      job.destination_second(),
                                                      job.source_length(),
                                                      detail::ml::dualcast_options(job.flags()),
                                                      detail::ml::dualcast_specific_options(job.specific_flags()));
                break;
            case DML_OP_COMPARE:
                return detail::ml::make_compare_task(job.source_first(),
                                                     job.source_second(),
                                                     job.source_length(),
                                                     detail::ml::compare_options(job.flags()),
                                                     detail::compare_result(job.expected_result()));
                break;
            case DML_OP_COMPARE_PATTERN:
                return detail::ml::make_compare_pattern_task(job.pattern(),
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

                    return detail::ml::make_crc_task(job.source_first(),
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

                    return detail::ml::make_copy_crc_task(job.source_first(),
                                                          job.destination_first(),
                                                          job.source_length(),
                                                          crc_seed,
                                                          detail::ml::copy_crc_options(job.flags()),
                                                          crc_options);
                }
                break;
            case DML_OP_DELTA_CREATE:
                return detail::ml::make_create_delta_task(job.source_first(),
                                                          job.source_second(),
                                                          job.source_length(),
                                                          job.destination_first(),
                                                          job.destination_length(),
                                                          detail::ml::create_delta_options(job.flags()),
                                                          detail::create_delta_result(job.expected_result()));
                break;
            case DML_OP_DELTA_APPLY:
                return detail::ml::make_apply_delta_task(job.source_first(),
                                                         job.source_length(),
                                                         job.destination_first(),
                                                         job.destination_length(),
                                                         detail::ml::apply_delta_options(job.flags()));
                break;
            case DML_OP_DIF_CHECK:
                return detail::ml::make_dif_check_task(job.source_first(),
                                                       job.source_length(),
                                                       { job.src_ref_tag(), job.src_app_tag_mask(), job.src_app_tag() },
                                                       detail::ml::dif_check_options(job.flags()),
                                                       detail::ml::dif_specific_options(job.dif_flags()),
                                                       detail::ml::dif_source_options(job.dif_src_flags()));
                break;
            case DML_OP_DIF_INSERT:
                return detail::ml::make_dif_insert_task(job.source_first(),
                                                        job.destination_first(),
                                                        job.source_length(),
                                                        { job.dst_ref_tag(), job.dst_app_tag_mask(), job.dst_app_tag() },
                                                        detail::ml::dif_insert_options(job.flags()),
                                                        detail::ml::dif_specific_options(job.dif_flags()),
                                                        detail::ml::dif_destination_options(job.dif_dst_flags()));
                break;
            case DML_OP_DIF_STRIP:
                return detail::ml::make_dif_strip_task(job.source_first(),
                                                       job.destination_first(),
                                                       job.source_length(),
                                                       { job.src_ref_tag(), job.src_app_tag_mask(), job.src_app_tag() },
                                                       detail::ml::dif_strip_options(job.flags()),
                                                       detail::ml::dif_specific_options(job.dif_flags()),
                                                       detail::ml::dif_source_options(job.dif_src_flags()));
                break;
            case DML_OP_DIF_UPDATE:
                return detail::ml::make_dif_update_task(job.source_first(),
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
                return detail::ml::make_cache_flush_task(job.destination_first(),
                                                         job.destination_length(),
                                                         detail::ml::cache_flush_options(job.flags()));
                break;
        }

        // Nop is safe
        return detail::ml::make_nop_task(detail::ml::nop_options(job.flags()));
    }
}  // namespace dml

#endif  // DML_MAKE_DESCRIPTOR_HPP
