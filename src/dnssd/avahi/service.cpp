///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include "avahi/service.hpp"

namespace trial
{
namespace aware
{
namespace avahi
{

boost::asio::io_service::id service::id;

service::service(boost::asio::io_service& io)
    : boost::asio::io_service::service(io),
      current_poller(io),
      current_client(current_poller)
{
}

} // namespace avahi
} // namespace aware
} // namespace trial
