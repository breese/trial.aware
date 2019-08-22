///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include "dnssd/avahi/announce_socket.hpp"

namespace trial
{
namespace aware
{
namespace avahi
{

announce_socket::announce_socket(const trial::net::executor& executor)
    : boost::asio::basic_io_object<avahi::service>(static_cast<net::io_context&>(executor.context())),
      announcer(boost::asio::use_service<avahi::service>(static_cast<net::io_context&>(executor.context())).client())
{
}

void announce_socket::async_announce(aware::contact& contact,
                                     async_announce_handler handler)
{
    announcer.async_announce(contact, handler);
}

} // namespace avahi
} // namespace aware
} // namespace trial
