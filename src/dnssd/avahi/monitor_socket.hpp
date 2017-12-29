#ifndef TRIAL_AWARE_AVAHI_MONITOR_SOCKET_HPP
#define TRIAL_AWARE_AVAHI_MONITOR_SOCKET_HPP

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
#include <string>
#include <map>
#include <boost/asio/basic_io_object.hpp>
#include <trial/aware/monitor_socket.hpp>
#include "avahi/service.hpp"

namespace trial
{
namespace aware
{
namespace avahi
{
namespace detail { class monitor; }

class monitor_socket
    : public aware::monitor_socket,
      public boost::asio::basic_io_object<avahi::service>
{
    using monitor_ptr = std::shared_ptr<avahi::detail::monitor>;
    using monitor_map = std::map<std::string, monitor_ptr>;

public:
    monitor_socket(boost::asio::io_service&);

    virtual void async_monitor(aware::contact&,
                               async_monitor_handler) override;

private:
    monitor_map monitors;
};

} // namespace avahi
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_AVAHI_MONITOR_SOCKET_HPP
