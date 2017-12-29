///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <trial/aware/error.hpp>

namespace
{

class aware_category
    : public boost::system::error_category
{
    virtual const char *name() const noexcept override
    {
        return "trial::aware";
    }

    virtual std::string message(int value) const override
    {
        using namespace trial::aware;
        switch (errc(value))
        {
        case success:
            return "success";
        }
        return "trial::aware error";
    }
};

} // anonymous namespace

namespace trial
{
namespace aware
{

const boost::system::error_category& category()
{
    static aware_category instance;
    return instance;
}

boost::system::error_code make_error_code(aware::errc value)
{
    return boost::system::error_code(value, aware::category());
}

boost::system::error_condition make_error_condition(aware::errc value)
{
    return boost::system::error_condition(value, aware::category());
}

} // namespace aware
} // namespace trial
