/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include <gtest/gtest.h>
#include <dml/dml.h>
#include <t_job.hpp>
#include <t_common.hpp>

namespace dml
{

TEST(dml_wait, busy_polling)
{
    auto transfer_size = 128u * 1024u; // 128KB

    auto src = std::vector<std::uint8_t>(transfer_size, 1);
    auto dst = std::vector<std::uint8_t>(transfer_size, 0);

    auto job = test::job_t(test::variables_t::path);
    job->operation = DML_OP_MEM_MOVE;
    job->source_first_ptr = src.data();
    job->destination_first_ptr = dst.data();
    job->source_length = transfer_size;
    job->flags |= DML_FLAG_PREFETCH_CACHE;

    ASSERT_EQ(DML_STATUS_OK, dml_submit_job(&(*job)));
    ASSERT_EQ(DML_STATUS_OK, dml_wait_job(&(*job), DML_WAIT_MODE_BUSY_POLL));

    ASSERT_EQ(src, dst);
}

TEST(dml_wait, umwait)
{
    auto transfer_size = 128u * 1024u; // 128KB

    auto src = std::vector<std::uint8_t>(transfer_size, 1);
    auto dst = std::vector<std::uint8_t>(transfer_size, 0);

    auto job = test::job_t(test::variables_t::path);
    job->operation = DML_OP_MEM_MOVE;
    job->source_first_ptr = src.data();
    job->destination_first_ptr = dst.data();
    job->source_length = transfer_size;
    job->flags |= DML_FLAG_PREFETCH_CACHE;

    ASSERT_EQ(DML_STATUS_OK, dml_submit_job(&(*job)));
    ASSERT_EQ(DML_STATUS_OK, dml_wait_job(&(*job), DML_WAIT_MODE_UMWAIT));

    ASSERT_EQ(src, dst);
}
}
