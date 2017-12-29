#ifndef TRIAL_AWARE_MDNS_MONITOR_HPP
#define TRIAL_AWARE_MDNS_MONITOR_HPP

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
#include <queue>
#include <set>
#include <map>
#include <chrono>
#include <boost/optional.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/basic_waitable_timer.hpp>
#include <trial/aware/contact.hpp>
#include <trial/aware/monitor_socket.hpp>
#include "mdns/handle.hpp"
#include "mdns/browser.hpp"
#include "mdns/resolver.hpp"

namespace trial
{
namespace aware
{
namespace mdns
{

// mDNSResponder periodically (about once per minute) reports false positives
// where announcements are reported as disappeared only to be reported as
// appeared again shortly after (a second or two.) In reality they have never
// disappeared. Consequently, action on disappear events is defered for a
// couple of seconds.
//
// When mDNSResponder reports false disappearances they are either reported on
// their actual network interface or on the "any" network interface. In the
// latter case, we cannot know exactly which network interface to ignore, so
// we keep track of the amount of disappearances as well as the re-appeared
// annoucements.

class monitor
    : public browser::listener,
      public resolver::listener
{
public:
    using async_monitor_handler = typename aware::monitor_socket::async_monitor_handler;

    monitor(boost::asio::io_service&, mdns::handle&);

    void listen(aware::contact&, async_monitor_handler);

private:
    // browser::listener
    virtual void on_browser_appear(const aware::contact&, bool) override;
    virtual void on_browser_disappear(const aware::contact&, bool) override;
    virtual void on_browser_failure(const boost::system::error_code&) override;
    // resolver::listener
    virtual void on_resolver_done(const aware::contact&) override;
    virtual void on_resolver_failure(const boost::system::error_code&) override;

private:
    boost::asio::io_service& io;
    mdns::handle& connection;
    std::string type;
    boost::optional<mdns::browser> browser;
    boost::system::error_code permanent_error;

    struct request
    {
        request(aware::contact&, async_monitor_handler);

        aware::contact& contact;
        async_monitor_handler handler;
    };
    std::queue<request> requests;

    class scope
    {
    public:
        using active_container = std::set<aware::contact>;
        using addition_container = std::set<aware::contact>;
        using removal_container = std::multiset<aware::contact>;

        scope(boost::asio::io_service&, monitor&);
        ~scope();

        void submit_appear(const aware::contact&);
        void commit_appear();
        void submit_disappear(const aware::contact&);
        void commit_disappear();
        void resolved(const aware::contact&);

        void activate(const aware::contact&);
        void deactivate(const aware::contact&);

    private:
        scope(const scope&) = delete;
        scope(scope&&) = delete;
        scope& operator= (const scope&) = delete;
        scope& operator= (scope&&) = delete;

        void process_disappear(const boost::system::error_code&);
        void execute_appear();
        void execute_disappear();

    private:
        monitor& self;
        boost::asio::basic_waitable_timer<std::chrono::steady_clock> timer;
        active_container active;
        addition_container additions;
        removal_container removals;
        std::size_t wildcard_count;
    };
    // Key is name (type is implicit)
    std::map< std::string, std::shared_ptr<scope> > scopes;

    using name_container = std::set<std::string>;
    name_container uncommitted_appear;
    name_container uncommitted_disappear;

    std::map< aware::contact, std::shared_ptr<mdns::resolver> > resolvers;
};

} // namespace mdns
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_MDNS_MONITOR_HPP
