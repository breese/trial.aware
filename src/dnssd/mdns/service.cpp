///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include "dnssd/mdns/dns_sd.hpp"
#include "dnssd/mdns/service.hpp"

namespace trial
{
namespace aware
{
namespace mdns
{

boost::asio::io_service::id service::id;

service::service(boost::asio::io_service& io)
    : boost::asio::io_service::service(io),
      connection(mdns::handle::with_connection)
{
}

mdns::handle& service::get_connection()
{
    return connection;
}

} // namespace mdns
} // namespace aware
} // namespace trial
