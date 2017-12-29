#ifndef TRIAL_AWARE_NATIVE_SOCKET_HPP
#define TRIAL_AWARE_NATIVE_SOCKET_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2013 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/asio.hpp>
#include <boost/function.hpp>

namespace trial
{
namespace aware
{

template <typename Service>
class non_closing_service : public Service
{
public:
    non_closing_service(boost::asio::io_service& io)
        : Service(io)
    {}

    void destroy(typename Service::implementation_type&)
    {
        // Do not close the file descriptor as we have no ownership of it
    }
};

//! @brief Boost.Asio wrapper for a native socket
class native_socket
{
    using service_type = non_closing_service<boost::asio::posix::stream_descriptor_service>;
    using socket_type = boost::asio::posix::basic_stream_descriptor<service_type>;

public:
    using native_handle_type = socket_type::native_handle_type;

    native_socket(boost::asio::io_service&, native_handle_type);
    ~native_socket();

    template <typename Handler>
    void async_read_event(Handler&& handler)
    {
        socket.async_read_some(boost::asio::null_buffers(), std::forward<Handler>(handler));
    }

    template <typename Handler>
    void async_write_event(Handler&& handler)
    {
        socket.async_write_some(boost::asio::null_buffers(), std::forward<Handler>(handler));
    }

    native_handle_type native_handle();

private:
    socket_type socket;
};

} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_NATIVE_SOCKET_HPP
