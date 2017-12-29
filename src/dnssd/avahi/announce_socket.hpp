#ifndef TRIAL_AWARE_AVAHI_ANNOUNCE_SOCKET_HPP
#define TRIAL_AWARE_AVAHI_ANNOUNCE_SOCKET_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/asio/basic_io_object.hpp>
#include <trial/aware/announce_socket.hpp>
#include "avahi/service.hpp"
#include "avahi/announcer.hpp"

namespace trial
{
namespace aware
{
namespace avahi
{

class announce_socket
    : public aware::announce_socket,
      public boost::asio::basic_io_object<avahi::service>
{
public:
    announce_socket(boost::asio::io_service&);

    virtual void async_announce(aware::contact&,
                                async_announce_handler) override;

private:
    avahi::announcer announcer;
};

} // namespace avahi
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_AVAHI_ANNOUNCE_SOCKET_HPP
