#ifndef TRIAL_AWARE_DNSSD_FACTORY_HPP
#define TRIAL_AWARE_DNSSD_FACTORY_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <trial/aware/factory.hpp>

namespace trial
{
namespace aware
{
namespace dnssd
{

// DNS Service Discovery

class factory
    : public aware::factory
{
public:
    virtual std::unique_ptr<aware::announce_socket> make_announce(const net::executor&) override;
    virtual std::unique_ptr<aware::monitor_socket> make_monitor(const net::executor&) override;
};

} // namespace dnssd
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_DNSSD_FACTORY_HPP
