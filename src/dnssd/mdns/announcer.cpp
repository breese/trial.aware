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
#include <string>
#include <memory>
#include <functional>
#include <boost/asio/placeholders.hpp>
#include <boost/utility/in_place_factory.hpp>
#include "mdns/dns_sd.hpp"
#include "mdns/error.hpp"
#include "mdns/throw_on_error.hpp"
#include "mdns/utility.hpp"
#include "mdns/properties.hpp"
#include "mdns/announcer.hpp"

namespace trial
{
namespace aware
{
namespace mdns
{

struct announcer::callback
{
    static void on_registered(::DNSServiceRef,
                              ::DNSServiceFlags flags,
                              ::DNSServiceErrorType error,
                              const char *name,
                              const char *regtype,
                              const char * /* domain */,
                              void *context)
    {
        announcer *self = static_cast<announcer *>(context);
        assert(self);

        try
        {
            if (error == kDNSServiceErr_NoError)
            {
                if (flags & kDNSServiceFlagsAdd)
                {
                    assert(self->current);
                    // Endpoint and properties have to be obtained via the monitor
                    self->current->contact = aware::contact(mdns::type_decode(regtype))
                        .name(name);
                    self->current->handler(mdns::make_error_code(kDNSServiceErr_NoError));
                    self->current = boost::none;
                }
                else
                {
                    // Name conflicts not implemented yet
                    self->current->handler(mdns::make_error_code(kDNSServiceErr_NameConflict));
                    self->current = boost::none;
                }
            }
            else
            {
                assert(self->current);
                self->current->handler(mdns::make_error_code(error));
                self->current = boost::none;
            }
        }
        catch (const boost::system::system_error& ex)
        {
            self->current->handler(ex.code());
            self->current = boost::none;
        }
        catch (...)
        {
            // Ignore unknown exceptions
        }
    }
};

announcer::announcer(mdns::handle& connection)
    : connection(connection)
{
}

void announcer::announce(aware::contact& contact,
                         async_announce_handler handler)
{
    if (current || !handle.empty())
    {
        throw_on_error(kDNSServiceErr_AlreadyRegistered);
    }

    // This operation will be completed when data is ready and process() is called
    current = boost::in_place(std::ref(contact), handler);

    const ::DNSServiceFlags flags = kDNSServiceFlagsShareConnection | kDNSServiceFlagsNoAutoRename;
    const uint32_t interface_index = from_index(contact.index());
    std::string name = contact.name();
    std::string type = mdns::type_encode(contact.type());
    const char *domain = 0; // .local
    const auto& address = contact.address();
    std::string host = address.is_unspecified()
        ? "" // Use default host name
        : address.to_string();
    const uint16_t port = boost::asio::detail::socket_ops::host_to_network_short(contact.port());

    current->properties = std::make_shared<mdns::properties>(contact.properties());

    ::DNSServiceRef ref = connection.get<DNSServiceRef>();
    // May block for seconds if service not running
    ::DNSServiceErrorType error = ::DNSServiceRegister(
        &ref,
        flags,
        interface_index,
        name.c_str(),
        type.c_str(),
        domain,
        host.empty() ? 0 : host.c_str(),
        port,
        current->properties->size(),
        current->properties->data(),
        &announcer::callback::on_registered,
        this);
    throw_on_error(error);

    handle.reset(ref);
}

announcer::current_type::current_type(aware::contact& contact,
                                      async_announce_handler handler)
    : contact(contact),
      handler(handler)
{
}

announcer::current_type::current_type(const current_type& other)
    : contact(other.contact),
      properties(other.properties),
      handler(other.handler)
{
}

} // namespace mdns
} // namespace aware
} // namespace trial
