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
#include <boost/asio/posix/basic_stream_descriptor.hpp>
#include <trial/net/executor.hpp>
#include <trial/net/io_context.hpp>

// Deprecated in 1.66 and removed from 1.70
#if (BOOST_VERSION < 107000) || defined(BOOST_ASIO_ENABLE_OLD_SERVICES)
# define TRIAL_AWARE_HAVE_STREAM_DESCRIPTOR_SERVICE 1
#endif

namespace trial
{
namespace aware
{

#if defined(TRIAL_AWARE_HAVE_STREAM_DESCRIPTOR_SERVICE)
template <typename Service>
class non_closing_service : public Service
{
public:
    non_closing_service(net::io_context& io)
        : Service(io)
    {}

    void destroy(typename Service::implementation_type&)
    {
        // Do not close the file descriptor as we have no ownership of it
    }
};
#endif

//! @brief Boost.Asio wrapper for a native socket
class native_socket
{
#if defined(TRIAL_AWARE_HAVE_STREAM_DESCRIPTOR_SERVICE)
    using service_type = non_closing_service<boost::asio::posix::stream_descriptor_service>;
    using socket_type = boost::asio::posix::basic_stream_descriptor<service_type>;
#else
    using socket_type = boost::asio::posix::stream_descriptor;
#endif

public:
    using native_handle_type = socket_type::native_handle_type;

    native_socket(const trial::net::executor&, native_handle_type);
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
