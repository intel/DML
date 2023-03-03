/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TESTING_STATUS_HPP
#define DML_TESTING_STATUS_HPP

#include <ostream>
#include <utils/test.hpp>

namespace dml::testing
{
    class Status
    {
      public:
        constexpr explicit Status(std::string_view view) noexcept : view_(view) {}

#ifdef C_API
        inline explicit Status(dml_status_t code) noexcept;
#endif

#ifdef CPP_API
        inline explicit Status(dml::status_code code) noexcept;
#endif

        bool operator==(const Status &rhs) const { return view_ == rhs.view_; }

        friend std::ostream &operator<<(std::ostream &os, const Status &status)
        {
            os << status.view_;
            return os;
        }

      private:
        std::string_view view_;
    };

    struct StatusCode
    {
        static constexpr auto Success = Status("Success");
        static constexpr auto FalsePredicate = Status("FalsePredicate");
        static constexpr auto PageFault = Status("PageFault");
        static constexpr auto Unknown = Status("Unknown");
    };

#ifdef C_API
    inline Status::Status(dml_status_t code) noexcept
    {
        switch (code)
            {
                case DML_STATUS_OK:
                    view_ = StatusCode::Success.view_;
                    break;
                case DML_STATUS_FALSE_PREDICATE_OK:
                    view_ = StatusCode::FalsePredicate.view_;
                    break;
                case DML_STATUS_PAGE_FAULT_ERROR:
                    view_ = StatusCode::PageFault.view_;
                    break;
                default:
                    view_ = StatusCode::Unknown.view_;
            }
    }
#endif

#ifdef CPP_API
    inline Status::Status(dml::status_code code) noexcept
    {
        switch (code)
            {
                case status_code::ok:
                    view_ = StatusCode::Success.view_;
                    break;
                case status_code::false_predicate:
                    view_ = StatusCode::FalsePredicate.view_;
                    break;
                case status_code::partial_completion:
                    view_ = StatusCode::PageFault.view_;
                    break;
                default:
                    view_ = StatusCode::Unknown.view_;
            }
    }
#endif
}// namespace dml::testing

#endif//DML_TESTING_STATUS_HPP
