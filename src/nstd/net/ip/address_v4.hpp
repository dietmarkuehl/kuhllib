// nstd/net/ip/address_v4.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_IP_ADDRESS_V4
#define INCLUDED_NSTD_NET_IP_ADDRESS_V4

#include "nstd/net/netfwd.hpp"
#include "nstd/net/ip/v4_mapped.hpp"
#include <system_error>
#include <iosfwd>
#include <array>
#include <string>
#include <string_view>
#include <sstream>
#include <cstdint>

// ----------------------------------------------------------------------------

namespace nstd::net::ip {
    struct address_v4_bytes_type;
    class address_v4;

    constexpr auto make_address_v4(::nstd::net::ip::address_v4_bytes_type const&)
        -> ::nstd::net::ip::address_v4;
    constexpr auto make_address_v4(::std::uint_least32_t)
        -> ::nstd::net::ip::address_v4;
    constexpr auto make_address_v4(::nstd::net::ip::v4_mapped_t,
                                   ::nstd::net::ip::address_v6 const&)
        -> ::nstd::net::ip::address_v4;
    constexpr auto make_address_v4(char const*)
        -> ::nstd::net::ip::address_v4;
    constexpr auto make_address_v4(char const*, ::std::error_code&) noexcept
        -> ::nstd::net::ip::address_v4;
    constexpr auto make_address_v4(::std::string const&)
        -> ::nstd::net::ip::address_v4;
    constexpr auto make_address_v4(::std::string const&, ::std::error_code&) noexcept
        -> ::nstd::net::ip::address_v4;
    constexpr auto make_address_v4(::std::string_view)
        -> ::nstd::net::ip::address_v4;
    constexpr auto make_address_v4(::std::string_view, ::std::error_code&) noexcept
        -> ::nstd::net::ip::address_v4;

    template <typename CharT, typename Traits>
    auto operator<< (::std::basic_ostream<CharT, Traits>&,
                     ::nstd::net::ip::address_v4 const&)
        -> ::std::basic_ostream<CharT, Traits>&;
}

// ----------------------------------------------------------------------------

struct nstd::net::ip::address_v4_bytes_type
    : ::std::array<unsigned char, 4>
{
    template <typename... T>
    explicit constexpr address_v4_bytes_type(T... t)
        : std::array<unsigned char, 4>{{ static_cast<unsigned char>(t)... }}
    {
    }
};

// ----------------------------------------------------------------------------

class nstd::net::ip::address_v4
{
public:
    using uint_type = ::std::uint_least32_t;
    using bytes_type = ::nstd::net::ip::address_v4_bytes_type;

private:
    bytes_type d_bytes;

public:
    static constexpr auto any() noexcept -> address_v4;
    static constexpr auto loopback() noexcept -> address_v4;
    static constexpr auto broadcast() noexcept -> address_v4;

    constexpr address_v4() noexcept = default;
    constexpr address_v4(address_v4 const&) noexcept = default;
    constexpr address_v4(bytes_type const&);
    explicit constexpr address_v4(uint_type);

    auto operator= (address_v4 const&) noexcept -> address_v4& = default;
    constexpr auto operator== (address_v4 const&) const -> bool = default;
    constexpr auto operator<=> (address_v4 const&) const = default;

    constexpr auto is_unspecified() const noexcept -> bool;
    constexpr auto is_loopback() const noexcept -> bool;
    constexpr auto is_multicast() const noexcept -> bool;
    constexpr auto to_bytes() const noexcept -> bytes_type;
    constexpr auto to_uint() const noexcept -> uint_type;
    template <typename Allocator = ::std::allocator<char>>
    auto to_string(Allocator const& = {}) const
        -> ::std::basic_string<char, ::std::char_traits<char>, Allocator>;
};

// ----------------------------------------------------------------------------

inline constexpr nstd::net::ip::address_v4::address_v4(
    ::nstd::net::ip::address_v4::bytes_type const& bytes)
    : d_bytes(bytes)
{
}

inline constexpr nstd::net::ip::address_v4::address_v4(
    ::nstd::net::ip::address_v4::uint_type value)
    : d_bytes{ (value >> 24) & 0xff, (value >> 16) & 0xff, (value >> 8) & 0xff, (value >> 0) & 0xff }
{
}

// ----------------------------------------------------------------------------

inline constexpr auto nstd::net::ip::address_v4::to_bytes() const noexcept
    -> ::nstd::net::ip::address_v4::bytes_type
{
    return this->d_bytes;
}

inline constexpr auto nstd::net::ip::address_v4::to_uint() const noexcept
    -> ::nstd::net::ip::address_v4::uint_type
{
    return (::nstd::net::ip::address_v4::uint_type(this->d_bytes[0]) << 24)
        |  (::nstd::net::ip::address_v4::uint_type(this->d_bytes[1]) << 16)
        |  (::nstd::net::ip::address_v4::uint_type(this->d_bytes[2]) <<  8)
        |  (::nstd::net::ip::address_v4::uint_type(this->d_bytes[3]) <<  0)
        ;
}

inline constexpr auto nstd::net::ip::address_v4::is_unspecified() const noexcept
    -> bool
{
    return this->to_uint() == ::nstd::net::ip::address_v4::uint_type();
}

inline constexpr auto nstd::net::ip::address_v4::is_loopback() const noexcept
    -> bool
{
    return (this->to_uint() & ::nstd::net::ip::address_v4::uint_type(0xFF000000))
        == ::nstd::net::ip::address_v4::uint_type(0x7F000000);
}

inline constexpr auto nstd::net::ip::address_v4::is_multicast() const noexcept
    -> bool
{
    return (this->to_uint() & ::nstd::net::ip::address_v4::uint_type(0xF0000000))
        == ::nstd::net::ip::address_v4::uint_type(0xE0000000);
}

template <typename Allocator>
inline auto nstd::net::ip::address_v4::to_string(Allocator const& a) const
    -> ::std::basic_string<char, ::std::char_traits<char>, Allocator>
{
    ::std::ostringstream out;
    out << int(this->d_bytes[0]) << '.'
        << int(this->d_bytes[1]) << '.'
        << int(this->d_bytes[2]) << '.'
        << int(this->d_bytes[3]);
    auto tmp(out.str());
    return ::std::basic_string<char, ::std::char_traits<char>, Allocator>(tmp.begin(), tmp.end(), a);
}

// ----------------------------------------------------------------------------

inline constexpr auto nstd::net::ip::address_v4::any() noexcept
    -> ::nstd::net::ip::address_v4
{
    return ::nstd::net::ip::address_v4();
}

inline constexpr auto nstd::net::ip::address_v4::loopback() noexcept
    -> ::nstd::net::ip::address_v4
{
    return ::nstd::net::ip::address_v4(0x7F000001);
}

inline constexpr auto nstd::net::ip::address_v4::broadcast() noexcept
    -> ::nstd::net::ip::address_v4
{
    return ::nstd::net::ip::address_v4(0xFFFFFFFF);
}

// ----------------------------------------------------------------------------

#endif
