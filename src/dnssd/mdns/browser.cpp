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
#include <trial/aware/contact.hpp>
#include "dnssd/mdns/dns_sd.hpp"
#include "dnssd/mdns/utility.hpp"
#include "dnssd/mdns/error.hpp"
#include "dnssd/mdns/throw_on_error.hpp"
#include "dnssd/mdns/browser.hpp"

namespace trial
{
namespace aware
{
namespace mdns
{

//-----------------------------------------------------------------------------
// Callback
//-----------------------------------------------------------------------------

struct browser::callback
{
    static void on_browsed(::DNSServiceRef,
                           ::DNSServiceFlags flags,
                           uint32_t interface_index,
                           ::DNSServiceErrorType error,
                           const char *name,
                           const char *regtype,
                           const char *domain,
                           void *context)
    {
        try
        {
            auto self = static_cast<browser *>(context);
            assert(self);

            if (error == kDNSServiceErr_NoError)
            {
                auto contact = aware::contact(mdns::type_decode(regtype))
                    .name(name)
                    .domain(domain)
                    .index(to_index(interface_index));
                const bool commit = !(flags & kDNSServiceFlagsMoreComing);
                if (flags & kDNSServiceFlagsAdd)
                {
                    self->member.listener.on_browser_appear(contact, commit);
                }
                else
                {
                    self->member.listener.on_browser_disappear(contact, commit);
                }
            }
            else
            {
                self->member.listener.on_browser_failure(mdns::make_error_code(error));
            }
        }
        catch (...)
        {
            // Ignore unknown exceptions
        }
    }
};

//-----------------------------------------------------------------------------
// browser
//-----------------------------------------------------------------------------

browser::browser(const std::string& type,
                 mdns::handle& connection,
                 typename browser::listener& listener)
    : member{ connection, listener, {} }
{
    const ::DNSServiceFlags flags = kDNSServiceFlagsShareConnection;
    std::string regtype = mdns::type_encode(type);
    const uint32_t interfaceIndex = kDNSServiceInterfaceIndexAny;
    const char *domain = 0; // Use .local

    auto ref = connection.get<DNSServiceRef>();
    ::DNSServiceErrorType error = ::DNSServiceBrowse(
        &ref,
        flags,
        interfaceIndex,
        regtype.c_str(),
        domain,
        &browser::callback::on_browsed,
        this);
    throw_on_error(error);

    member.handle.reset(ref);
}

} // namespace mdns
} // namespace aware
} // namespace trial
