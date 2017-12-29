///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <trial/aware/contact.hpp>

namespace trial
{
namespace aware
{

contact::contact(std::string value)
{
    data.type = std::move(value);
    data.index = wildcard;
}

contact& contact::name(std::string value)
{
    data.name = std::move(value);
    return *this;
}

contact& contact::domain(std::string value)
{
    data.domain = std::move(value);
    return *this;
}

contact& contact::index(index_type value)
{
    data.index = value;
    return *this;
}

contact& contact::address(address_type value)
{
    data.address = std::move(value);
    return *this;
}

contact& contact::port(port_type value)
{
    data.port = value;
    return *this;
}

contact& contact::properties(properties_type value)
{
    data.properties = std::move(value);
    return *this;
}

bool contact::empty() const
{
    return data.address.is_unspecified();
}

bool contact::operator== (const contact& other) const
{
    return (((data.index == other.data.index) ||
             (data.index == wildcard) ||
             (other.data.index == wildcard)) &&
            (data.type == other.data.type) &&
            (data.name == other.data.name) &&
            (data.domain == other.data.domain));
}

bool contact::operator< (const contact& other) const
{
    return ((data.index < other.data.index) ||
            ((data.index == other.data.index) &&
             ((data.type < other.data.type) ||
              ((data.type == other.data.type) &&
               ((data.name < other.data.name) ||
                ((data.name == other.data.name) &&
                 (data.domain < other.data.domain)))))));
}

} // namespace aware
} // namespace trial
