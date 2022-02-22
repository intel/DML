/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <dml_dif.h>

#include <core/utils.hpp>
#include <cstring>
#include <dml/detail/common/status.hpp>
#include <dml/detail/common/utils/enum.hpp>

#include "../../../../include/dml/dmldefs.h"
#include "immintrin.h"
#include "kernels.hpp"

namespace dml::core::kernels
{
    void dif_update(const_view<descriptor, operation::dif_update> dsc) noexcept
    {
        auto record = make_view<operation::dif_update>(get_completion_record(dsc));

        const auto src              = reinterpret_cast<byte_t *>(dsc.source_address());
        const auto dst              = reinterpret_cast<byte_t *>(dsc.destination_address());
        const auto transfer_size    = dsc.transfer_size();
        const auto options          = dsc.flags();
        const auto dif_options      = dsc.dif_flags();
        const auto dif_src_options  = dsc.source_dif_flags();
        const auto dif_dst_options  = dsc.destination_dif_flags();
        const auto src_app_tag_mask = dsc.source_app_tag_mask();
        const auto dst_app_tag_mask = dsc.destination_app_tag_mask();

        auto src_ref_tag = dsc.source_ref_tag();
        auto dst_ref_tag = dsc.destination_ref_tag();
        auto src_app_tag = dsc.source_app_tag();
        auto dst_app_tag = dsc.destination_app_tag();

        dml_job_t job;
        memset(&job, 0, sizeof(dml_job_t));
        job.source_first_ptr                            = src;
        job.destination_first_ptr                       = dst;
        job.source_length                               = transfer_size;
        job.operation                                   = DML_OP_DIF_UPDATE;
        job.dif_config.source_reference_tag_seed        = src_ref_tag;
        job.dif_config.source_application_tag_seed      = src_app_tag;
        job.dif_config.source_application_tag_mask      = src_app_tag_mask;
        job.dif_config.destination_reference_tag_seed   = dst_ref_tag;
        job.dif_config.destination_application_tag_seed = dst_app_tag;
        job.dif_config.destination_application_tag_mask = dst_app_tag_mask;
        job.dif_config.block_size                       = static_cast<dml_dif_block_size_t>(dif_options & 0b11);

        // Job API composes DIF flags into one 64-bit value via shifting, check dmldefs.h
        job.dif_config.flags = (uint64_t(dif_options) << 16) | (uint64_t(dif_dst_options) << 8) | dif_src_options;

        job.flags = static_cast<flags_t>(options);

        auto status = dml_legacy_dif_update(&job);

        record.dif_status()      = job.result;
        record.bytes_completed() = job.offset;
        // TODO: Tags should be written

        _mm_mfence();
        record.status() = to_underlying((status == DML_STATUS_OK) ? dml::detail::execution_status::success
                                                                  : dml::detail::execution_status::dif_control_error);
    }
}  // namespace dml::core::kernels
