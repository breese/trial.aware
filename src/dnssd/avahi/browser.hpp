#ifndef TRIAL_AWARE_AVAHI_BROWSER_HPP
#define TRIAL_AWARE_AVAHI_BROWSER_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/system/error_code.hpp>
#include <trial/aware/contact.hpp>

struct AvahiServiceBrowser;

namespace trial
{
namespace aware
{
namespace avahi
{
class client;

class browser
{
public:
    class listener
    {
    public:
        virtual ~listener() {}

        virtual void on_appear(const aware::contact&) = 0;
        virtual void on_disappear(const aware::contact&) = 0;
        virtual void on_failure(const boost::system::error_code&) = 0;
    };

    browser(const avahi::client&,
            const aware::contact& contact,
            browser::listener&);
    ~browser();

private:
    using listener_type = browser::listener;
    struct wrapper;

    browser::listener& listener;
    AvahiServiceBrowser *ptr;
};

} // namespace avahi
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_AVAHI_BROWSER_HPP
