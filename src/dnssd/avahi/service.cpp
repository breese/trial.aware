///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include "dnssd/avahi/service.hpp"

namespace trial
{
namespace aware
{
namespace avahi
{

trial::net::io_context::id service::id;

service::service(trial::net::io_context& io)
    : net::io_context::service(io),
      current_poller(net::extension::get_executor(io)),
      current_client(current_poller)
{
}

} // namespace avahi
} // namespace aware
} // namespace trial
