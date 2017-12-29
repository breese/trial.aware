#ifndef TRIAL_AWARE_MDNS_PROPERTIES_HPP
#define TRIAL_AWARE_MDNS_PROPERTIES_HPP

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
#include <type_traits>

namespace trial
{
namespace aware
{
namespace mdns
{

class properties
{
public:
    using size_type = std::size_t;
    using map_type = std::map<std::string, std::string>;

    properties(const map_type& input);
    ~properties();

    size_type size() const;
    const void *data() const;

private:
    static constexpr size_type record_size = 16;
    std::aligned_storage<record_size, sizeof(void *)>::type record;
};

} // namespace mdns
} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_MDNS_PROPERTIES_HPP
