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
        : socket(factory.make_monitor(io))
    {}

    void async_monitor(trial::aware::contact& contact)
    {
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
                std::cout << "Removed:" << std::endl;
                std::cout << "  type = " << contact.type() << std::endl;
                std::cout << "  name = " << contact.name() << std::endl;
            }
            else
            {
                std::cout << "Added:" << std::endl;
                std::cout << "  type = " << contact.type() << std::endl;
                std::cout << "  name = " << contact.name() << std::endl;
                std::cout << "  endpoint = " << contact.address().to_string() << ":" << contact.port() << std::endl;
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
    my_monitor monitor(io, factory);

    trial::aware::contact contact(argv[1]);
    monitor.async_monitor(contact);
    io.run();

    return 0;
}
