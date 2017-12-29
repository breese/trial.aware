#ifndef TRIAL_AWARE_AVAHI_ANNOUNCER_HPP
#define TRIAL_AWARE_AVAHI_ANNOUNCER_HPP

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
#include <map>
#include <functional>
#include <boost/system/error_code.hpp>
#include <trial/aware/contact.hpp>

struct AvahiEntryGroup;

namespace trial
{
namespace aware
{
namespace avahi
{
class client;

// RAII wrapper for avahi_entry_group functionality
class announcer
{
public:
    using property_map = std::map<std::string, std::string>;
    using async_announce_handler = std::function<void (const boost::system::error_code&)>;

    announcer(const avahi::client&);
    ~announcer();

    void async_announce(aware::contact&,
                        async_announce_handler);

private:
    void commit(AvahiEntryGroup *);

private:
    struct wrapper;

    AvahiEntryGroup *ptr;
    async_announce_handler handler;
};

} // namespace avahi
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_AVAHI_ANNOUNCER_HPP
