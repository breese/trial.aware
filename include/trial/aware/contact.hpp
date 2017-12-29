#ifndef TRIAL_AWARE_CONTACT_HPP
#define TRIAL_AWARE_CONTACT_HPP

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
#include <map>
#include <boost/asio/ip/address.hpp>

namespace trial
{
namespace aware
{

class contact
{
public:
    using index_type = int;
    using address_type = boost::asio::ip::address;
    using port_type = unsigned short;
    using properties_type = std::map<std::string, std::string>;

    contact(std::string type);
    contact(const contact&) = default;
    contact(contact&&) = default;
    contact& operator= (const contact&) = default;
    contact& operator= (contact&&) = default;

    // Setters
    contact& name(std::string);
    contact& domain(std::string);
    contact& index(index_type);
    contact& address(address_type);
    contact& port(port_type);
    contact& properties(properties_type);

    // Getters
    bool empty() const;
    const std::string& type() const & { return data.type; }
    const std::string& name() const & { return data.name; }
    const std::string& domain() const & { return data.domain; }
    index_type index() const & { return data.index; }
    const address_type& address() const & { return data.address; }
    port_type port() const & { return data.port; }
    const properties_type& properties() const & { return data.properties; }

    bool operator== (const contact&) const;
    bool operator< (const contact&) const;

public:
    static constexpr index_type wildcard = -1;

private:
    struct
    {
        std::string type;
        std::string name;
        std::string domain;
        address_type address;
        properties_type properties;
        index_type index;
        port_type port;
    } data;
};

} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_CONTACT_HPP
