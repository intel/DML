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

#ifndef DML_ML_DEVICE_HPP
#define DML_ML_DEVICE_HPP

#include "completion_record.hpp"
#include "core.hpp"
#include "make_descriptor.hpp"
#include "status.hpp"

namespace dml::ml
{
    /**
     * @todo
     */
    class device
    {
    public:
        /**
         * @todo
         */
        device() = default;

        /**
         * @todo
         */
        virtual ~device() = default;

        /**
         * @todo
         */
        virtual submission_status submit(descriptor& dsc, completion_record& record) noexcept = 0;
    };

    /**
     * @todo
     */
    class software: public device
    {
    public:
        /**
         * @todo
         */
        software() = default;

        /**
         * @todo
         */
        ~software() override = default;

        /**
         * @todo
         */
        submission_status submit(descriptor& dsc, completion_record& record) noexcept override
        {
            views::any_descriptor(dsc).completion_record_address() = reinterpret_cast<address_t>(&record);
            static_cast<void>(core::submit(dsc));

            return submission_status::success;
        }
    };

    /**
     * @todo
     */
    class hardware: public device
    {
    public:
        /**
         * @todo
         */
        hardware() = default;

        /**
         * @todo
         */
        ~hardware() override = default;

#ifdef DML_HW
        /**
         * @todo
         */
        submission_status submit(descriptor& dsc, completion_record& record) noexcept override;
#else
        /**
         * @todo
         */
        submission_status submit(descriptor& dsc, completion_record& record) noexcept override
        {
            return submission_status::failure;
        }
#endif
    };

}  // namespace dml::ml

#endif  //DML_ML_DEVICE_HPP
