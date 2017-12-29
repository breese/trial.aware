#ifndef TRIAL_AWARE_MDNS_RESOLVER_HPP
#define TRIAL_AWARE_MDNS_RESOLVER_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/system/error_code.hpp>
#include <trial/aware/contact.hpp>
#include "mdns/handle.hpp"

namespace trial
{
namespace aware
{
namespace mdns
{

class resolver
{
public:
    class listener
    {
    public:
        virtual ~listener() = default;

        virtual void on_resolver_done(const aware::contact& contact) = 0;
        virtual void on_resolver_failure(const boost::system::error_code&) = 0;
    };

    resolver(mdns::handle&,
             const aware::contact& contact,
             resolver::listener&);

private:
    void on_resolved(const char *host);
    void on_addrinfo(bool more);

private:
    struct callback;

    mdns::handle& connection;
    resolver::listener& listener;
    mdns::handle handle;
    aware::contact contact;
    unsigned short port;
};

} // namespace mdns
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_MDNS_RESOLVER_HPP
