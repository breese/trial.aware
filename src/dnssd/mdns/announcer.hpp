#ifndef TRIAL_AWARE_MDNS_ANNOUNCER_HPP
#define TRIAL_AWARE_MDNS_ANNOUNCER_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <memory>
#include <trial/aware/contact.hpp>
#include <trial/aware/announce_socket.hpp>
#include "dnssd/mdns/handle.hpp"
#include "dnssd/mdns/properties.hpp"

namespace trial
{
namespace aware
{
namespace mdns
{

// Wrapper for DNSServiceRegister
class announcer
{
public:
    using error_type = int;
    using async_announce_handler = typename aware::announce_socket::async_announce_handler;

    // Instantiated per announce type
    announcer(mdns::handle&);

    // Called per announce request
    void announce(aware::contact&, async_announce_handler);

private:
    struct callback;

    mdns::handle& connection;
    mdns::handle handle;
    struct current_type
    {
        current_type(aware::contact&, async_announce_handler);
        current_type(const current_type&);

        aware::contact& contact;
        std::shared_ptr<mdns::properties> properties;
        async_announce_handler handler;
    };
    std::unique_ptr<current_type> current;
};

} // namespace mdns
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_MDNS_ANNOUNCER_HPP
