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
#include <new> // std::nothrow
#include <memory>
#include <chrono>
#include <utility>
#include <trial/net/io_context.hpp>
#include <avahi-common/timeval.h>
#include "native_socket.hpp"
#include "dnssd/avahi/poller.hpp"

using namespace trial::aware;

//-----------------------------------------------------------------------------
// AvahiWatch
//-----------------------------------------------------------------------------

// This struct must be in global namespace
struct AvahiWatch
{
    AvahiWatch(const trial::net::executor& executor,
               int fd,
               AvahiWatchEvent event,
               AvahiWatchCallback callback,
               void *userdata)
        : socket(executor, fd),
          callback(callback),
          userdata(userdata),
          revents(AvahiWatchEvent(0))
    {
        if (event & AVAHI_WATCH_IN)
        {
            start_read();
        }
        if (event & AVAHI_WATCH_OUT)
        {
            start_write();
        }
    }

    void update(AvahiWatchEvent /* event */)
    {
        // FIXME
        assert(false);
    }

    AvahiWatchEvent get_events() const
    {
        return revents;
    }

    void start_read()
    {
        revents = (AvahiWatchEvent)(revents & ~AVAHI_WATCH_IN);
        socket.async_read_event(std::bind(&AvahiWatch::process_read,
                                          this,
                                          std::placeholders::_1,
                                          std::placeholders::_2));
    }

    void process_read(const boost::system::error_code& error,
                      std::size_t /* bytes_transferred */)
    {
        if (!error)
        {
            revents = (AvahiWatchEvent)(revents | AVAHI_WATCH_IN);
            callback(this, socket.native_handle(), AVAHI_WATCH_IN, userdata);
            start_read();
        }
        else if (error == boost::asio::error::operation_aborted)
        {
            // We are closing down
        }
        else
        {
            // FIXME
        }
    }

    void start_write()
    {
        revents = (AvahiWatchEvent)(revents & ~AVAHI_WATCH_OUT);
        socket.async_write_event(std::bind(&AvahiWatch::process_write,
                                           this,
                                           std::placeholders::_1,
                                           std::placeholders::_2));
    }

    void process_write(const boost::system::error_code& error,
                       std::size_t /* bytes_transferred */)
    {
        if (!error)
        {
            revents = (AvahiWatchEvent)(revents | AVAHI_WATCH_OUT);
            callback(this, socket.native_handle(), AVAHI_WATCH_OUT, userdata);
            start_write();
        }
        else if (error == boost::asio::error::operation_aborted)
        {
            // We are closing down
        }
        else
        {
            // FIXME
        }
    }

    native_socket socket;
    AvahiWatchCallback callback;
    void *userdata;
    AvahiWatchEvent revents;
};

extern "C"
AvahiWatch *aware_avahi_watch_new(const AvahiPoll *poll,
                                  int fd,
                                  AvahiWatchEvent event,
                                  AvahiWatchCallback callback,
                                  void *userdata)
{
    auto poller = reinterpret_cast<const avahi::poller *>(poll);
    return new (std::nothrow) AvahiWatch(poller->get_executor(), fd, event, callback, userdata);
}

extern "C"
void aware_avahi_watch_free(AvahiWatch *self)
{
    delete self;
}

extern "C"
void aware_avahi_watch_update(AvahiWatch *self,
                              AvahiWatchEvent event)
{
    self->update(event);
}

extern "C"
AvahiWatchEvent aware_avahi_watch_get_events(AvahiWatch *self)
{
    return self->get_events();
}

//-----------------------------------------------------------------------------
// AvahiTimeout
//-----------------------------------------------------------------------------

class avahi_timer;

// This struct must be in global namespace
struct AvahiTimeout
{
    explicit AvahiTimeout(std::shared_ptr<avahi_timer>);
    void update(const struct timeval *);
    void release();

    // AvahiTimeout may be deleted before the timer callback is invoked, so the
    // timer is kept as a shared_ptr, for which this class acts as a simple
    // wrapper.
    std::shared_ptr<avahi_timer> timer;
};

