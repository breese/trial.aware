///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <memory>
#include <queue>
#include <functional>
#include <utility> // std::pair
#include "dnssd/avahi/browser.hpp"
#include "dnssd/avahi/monitor_socket.hpp"

namespace trial
{
namespace aware
{
namespace avahi
{

namespace detail
{

class monitor
    : public browser::listener
{
    using response_type = std::pair<boost::system::error_code, aware::contact>;
    using handler_type = aware::monitor_socket::async_monitor_handler;

public:
    monitor(const net::executor& executor,
            aware::contact& contact)
        : contact_pointer(&contact)
    {
        browser = std::make_shared<avahi::browser>(
            boost::asio::use_service<avahi::service>(static_cast<net::io_context&>(executor.context())).client(),
            contact,
            std::ref(*this));
    }

    //! @pre Must be called from the executor thread
    void prepare(aware::contact& contact, handler_type handler)
    {
        contact_pointer = &contact;
        handlers.push(std::move(handler));
        perform();
    }

    //! @pre Must be called from the executor thread
    void perform()
    {
        if (responses.empty())
            return; // Nothing to send
        if (handlers.empty())
            return; // No receiver

        const auto& response = responses.front();
        auto& handler = handlers.front();
        *contact_pointer = response.second;
        handler(response.first);
        contact_pointer = nullptr;
        responses.pop();
        handlers.pop();
    }

    virtual void on_appear(const aware::contact& contact) override final
    {
        boost::system::error_code success;
        responses.push(std::make_pair(success, contact));
        perform();
    }

    virtual void on_disappear(const aware::contact& contact) override final
    {
        boost::system::error_code success;
        responses.push(std::make_pair(success, contact));
        perform();
    }

    virtual void on_failure(const boost::system::error_code& error) override final
    {
        aware::contact no_contact("");
        responses.push(std::make_pair(error, no_contact));
        perform();
    }

private:
    aware::contact *contact_pointer;
    std::shared_ptr<avahi::browser> browser;
    std::queue<response_type> responses;
    std::queue<handler_type> handlers;
};

} // namespace detail

monitor_socket::monitor_socket(const trial::net::executor& executor)
    : boost::asio::basic_io_object<avahi::service>(static_cast<net::io_context&>(executor.context()))
{
}

void monitor_socket::async_monitor(aware::contact& contact,
                                   async_monitor_handler handler)
{
    // Perform from executor thread because the constructor of
    // detail::browser will invoke the first callback
    net::post(
        net::extension::get_executor(*this),
        [this, &contact, handler]
        {
            const auto& key = contact.type();
            auto where = monitors.lower_bound(key);
            if ((where == monitors.end()) || (monitors.key_comp()(key, where->first)))
            {
                where = monitors.insert(
                    where,
                    monitor_map::value_type(
                        key,
                        std::make_shared<avahi::detail::monitor>(
                            net::extension::get_executor(*this),
                            std::ref(contact))));
            }
            assert(where != monitors.end());
            where->second->prepare(contact, handler);
        });
}

} // namespace avahi
} // namespace aware
} // namespace trial
