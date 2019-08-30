#ifndef TRIAL_AWARE_MDNS_BROWSER_HPP
#define TRIAL_AWARE_MDNS_BROWSER_HPP

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
#include <boost/system/error_code.hpp>
#include <trial/aware/contact.hpp>
#include "dnssd/mdns/handle.hpp"

namespace trial
{
namespace aware
{
namespace mdns
{

class browser
{
    using identifier_type = void *;

public:
    class listener
    {
    public:
        virtual ~listener() = default;

        virtual void on_browser_appear(const aware::contact& contact,
                                       bool more) = 0;
        virtual void on_browser_disappear(const aware::contact& contact,
                                          bool more) = 0;
        virtual void on_browser_failure(const boost::system::error_code&) = 0;
    };

    browser(const std::string& type,
            mdns::handle&,
            browser::listener&);

private:
    struct callback;
    struct
    {
        mdns::handle& connection;
        browser::listener& listener;
        mdns::handle handle;
    } member;
};

} // namespace mdns
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_MDNS_BROWSER_HPP
