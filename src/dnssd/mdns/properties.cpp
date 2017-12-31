///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include "dnssd/mdns/dns_sd.hpp"
#include "dnssd/mdns/throw_on_error.hpp"
#include "dnssd/mdns/properties.hpp"

namespace trial
{
namespace aware
{
namespace mdns
{

properties::properties(const map_type& input)
{
    static_assert(sizeof(decltype(record)) >= sizeof(::TXTRecordRef),
                  "Increase properties::record_size");

    ::TXTRecordCreate(reinterpret_cast<TXTRecordRef*>(std::addressof(record)), 0, 0);
    for (const auto& property : input)
    {
        ::DNSServiceErrorType error = ::TXTRecordSetValue(reinterpret_cast<TXTRecordRef*>(std::addressof(record)),
                                                          property.first.data(),
                                                          property.second.size(),
                                                          property.second.data());
        throw_on_error(error);
    }
}

properties::~properties()
{
    ::TXTRecordDeallocate(reinterpret_cast<TXTRecordRef*>(std::addressof(record)));
}

auto properties::size() const -> size_type
{
    return ::TXTRecordGetLength(reinterpret_cast<const TXTRecordRef*>(std::addressof(record)));
}

const void *properties::data() const
{
    return ::TXTRecordGetBytesPtr(reinterpret_cast<const TXTRecordRef*>(std::addressof(record)));
}

} // namespace mdns
} // namespace aware
} // namespace trial
