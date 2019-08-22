#ifndef TRIAL_AWARE_MDNS_ANNOUNCE_SOCKET_HPP
#define TRIAL_AWARE_MDNS_ANNOUNCE_SOCKET_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <map>
#include <memory>
#include <trial/net/io_context.hpp>
#include <trial/aware/announce_socket.hpp>
#include "native_socket.hpp"
#include "dnssd/mdns/handle.hpp"

namespace trial
{
namespace aware
{
namespace mdns
{
class announcer;

class announce_socket
    : public aware::announce_socket
{
public:
    announce_socket(const net::executor&);

    virtual void async_announce(aware::contact& contact,
                                async_announce_handler) override;

private:
    void process_read_event(const boost::system::error_code&,
                            std::size_t);

    void invoke(const boost::system::error_code&,
                async_announce_handler);

private:
    net::executor executor;
    mdns::handle connection;
    aware::native_socket socket;
    bool waiting;
    std::map< std::string, std::shared_ptr<mdns::announcer> > announcers;
};

} // namespace mdns
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_MDNS_ANNOUNCE_SOCKET_HPP
