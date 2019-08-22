///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <trial/aware/dnssd/factory.hpp>
#include "dnssd/mdns/announce_socket.hpp"
#include "dnssd/mdns/monitor_socket.hpp"

namespace trial
{
namespace aware
{
namespace dnssd
{

std::unique_ptr<aware::announce_socket> factory::make_announce(const net::executor& executor)
{
    return std::unique_ptr<mdns::announce_socket>(new mdns::announce_socket(executor));
}

std::unique_ptr<aware::monitor_socket> factory::make_monitor(const net::executor& executor)
{
    return std::unique_ptr<mdns::monitor_socket>(new mdns::monitor_socket(executor));
}

} // namespace dnssd
} // namespace aware
} // namespace trial