class avahi_timer : public std::enable_shared_from_this<avahi_timer>
{
public:
    static std::shared_ptr<avahi_timer> create(const trial::net::executor& executor,
                                               const struct timeval *tv,
                                               AvahiTimeoutCallback callback,
                                               void *userdata)
    {
        auto result = std::make_shared<avahi_timer>(executor, callback, userdata);
        if (tv)
        {
            result->update(tv);
        }
        result->set_resource(new (std::nothrow) AvahiTimeout(result));
        return result;
    }

    avahi_timer(const trial::net::executor& executor,
                AvahiTimeoutCallback callback,
                void *userdata)
        : resource(nullptr),
          timer(executor),
          callback(callback),
          userdata(userdata)
    {
    }

    avahi_timer(const avahi_timer&) = delete;
    avahi_timer(avahi_timer&&) = delete;
    avahi_timer& operator= (const avahi_timer&) = delete;
    avahi_timer& operator= (avahi_timer&&) = delete;

    ~avahi_timer()
    {
        delete resource;
    }

    AvahiTimeout *get_resource() const
    {
        assert(resource);
        return resource;
    }

    void set_resource(AvahiTimeout *res)
    {
        resource = res;
    }

    void cancel()
    {
        boost::system::error_code dummy;
        timer.cancel(dummy);
    }

    void update(const struct timeval *tv)
    {
        if (tv)
        {
            const AvahiUsec deadline = ((tv->tv_sec == 0) && (tv->tv_usec == 0)) ? 0 : -avahi_age(tv);
            boost::system::error_code dummy;
            timer.expires_from_now(std::chrono::microseconds(deadline), dummy);
            timer.async_wait(std::bind(&avahi_timer::process_timeout,
                                       shared_from_this(),
                                       std::placeholders::_1));
        }
        else
        {
            cancel();
        }
    }

    void process_timeout(const boost::system::error_code& error)
    {
        if (!error)
        {
            callback(resource, userdata);
        }
        else if (error == boost::asio::error::operation_aborted)
        {
            // We are closing down
        }
        else
        {
            // FIXME
        }
    }

    AvahiTimeout *resource;
    boost::asio::basic_waitable_timer<std::chrono::steady_clock> timer;
    AvahiTimeoutCallback callback;
    void *userdata;
};

AvahiTimeout::AvahiTimeout(std::shared_ptr<avahi_timer> timer)
    : timer(std::move(timer))
{
}

void AvahiTimeout::update(const struct timeval *tv)
{
    timer->update(tv);
}

void AvahiTimeout::release()
{
    timer->cancel();
    timer.reset();
}

extern "C"
AvahiTimeout *aware_avahi_timeout_new(const AvahiPoll *poll,
                                      const struct timeval *tv,
                                      AvahiTimeoutCallback callback,
                                      void *userdata)
{
    auto poller = reinterpret_cast<const avahi::poller *>(poll);
    auto timer = avahi_timer::create(poller->get_executor(), tv, callback, userdata);
    return timer->get_resource();
}

extern "C"
void aware_avahi_timeout_free(AvahiTimeout *self)
{
    self->release();
}

extern "C"
void aware_avahi_timeout_update(AvahiTimeout *self,
                          const struct timeval *tv)
{
    self->update(tv);
}

//-----------------------------------------------------------------------------
// poller
//-----------------------------------------------------------------------------

namespace trial
{
namespace aware
{
namespace avahi
{

poller::poller(const trial::net::executor& executor)
    : executor(executor)
{
    userdata = this;
    watch_new = &aware_avahi_watch_new;
    watch_update = &aware_avahi_watch_update;
    watch_get_events = &aware_avahi_watch_get_events;
    watch_free = &aware_avahi_watch_free;
    timeout_new = &aware_avahi_timeout_new;
    timeout_update = &aware_avahi_timeout_update;
    timeout_free = &aware_avahi_timeout_free;
}

trial::net::executor poller::get_executor() const
{
    return executor;
}

} // namespace avahi
} // namespace aware
} // namespace trial
