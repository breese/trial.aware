#ifndef TRIAL_AWARE_MDNS_DNS_SD_HPP
#define TRIAL_AWARE_MDNS_DNS_SD_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <dns_sd.h>
#include <trial/aware/contact.hpp>

// The Avahi Bonjour compatibility layer includes a very old dns_sd.h header.
// http://0pointer.net/blog/projects/avahi-compat.html
#if _DNS_SD_H + 0 == 0
# error "mDNSResponder is too old"
#endif

namespace trial
{
namespace aware
{
namespace mdns
{

inline uint32_t from_index(int value)
{
    switch (value)
    {
    case aware::contact::wildcard:
        return kDNSServiceInterfaceIndexAny;
    default:
        return value;
    }
}

inline int to_index(uint32_t value)
{
    switch (value)
    {
    case kDNSServiceInterfaceIndexAny:
        return aware::contact::wildcard;
    default:
        return value;
    }
}

} // namespace mdns
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_MDNS_DNS_SD_HPP
