// nstd/net/ip/address_v6.hpp                                         -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
//                                                                       
//  Permission is hereby granted, free of charge, to any person          
//  obtaining a copy of this software and associated documentation       
//  files (the "Software"), to deal in the Software without restriction, 
//  including without limitation the rights to use, copy, modify,        
//  merge, publish, distribute, sublicense, and/or sell copies of        
//  the Software, and to permit persons to whom the Software is          
//  furnished to do so, subject to the following conditions:             
//                                                                       
//  The above copyright notice and this permission notice shall be       
//  included in all copies or substantial portions of the Software.      
//                                                                       
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES      
//  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND             
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT          
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,         
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING         
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR        
//  OTHER DEALINGS IN THE SOFTWARE. 
// ----------------------------------------------------------------------------

#ifndef INCLUDED_NSTD_NET_IP_ADDRESS_V6
#define INCLUDED_NSTD_NET_IP_ADDRESS_V6

#include "nstd/net/netfwd.hpp"
#include "nstd/net/ip/types.hpp"
#include <system_error>
#include <iosfwd>
#include <array>
#include <string>
#include <string_view>
#include <sstream>
#include <cstdint>

// ----------------------------------------------------------------------------

namespace nstd::net::ip {
    class address_v6;

    template <typename CharT, typename Traits>
    auto operator<< (::std::basic_ostream<CharT, Traits>&,
                     ::nstd::net::ip::address_v6 const&)
        -> ::std::basic_ostream<CharT, Traits>&;
}

// ----------------------------------------------------------------------------

class nstd::net::ip::address_v6
{
public:
    struct bytes_type
        : ::std::array<unsigned char, 16>
    {
        template <typename... T>
        explicit constexpr bytes_type(T... t)
            : std::array<unsigned char, 16>{{ static_cast<unsigned char>(t)... }}
        {
        }
    };


private:
    bytes_type                     d_bytes;
    ::nstd::net::ip::scope_id_type d_scope_id;

public:
    static constexpr auto any() noexcept -> address_v6;
    static constexpr auto loopback() noexcept -> address_v6;

    constexpr address_v6() noexcept;
    constexpr address_v6(address_v6 const&) noexcept = default;
    constexpr address_v6(bytes_type const&,
                         ::nstd::net::ip::scope_id_type = {});

    auto operator= (address_v6 const&) noexcept -> address_v6& = default;
    constexpr auto operator== (address_v6 const&) const -> bool = default;
    constexpr auto operator<=> (address_v6 const&) const = default;

    auto scope_id(::nstd::net::ip::scope_id_type) noexcept -> void;
    constexpr auto scope_id() const noexcept -> ::nstd::net::ip::scope_id_type;
    constexpr auto is_unspecified() const noexcept -> bool;
    constexpr auto is_loopback() const noexcept -> bool;
    constexpr auto is_multicast() const noexcept -> bool;
    constexpr auto is_link_local() const noexcept -> bool;
    constexpr auto is_site_local() const noexcept -> bool;
    constexpr auto is_v4_mapped() const noexcept -> bool;
    constexpr auto is_multicast_node_local() const noexcept -> bool;
    constexpr auto is_multicast_link_local() const noexcept -> bool;
    constexpr auto is_multicast_site_local() const noexcept -> bool;
    constexpr auto is_multicast_org_local() const noexcept -> bool;
    constexpr auto is_multicast_global() const noexcept -> bool;
    constexpr auto to_bytes() const noexcept -> bytes_type;
    template <typename Allocator = ::std::allocator<char>>
    auto to_string(Allocator const& = {}) const
        -> ::std::basic_string<char, ::std::char_traits<char>, Allocator>;
};

// ----------------------------------------------------------------------------

inline constexpr nstd::net::ip::address_v6::address_v6() noexcept
    : d_bytes()
    , d_scope_id()
{
}

inline constexpr nstd::net::ip::address_v6::address_v6(
    ::nstd::net::ip::address_v6::bytes_type const& bytes,
    ::nstd::net::ip::scope_id_type                 scope_id)
    : d_bytes(bytes)
    , d_scope_id(scope_id)
{
}

// ----------------------------------------------------------------------------

inline constexpr auto nstd::net::ip::address_v6::to_bytes() const noexcept
    -> ::nstd::net::ip::address_v6::bytes_type
{
    return this->d_bytes;
}

inline auto nstd::net::ip::address_v6::scope_id(::nstd::net::ip::scope_id_type scope_id) noexcept
    -> void
{
    this->d_scope_id = scope_id;
}

inline constexpr auto nstd::net::ip::address_v6::scope_id() const noexcept
    -> ::nstd::net::ip::scope_id_type
{
    return this->d_scope_id;
}

inline constexpr auto nstd::net::ip::address_v6::is_unspecified() const noexcept
    -> bool
{
    return true; //-dk:TODO is_unspecified
}

inline constexpr auto nstd::net::ip::address_v6::is_loopback() const noexcept
    -> bool
{
    return true; //-dk:TODO is_loopback
}

inline constexpr auto nstd::net::ip::address_v6::is_multicast() const noexcept
    -> bool
{
    return true; //-dk:TODO is_multicast
}

template <typename Allocator>
inline auto nstd::net::ip::address_v6::to_string(Allocator const& a) const
    -> ::std::basic_string<char, ::std::char_traits<char>, Allocator>
{
    ::std::ostringstream out;
    #if 0
    out << int(this->d_bytes[0]) << '.'
        << int(this->d_bytes[1]) << '.'
        << int(this->d_bytes[2]) << '.'
        << int(this->d_bytes[3]);
        #endif
    auto tmp(out.str());
    return ::std::basic_string<char, ::std::char_traits<char>, Allocator>(tmp.begin(), tmp.end(), a);
}

// ----------------------------------------------------------------------------

inline constexpr auto nstd::net::ip::address_v6::any() noexcept
    -> ::nstd::net::ip::address_v6
{
    return ::nstd::net::ip::address_v6();
}

inline constexpr auto nstd::net::ip::address_v6::loopback() noexcept
    -> ::nstd::net::ip::address_v6
{
    return ::nstd::net::ip::address_v6();
}

// ----------------------------------------------------------------------------

#endif
