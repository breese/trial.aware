#ifndef TRIAL_AWARE_MONITOR_SOCKET_HPP
#define TRIAL_AWARE_MONITOR_SOCKET_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <functional>
#include <boost/system/error_code.hpp>
#include <trial/aware/contact.hpp>

namespace trial
{
namespace aware
{

class monitor_socket
{
public:
    using async_monitor_handler = std::function<void (const boost::system::error_code&)>;

    virtual ~monitor_socket() = default;

    virtual void async_monitor(aware::contact&, async_monitor_handler) = 0;
};

} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_MONITOR_SOCKET_HPP
