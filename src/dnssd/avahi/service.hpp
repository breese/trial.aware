#ifndef TRIAL_AWARE_AVAHI_SERVICE_HPP
#define TRIAL_AWARE_AVAHI_SERVICE_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <trial/net/io_context.hpp>
#include "dnssd/avahi/poller.hpp"
#include "dnssd/avahi/client.hpp"

namespace trial
{
namespace aware
{
namespace avahi
{

class service
    : public trial::net::io_context::service
{
public:
    static trial::net::io_context::id id;

    struct implementation_type {};

    explicit service(trial::net::io_context&);

    void construct(implementation_type&) {}
    void destroy(implementation_type&) {}

    avahi::client& client() & { return current_client; }

private:
    virtual void shutdown_service() {}

private:
    avahi::poller current_poller;
    avahi::client current_client;
};

} // namespace avahi
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_AVAHI_SERVICE_HPP
