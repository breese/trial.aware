///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/system/system_error.hpp>
#include "dnssd/mdns/error.hpp"
#include "dnssd/mdns/throw_on_error.hpp"

namespace trial
{
namespace aware
{
namespace mdns
{

void throw_on_error(int error)
{
    if (error)
        throw boost::system::system_error(mdns::make_error_code(error));
}

} // namespace mdns
} // namespace aware
} // namespace trial
