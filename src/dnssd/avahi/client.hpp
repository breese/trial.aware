#ifndef TRIAL_AWARE_AVAHI_CLIENT_HPP
#define TRIAL_AWARE_AVAHI_CLIENT_HPP

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

struct AvahiClient;

namespace trial
{
namespace aware
{
namespace avahi
{
class poller;

// RAII wrapper of avahi_client functionality
class client
{
public:
    client(avahi::poller&);
    virtual ~client();

    operator AvahiClient *() const { return ptr; }

private:
    struct wrapper;

    void registering(AvahiClient *);
    void connecting(AvahiClient *);
    void running(AvahiClient *);
    void collision(AvahiClient *);
    void failure(AvahiClient *);

private:
    AvahiClient *ptr;
};

} // namespace avahi
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_AVAHI_CLIENT_HPP
