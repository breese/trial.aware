#ifndef TRIAL_AWARE_MDNS_UTILITY_HPP
#define TRIAL_AWARE_MDNS_UTILITY_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <string>

namespace trial
{
namespace aware
{
namespace mdns
{


std::string type_encode(const std::string&);
std::string type_decode(const char *);

} // namespace mdns
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_MDNS_UTILITY_HPP
