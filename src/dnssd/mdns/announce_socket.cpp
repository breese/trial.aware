///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <functional>
#include <boost/system/system_error.hpp>
#include "dnssd/mdns/dns_sd.hpp"
#include "dnssd/mdns/throw_on_error.hpp"
#include "dnssd/mdns/announcer.hpp"
#include "dnssd/mdns/announce_socket.hpp"

namespace trial
{
namespace aware
{
namespace mdns
{

announce_socket::announce_socket(const net::executor& executor)
    : executor(executor),
      connection(mdns::handle::with_connection),
      socket(executor, connection.native_handle()),
      waiting(false)
{
}

void announce_socket::async_announce(aware::contact& contact,
                                     async_announce_handler handler)
{
    if (connection.empty())
    {
        using namespace boost::system;
        auto irrecoverable = errc::make_error_code(errc::state_not_recoverable);
        net::post(
            executor,
            std::bind(&announce_socket::invoke,
                      this,
                      irrecoverable,
                      handler));
        return;
    }

    try
    {
        const auto& key = contact.type();
        auto where = announcers.lower_bound(key);
        if ((where == announcers.end()) || (announcers.key_comp()(key, where->first)))
        {
            where = announcers.insert(
                where,
                std::make_pair(key,
                               std::make_shared<mdns::announcer>(std::ref(connection))));
        }
        assert(where != announcers.end());
        where->second->announce(contact, handler);

        if (!waiting)
        {
            socket.async_read_event(std::bind(&announce_socket::process_read_event,
                                              this,
                                              std::placeholders::_1,
                                              std::placeholders::_2));
            waiting = true;
        }
    }
    catch (const boost::system::system_error& ex)
    {
        net::post(
            executor,
            std::bind(&announce_socket::invoke,
                      this,
                      ex.code(),
                      handler));
    }
    // Other exceptions are propagated outwards
}

void announce_socket::process_read_event(const boost::system::error_code& error,
                                         std::size_t)
{
    waiting = false;
    if (!error)
    {
        // Execute the register callbacks
        int status = ::DNSServiceProcessResult(connection.get<DNSServiceRef>());
        if (status == kDNSServiceErr_NoError)
        {
            socket.async_read_event(std::bind(&announce_socket::process_read_event,
                                              this,
                                              std::placeholders::_1,
                                              std::placeholders::_2));
            waiting = true;
        }
        else
        {
            // FIXME: How to report error?
        }
    }
    else if (error == boost::asio::error::operation_aborted)
    {
        // Ignore
    }
    else
    {
        // FIXME: How to report error?
    }
}

void announce_socket::invoke(const boost::system::error_code& error,
                             async_announce_handler handler)
{
    handler(error);
}

} // namespace mdns
} // namespace aware
} // namespace trial
