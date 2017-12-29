///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <trial/aware/contact.hpp>
#include <trial/aware/dnssd/factory.hpp>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <type> <name>" << std::endl;
        return 1;
    }

    trial::aware::contact::properties_type properties;
    properties["key"] = "value";
    auto contact = trial::aware::contact(argv[1])
        .name(argv[2])
        .address(boost::asio::ip::address())
        .port(3834)
        .properties(properties);

    boost::asio::io_service io;
    trial::aware::dnssd::factory factory;
    auto announcer = factory.make_announce(io);
    announcer->async_announce(
        contact,
        [&contact] (const boost::system::error_code& error)
        {
            std::cout << "Announced: " << error.message() << std::endl;
            if (!error)
            {
                std::cout << contact.type() << " = " << contact.name() << std::endl;
            }
        });
    io.run();
    return 0;
}
