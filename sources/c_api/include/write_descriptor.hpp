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

#include <dml/cpp/middle_layer/make_descriptor.hpp>

namespace dml
{
    inline auto write_descriptor(job_view job) noexcept
    {
        switch (job.operation())
        {
            case DML_OP_NOP:
                job.state().dsc = ml::make_nop_descriptor(ml::nop_options(job.flags()));
                break;
            case DML_OP_BATCH:
                job.state().dsc =
                    ml::make_batch_descriptor(reinterpret_cast<const ml::descriptor*>(job.destination_first()),
                                              job.destination_length() / (sizeof(dml::ml::descriptor) + sizeof(dml::ml::completion_record)),
                                              ml::batch_options(job.flags()));
                break;
            case DML_OP_DRAIN:
                job.state().dsc = ml::make_drain_descriptor(reinterpret_cast<ml::address_t>(job.destination_first()),
                                                            reinterpret_cast<ml::address_t>(job.destination_second()),
                                                            ml::drain_options(job.flags()),
                                                            ml::drain_additional_options(job.specific_flags()));
                break;
            case DML_OP_MEM_MOVE:
                job.state().dsc = ml::make_mem_move_descriptor(job.source_first(),
                                                               job.destination_first(),
                                                               job.source_length(),
                                                               ml::mem_move_options(job.flags()));
                break;
            case DML_OP_FILL:
                job.state().dsc = ml::make_fill_descriptor(job.pattern(),
                                                           job.destination_first(),
                                                           job.destination_length(),
                                                           ml::fill_options(job.flags()));
                break;
            case DML_OP_DUALCAST:
                job.state().dsc = ml::make_dualcast_descriptor(job.source_first(),
                                                               job.destination_first(),
                                                               job.destination_second(),
                                                               job.source_length(),
                                                               ml::dualcast_options(job.flags()),
                                                               ml::dualcast_additional_options(job.specific_flags()));
                break;
            case DML_OP_COMPARE:
                job.state().dsc = ml::make_compare_descriptor(job.source_first(),
                                                              job.source_second(),
                                                              job.source_length(),
                                                              ml::compare_options(job.flags()),
                                                              ml::compare_expected_result_options(job.expected_result()));
                break;
            case DML_OP_COMPARE_PATTERN:
                job.state().dsc = ml::make_compare_pattern_descriptor(job.pattern(),
                                                                      job.source_first(),
                                                                      job.source_length(),
                                                                      ml::compare_pattern_options(job.flags()),
                                                                      ml::compare_expected_result_options(job.expected_result()));
                break;
            case DML_OP_CRC:
                {
                    uint32_t crc_seed = 0;
                    auto crc_options = ml::crc_additional_options(job.specific_flags());

                    if (crc_options.contains(ml::crc_additional_option::read_crc_seed))
                    {
                        crc_seed = *job.crc_ptr();
                    }

                    // Erase read_crc_seed flag from enabled options
                    uint8_t mask = 0xFF ^ static_cast<uint8_t>(ml::crc_additional_option::read_crc_seed);
                    crc_options = ml::crc_additional_options(static_cast<uint8_t>(crc_options) & mask);

                    job.state().dsc = ml::make_crc_descriptor(job.source_first(),
                                                              job.source_length(),
                                                              crc_seed,
                                                              ml::crc_options(job.flags()),
                                                              crc_options);
                    break;
                }
            case DML_OP_COPY_CRC:
                {
                    uint32_t crc_seed = 0;
                    auto crc_options = ml::copy_crc_additional_options(job.specific_flags());

                    if (crc_options.contains(ml::crc_additional_option::read_crc_seed))
                    {
                        crc_seed = *job.crc_ptr();
                    }

                    // Erase read_crc_seed flag from enabled options
                    uint8_t mask = 0xFF ^ static_cast<uint8_t>(ml::crc_additional_option::read_crc_seed);
                    crc_options = ml::copy_crc_additional_options(static_cast<uint8_t>(crc_options) & mask);

                    job.state().dsc = ml::make_copy_crc_descriptor(job.source_first(),
                                                                   job.destination_first(),
                                                                   job.source_length(),
                                                                   crc_seed,
                                                                   ml::copy_crc_options(job.flags()),
                                                                   crc_options);
                }
                break;
            case DML_OP_DELTA_CREATE:
                job.state().dsc = ml::make_create_delta_descriptor(job.source_first(),
                                                                   job.source_second(),
                                                                   job.source_length(),
                                                                   job.destination_first(),
                                                                   job.destination_length(),
                                                                   ml::create_delta_options(job.flags()),
                                                                   ml::delta_expected_result_options(job.expected_result()));
                break;
            case DML_OP_DELTA_APPLY:
                job.state().dsc = ml::make_apply_delta_descriptor(job.source_first(),
                                                                  job.source_length(),
                                                                  job.destination_first(),
                                                                  job.destination_length(),
                                                                  ml::apply_delta_options(job.flags()));
                break;
            case DML_OP_DIF_CHECK:
                job.state().dsc = ml::make_dif_check_descriptor(job.source_first(),
                                                                job.source_length(),
                                                                { job.src_ref_tag(), job.src_app_tag_mask(), job.src_app_tag() },
                                                                ml::dif_check_options(job.flags()),
                                                                ml::dif_additional_options(job.dif_flags()),
                                                                ml::dif_additional_src_options(job.dif_src_flags()));
                break;
            case DML_OP_DIF_INSERT:
                job.state().dsc = ml::make_dif_insert_descriptor(job.source_first(),
                                                                 job.destination_first(),
                                                                 job.source_length(),
                                                                 { job.dst_ref_tag(), job.dst_app_tag_mask(), job.dst_app_tag() },
                                                                 ml::dif_insert_options(job.flags()),
                                                                 ml::dif_additional_options(job.dif_flags()),
                                                                 ml::dif_additional_dst_options(job.dif_dst_flags()));
                break;
            case DML_OP_DIF_STRIP:
                job.state().dsc = ml::make_dif_strip_descriptor(job.source_first(),
                                                                job.destination_first(),
                                                                job.source_length(),
                                                                { job.src_ref_tag(), job.src_app_tag_mask(), job.src_app_tag() },
                                                                ml::dif_strip_options(job.flags()),
                                                                ml::dif_additional_options(job.dif_flags()),
                                                                ml::dif_additional_src_options(job.dif_src_flags()));
                break;
            case DML_OP_DIF_UPDATE:
                job.state().dsc = ml::make_dif_update_descriptor(job.source_first(),
                                                                 job.destination_first(),
                                                                 job.source_length(),
                                                                 { job.src_ref_tag(), job.src_app_tag_mask(), job.src_app_tag() },
                                                                 { job.dst_ref_tag(), job.dst_app_tag_mask(), job.dst_app_tag() },
                                                                 ml::dif_update_options(job.flags()),
                                                                 ml::dif_additional_options(job.dif_flags()),
                                                                 ml::dif_additional_src_options(job.dif_src_flags()),
                                                                 ml::dif_additional_dst_options(job.dif_dst_flags()));
                break;
            case DML_OP_CACHE_FLUSH:
                job.state().dsc = ml::make_cache_flush_descriptor(job.destination_first(),
                                                                  job.destination_length(),
                                                                  ml::cache_flush_options(job.flags()));
                break;
            default:
                job.state().dsc = ml::descriptor{};
        }
    }
}  // namespace dml

#endif  // DML_MAKE_DESCRIPTOR_HPP
