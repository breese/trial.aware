///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <avahi-common/error.h>
#include <trial/aware/error.hpp>
#include "dnssd/avahi/error.hpp"

namespace
{

class avahi_category
    : public boost::system::error_category
{
    virtual const char *name() const noexcept override
    {
        return "avahi";
    }

    virtual std::string message(int value) const override
    {
        return avahi_strerror(value);
    }

    virtual bool equivalent(int value,
                            const boost::system::error_condition& condition) const noexcept override
    {
        using namespace boost::system;
        using namespace trial;

        switch (value)
        {
        case AVAHI_OK:
            return bool(condition);

        case AVAHI_ERR_NO_MEMORY:
            return condition == errc::make_error_condition(errc::not_enough_memory);

        case AVAHI_ERR_NO_DAEMON:
            return condition == aware::make_error_code(aware::daemon_unavailable);

        default:
            return false;
        }
    }
};

} // anonymous namespace

namespace trial
{
namespace aware
{
namespace avahi
{

const boost::system::error_category& category()
{
    static avahi_category instance;
    return instance;
}

boost::system::error_code make_error_code(int value)
{
    return boost::system::error_code(value, avahi::category());
}

} // namespace avahi
} // namespace aware
} // namespace trial
