///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/asio/ip/address.hpp>
#include <boost/asio/detail/socket_ops.hpp> // network_to_host_short
#include "dnssd/mdns/dns_sd.hpp"
#include "dnssd/mdns/utility.hpp"
#include "dnssd/mdns/error.hpp"
#include "dnssd/mdns/throw_on_error.hpp"
#include "dnssd/mdns/resolver.hpp"

namespace trial
{
namespace aware
{
namespace mdns
{

boost::asio::ip::address to_address(const sockaddr& addr)
{
    using namespace boost::asio;
    switch (addr.sa_family)
    {
    case AF_INET:
        return ip::address_v4(reinterpret_cast<const ip::address_v4::bytes_type&>(reinterpret_cast<const sockaddr_in&>(addr).sin_addr));

    case AF_INET6:
        {
            auto& addr6 = reinterpret_cast<const sockaddr_in6&>(addr);
            return ip::address_v6(reinterpret_cast<const ip::address_v6::bytes_type&>(addr6.sin6_addr),
                                  addr6.sin6_scope_id);
        }

    default:
        assert(false);
        break;
    }
}

//-----------------------------------------------------------------------------
// Callback
//-----------------------------------------------------------------------------

struct resolver::callback
{
    static void on_resolved(::DNSServiceRef,
                            ::DNSServiceFlags,
                            uint32_t interface_index,
                            ::DNSServiceErrorType error,
                            const char * /* fullname */, // name._type._tcp.local
                            const char *host, // host.local
                            uint16_t port,
                            uint16_t txt_length,
                            const unsigned char *txt_record,
                            void *context)
    {
        auto self = static_cast<resolver *>(context);
        assert(self);

        try
        {
            if (error == kDNSServiceErr_NoError)
            {
                // Properties
                aware::contact::properties_type properties;
                const uint16_t keyLength = 256; // See dns_sd.h
                char key[keyLength];
                uint8_t valueLength;
                const void *value;
                uint16_t count = ::TXTRecordGetCount(txt_length, txt_record);
                for (uint16_t current = 0; current < count; ++current)
                {
                    if (::TXTRecordGetItemAtIndex(txt_length, txt_record,
                                                  current,
                                                  keyLength, key,
                                                  &valueLength, &value) == kDNSServiceErr_NoError)
                    {
                        properties[key] = std::string(static_cast<const char *>(value),
                                                      valueLength);
                    }
                }

                self->member.contact.index(to_index(interface_index));
                self->member.contact.properties(properties);
                // FIXME: Set port directly on contact?
                self->member.port = boost::asio::detail::socket_ops::network_to_host_short(port);
                self->on_resolved(host);
            }
            else
            {
                self->member.listener.on_resolver_failure(mdns::make_error_code(error));
            }
        }
        catch (const boost::system::system_error& ex)
        {
            self->member.listener.on_resolver_failure(ex.code());
        }
        catch (...)
        {
            // Ignore unknown exceptions
        }
    }

    static void on_addrinfo(::DNSServiceRef,
                            ::DNSServiceFlags flags,
                            uint32_t /* interface_index */,
                            ::DNSServiceErrorType error,
                            const char * /* host */,
                            const struct sockaddr *address,
                            uint32_t /* ttl */,
                            void *context)
    {
        auto self = static_cast<resolver *>(context);
        assert(self);

        try
        {
            if (error == kDNSServiceErr_NoError)
            {
                const bool more = flags & kDNSServiceFlagsMoreComing;
                self->member.contact.address(mdns::to_address(*address));
                self->member.contact.port(self->member.port);
                self->on_addrinfo(more);
            }
            else
            {
                self->member.listener.on_resolver_failure(mdns::make_error_code(error));
            }
        }
        catch (...)
        {
            // Ignore unknown exceptions
        }
    }
};

//-----------------------------------------------------------------------------
// resolver
//-----------------------------------------------------------------------------

resolver::resolver(mdns::handle& connection,
                   const aware::contact& contact,
                   typename resolver::listener& listener)
    : member{ connection, listener, {}, contact, 0 }
{
    const ::DNSServiceFlags flags = kDNSServiceFlagsShareConnection;
    auto regtype = mdns::type_encode(contact.type());

    auto ref = connection.get<DNSServiceRef>();
    ::DNSServiceErrorType error = ::DNSServiceResolve(&ref,
                                                      flags,
                                                      from_index(contact.index()),
                                                      contact.name().c_str(),
                                                      regtype.c_str(),
                                                      contact.domain().c_str(),
                                                      &resolver::callback::on_resolved,
                                                      this);
    throw_on_error(error);

    member.handle.reset(ref);
}

void resolver::on_resolved(const char *host)
{
    const ::DNSServiceFlags flags = kDNSServiceFlagsShareConnection;
    const ::DNSServiceProtocol protocol = kDNSServiceProtocol_IPv4 | kDNSServiceProtocol_IPv6;

    auto ref = member.connection.get<DNSServiceRef>();
    ::DNSServiceErrorType error = ::DNSServiceGetAddrInfo(&ref,
                                                          flags,
                                                          from_index(member.contact.index()),
                                                          protocol,
                                                          host,
                                                          &resolver::callback::on_addrinfo,
                                                          this);
    throw_on_error(error);

    member.handle.reset(ref);
}

void resolver::on_addrinfo(bool more)
{
    if (!more)
    {
        member.handle.reset();
    }
    member.listener.on_resolver_done(member.contact);
}

} // namespace mdns
} // namespace aware
} // namespace trial
