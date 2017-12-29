#ifndef TRIAL_AWARE_AVAHI_POLLER_HPP
#define TRIAL_AWARE_AVAHI_POLLER_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <avahi-common/watch.h>
#include <boost/asio/io_service.hpp>

namespace trial
{
namespace aware
{
namespace avahi
{

class poller
    : public ::AvahiPoll
{
public:
    poller(boost::asio::io_service&);

    boost::asio::io_service& get_io_service() const;

private:
    boost::asio::io_service& io;
};

} // namespace avahi
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_AVAHI_POLLER_HPP
