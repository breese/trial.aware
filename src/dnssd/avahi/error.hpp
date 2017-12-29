#ifndef TRIAL_AWARE_AVAHI_ERROR_HPP
#define TRIAL_AWARE_AVAHI_ERROR_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/system/error_code.hpp>

namespace trial
{
namespace aware
{
namespace avahi
{

const boost::system::error_category& category();
boost::system::error_code make_error_code(int);

} // namespace avahi
} // namespace aware
} // namespace trial


#endif // TRIAL_AWARE_AVAHI_ERROR_HPP
