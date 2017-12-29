#ifndef TRIAL_AWARE_MDNS_MONITOR_SOCKET_HPP
#define TRIAL_AWARE_MDNS_MONITOR_SOCKET_HPP

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
#include <map>
#include <boost/system/error_code.hpp>
#include <boost/asio/io_service.hpp>
#include <trial/aware/monitor_socket.hpp>
#include "native_socket.hpp"
#include "mdns/handle.hpp"

namespace trial
{
namespace aware
{
namespace mdns
{
class monitor;

class monitor_socket
    : public aware::monitor_socket
{
public:
    monitor_socket(boost::asio::io_service&);
    ~monitor_socket();

    virtual void async_monitor(aware::contact&,
                               async_monitor_handler) override;

private:
    void process_read_event(const boost::system::error_code&,
                            std::size_t);

    void invoke(const boost::system::error_code&,
                async_monitor_handler);

private:
    boost::asio::io_service& io;
    mdns::handle connection;
    aware::native_socket socket;
    bool waiting;
    boost::system::error_code permanent_error;
    std::map< std::string, std::shared_ptr<mdns::monitor> > monitors;
};

} // namespace mdns
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_MDNS_MONITOR_SOCKET_HPP
