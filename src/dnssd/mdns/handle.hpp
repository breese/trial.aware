#ifndef TRIAL_AWARE_MDNS_HANDLE_HPP
#define TRIAL_AWARE_MDNS_HANDLE_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

namespace trial
{
namespace aware
{
namespace mdns
{

// Type-erased RAII wrapper for DNSServiceRef
class handle
{
public:
    using native_handle_type = int;
    enum with_connection_tag { with_connection };

    handle();
    handle(with_connection_tag);
    ~handle();

    template <typename T>
    T get();

    template <typename T>
    const T& get() const;

    void reset();

    template <typename T>
    void reset(const T&);

    bool empty() const;

    native_handle_type native_handle();

private:
    handle(const handle&) = delete;
    handle(handle&&) = delete;
    handle& operator= (const handle&) = delete;
    handle& operator= (handle&&) = delete;

private:
    struct opaque_type;
    opaque_type *opaque;
};

} // namespace mdns
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_MDNS_HANDLE_HPP
