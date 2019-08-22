///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include "native_socket.hpp"

namespace trial
{
namespace aware
{

native_socket::native_socket(const trial::net::executor& executor,
                             native_handle_type handle)
    : socket(executor, handle)
{
}

native_socket::~native_socket()
{
  // Close the socket to make sure all asynchronous requests are cancelled.
  if (socket.is_open())
  {
    boost::system::error_code dummy; // Ignore errors
    socket.close(dummy);
    socket.release();
  }
}

native_socket::native_handle_type native_socket::native_handle()
{
    return socket.native_handle();
}

} // namespace aware
} // namespace trial
