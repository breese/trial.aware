///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <functional>
#include <utility>
#include <boost/asio.hpp>
#include <trial/aware/contact.hpp>
#include <trial/aware/monitor_socket.hpp>
#include <trial/aware/dnssd/factory.hpp>

class my_monitor
{
public:
    my_monitor(boost::asio::io_service& io,
               trial::aware::factory& factory)
        : contact(""),
          socket(factory.make_monitor(io))
    {}

    void async_monitor(const trial::aware::contact& input)
    {
        contact = input;
        socket->async_monitor(contact,
                              std::bind(&my_monitor::process_monitor,
                                        this,
                                        std::placeholders::_1,
                                        std::ref(contact)));
    }

private:
    void process_monitor(const boost::system::error_code& error,
                         trial::aware::contact& contact)
    {
        if (!error)
        {
            if (contact.empty())
            {
                std::cout << "- " << contact.index() << std::endl;
                std::cout << "  type    = " << contact.type() << std::endl;
                std::cout << "  name    = " << contact.name() << std::endl;
            }
            else
            {
                std::cout << "+ " << contact.index() << std::endl;
                std::cout << "  type    = " << contact.type() << std::endl;
                std::cout << "  name    = " << contact.name() << std::endl;
                std::cout << "  domain  = " << contact.domain() << std::endl;
                std::cout << "  address = " << contact.address().to_string() << std::endl;
                std::cout << "  port    = " << contact.port() << std::endl;
                auto properties = contact.properties();
                for (const auto& property : properties)
                {
                    std::cout << "  " << property.first << " = " << property.second << std::endl;
                }
            }
            // Launch the next monitor operation
            async_monitor(contact);
        }
        else if (error == boost::asio::error::operation_aborted)
        {
            // Ignore
        }
        else
        {
            std::cout << "Error = " << error << std::endl;
        }
    }

private:
    trial::aware::contact contact;
    std::unique_ptr<trial::aware::monitor_socket> socket;
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <type>" << std::endl;
        return 1;
    }
    boost::asio::io_service io;
    trial::aware::dnssd::factory factory;
    my_monitor monitor4(io, factory);
    my_monitor monitor6(io, factory);

    trial::aware::contact contact(argv[1]);
    monitor4.async_monitor(contact);
    monitor6.async_monitor(contact.address(boost::asio::ip::address_v6()));
    io.run();

    return 0;
}
