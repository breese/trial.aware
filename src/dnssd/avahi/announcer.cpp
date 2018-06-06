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
#include <boost/system/system_error.hpp>
#include <avahi-common/error.h>
#include <avahi-common/strlst.h>
#include <avahi-common/alternative.h>
#include <avahi-common/malloc.h>
#include <avahi-client/publish.h>
#include <avahi-client/client.h>
#include "dnssd/avahi/utility.hpp"
#include "dnssd/avahi/error.hpp"
#include "dnssd/avahi/client.hpp"
#include "dnssd/avahi/announcer.hpp"

namespace trial
{
namespace aware
{
namespace avahi
{

//-----------------------------------------------------------------------------
// announcer::wrapper
//-----------------------------------------------------------------------------

struct announcer::wrapper
{
    static void entry_group_callback(AvahiEntryGroup *group,
                                     AvahiEntryGroupState state,
                                     void *userdata)
    {
        auto self = static_cast<avahi::announcer *>(userdata);

        switch (state)
        {
        case AVAHI_ENTRY_GROUP_ESTABLISHED:
            {
                boost::system::error_code success;
                self->handler(success);
            }
            break;

        case AVAHI_ENTRY_GROUP_COLLISION:
        case AVAHI_ENTRY_GROUP_FAILURE:
            {
                self->handler(avahi::make_error_code(avahi_client_errno(avahi_entry_group_get_client(group))));
            }
            break;

        default:
            break;
        }
    }
};

//-----------------------------------------------------------------------------
// property_list
//-----------------------------------------------------------------------------

// RAII wrapper for AvahiStringList
class property_list
{
public:
    property_list()
        : data(0)
    {
    }

    ~property_list()
    {
        avahi_string_list_free(data);
    }

    property_list& operator = (const announcer::property_map& properties)
    {
        for (const auto& property : properties)
        {
            data = avahi_string_list_add_pair(data, property.first.c_str(), property.second.c_str());
            if (data == 0)
            {
                break;
            }
        }
        return *this;
    }

    operator AvahiStringList *()
    {
        return data;
    }

private:
    AvahiStringList *data;
};

//-----------------------------------------------------------------------------
// announcer
//-----------------------------------------------------------------------------

announcer::announcer(const avahi::client& client)
    : ptr(0)
{
    ptr = avahi_entry_group_new(client,
                                wrapper::entry_group_callback,
                                this);
    if (ptr == 0)
        throw boost::system::system_error(avahi::make_error_code(AVAHI_ERR_DISCONNECTED));

    assert(avahi_entry_group_get_state(ptr) == AVAHI_ENTRY_GROUP_UNCOMMITED);
}

announcer::~announcer()
{
    if (ptr)
    {
        avahi_entry_group_free(ptr);
    }
}

void announcer::async_announce(aware::contact& contact,
                               async_announce_handler h)
{
    assert(ptr != 0);

    handler = h;

    const AvahiPublishFlags flags = AvahiPublishFlags(0);
    // Use all network interfaces
    const AvahiIfIndex interface_index = AVAHI_IF_UNSPEC;
    // Use only a specific protocol
    const AvahiProtocol protocol =
        contact.address().is_v6()
        ? AVAHI_PROTO_INET6
        : AVAHI_PROTO_INET;
    auto name = contact.name();
    auto type = avahi::type_encode(contact.type());
    // Use .local
    const char *domain = 0;
    // Host name
    const auto& address = contact.address();
    std::string host = address.is_unspecified()
        ? "" // Use default host name
        : address.to_string();

    property_list properties;
    if (!contact.properties().empty())
    {
        properties = contact.properties();
        if (properties == 0)
        {
            handler(avahi::make_error_code(AVAHI_ERR_NO_MEMORY));
            return;
        }
    }

    const int rc = avahi_entry_group_add_service_strlst(ptr,
                                                        interface_index,
                                                        protocol,
                                                        flags,
                                                        name.c_str(),
                                                        type.c_str(),
                                                        domain,
                                                        host.empty() ? 0 : host.c_str(),
                                                        contact.port(),
                                                        properties);
    if (rc != AVAHI_OK)
    {
        // Name collisions are also reported as errors
        handler(avahi::make_error_code(rc));
        return;
    }
    commit(ptr);
}

void announcer::commit(AvahiEntryGroup *group)
{
    assert(group != 0);

    if (!avahi_entry_group_is_empty(group))
    {
        int rc = avahi_entry_group_commit(group);
        if (rc != AVAHI_OK)
        {
            handler(avahi::make_error_code(rc));
        }
    }
}

} // namespace avahi
} // namespace aware
} // namespace trial
