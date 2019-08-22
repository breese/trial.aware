#ifndef TRIAL_AWARE_MDNS_SERVICE_HPP
#define TRIAL_AWARE_MDNS_SERVICE_HPP

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
#include <trial/net/io_context.hpp>
#include "native_socket.hpp"
#include "dnssd/mdns/handle.hpp"

namespace trial
{
namespace aware
{
namespace mdns
{

class service
    : public trial::net::io_context::service
{
public:
    static trial::net::io_context::id id;

    struct implementation_type {};

    explicit service(trial::net::io_context& io);

    void construct(implementation_type&) {}
    void destroy(implementation_type&) {}

    mdns::handle& get_connection();

private:
    virtual void shutdown_service() {}

private:
    mdns::handle connection;
    std::unique_ptr<native_socket> socket;
};

} // namespace mdns
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_MDNS_SERVICE_HPP
