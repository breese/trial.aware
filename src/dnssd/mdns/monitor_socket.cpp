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
#include "mdns/dns_sd.hpp"
#include "mdns/error.hpp"
#include "mdns/monitor.hpp"
#include "mdns/monitor_socket.hpp"

namespace trial
{
namespace aware
{
namespace mdns
{

monitor_socket::monitor_socket(boost::asio::io_service& io)
    : io(io),
      connection(mdns::handle::with_connection),
      socket(io, connection.native_handle()),
      waiting(false)
{
}

monitor_socket::~monitor_socket()
{
    monitors.clear();
}

void monitor_socket::async_monitor(aware::contact& contact,
                                   async_monitor_handler handler)
{
    if (connection.empty())
    {
        permanent_error = mdns::make_error_code(kDNSServiceErr_BadState);
    }
    if (permanent_error)
    {
        io.post(std::bind(&monitor_socket::invoke,
                          this,
                          permanent_error,
                          handler));
        return;
    }

    try
    {
        const auto& key = contact.type();
        auto where = monitors.lower_bound(key);
        if ((where == monitors.end()) || (monitors.key_comp()(key, where->first)))
        {
            where = monitors.insert(
                where,
                std::make_pair(key,
                               std::make_shared<mdns::monitor>(std::ref(io),
                                                               std::ref(connection))));
        }
        assert(where != monitors.end());
        where->second->listen(contact, handler);

        if (!waiting)
        {
            socket.async_read_event(std::bind(&monitor_socket::process_read_event,
                                              this,
                                              std::placeholders::_1,
                                              std::placeholders::_2));
        }
    }
    catch (const boost::system::system_error& ex)
    {
        io.post(std::bind(&monitor_socket::invoke,
                          this,
                          ex.code(),
                          handler));
    }
    // Other exceptions are propagated outwards
}

void monitor_socket::process_read_event(const boost::system::error_code& error,
                                        std::size_t)
{
    waiting = false;
    if (!error)
    {
        // Execute the register callbacks
        int status = ::DNSServiceProcessResult(connection.get<DNSServiceRef>());
        if (status == kDNSServiceErr_NoError)
        {
            socket.async_read_event(std::bind(&monitor_socket::process_read_event,
                                              this,
                                              std::placeholders::_1,
                                              std::placeholders::_2));
            waiting = true;
        }
        else
        {
            permanent_error = mdns::make_error_code(status);
        }
    }
    else if (error == boost::asio::error::operation_aborted)
    {
        // Ignore
    }
    else
    {
        permanent_error = error;
    }
}

void monitor_socket::invoke(const boost::system::error_code& error,
                            async_monitor_handler handler)
{
    handler(error);
}

} // namespace mdns
} // namespace aware
} // namespace trial
