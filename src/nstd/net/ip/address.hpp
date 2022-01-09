// nstd/net/ip/address.hpp                                            -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_IP_ADDRESS
#define INCLUDED_NSTD_NET_IP_ADDRESS

#include "nstd/net/ip/address_v4.hpp"
#include "nstd/net/ip/address_v6.hpp"
#include <compare>
#include <variant>
#include "nstd/file/socket.hpp"

// ----------------------------------------------------------------------------

namespace nstd::net::ip {
    class address;
}

// ----------------------------------------------------------------------------

class nstd::net::ip::address
{
private:
    enum { v4_index, v6_index };
    using address_type = ::std::variant<::nstd::net::ip::address_v4, ::nstd::net::ip::address_v6>;
    address_type d_address;

public:
    constexpr address() noexcept = default;
    constexpr address(address const&) noexcept = default;
    constexpr address(::nstd::net::ip::address_v4 const&) noexcept;
    constexpr address(::nstd::net::ip::address_v6 const&) noexcept;
    auto operator=(address const&) noexcept -> ::nstd::net::ip::address& = default;
    auto operator=(::nstd::net::ip::address_v4 const&) noexcept -> ::nstd::net::ip::address& ;
    auto operator=(::nstd::net::ip::address_v6 const&) noexcept -> ::nstd::net::ip::address& ;

    constexpr auto operator<=> (::nstd::net::ip::address const&) const noexcept -> ::std::partial_ordering;
    constexpr auto operator== (::nstd::net::ip::address const&) const noexcept -> bool = default;

    auto get_address(::sockaddr_storage*, ::nstd::net::ip::port_type) const -> ::socklen_t;

    constexpr auto is_v4() const noexcept -> bool;
    constexpr auto is_v6() const noexcept -> bool;
    constexpr auto to_v4() const -> ::nstd::net::ip::address_v4;
    constexpr auto to_v6() const -> ::nstd::net::ip::address_v6;

    constexpr auto is_unspecified() const noexcept -> bool;
    constexpr auto is_loopback() const noexcept -> bool;
    constexpr auto is_multicast() const noexcept -> bool;

    template <typename Allocator = ::std::allocator<char>>
    auto to_string(Allocator const& = Allocator()) const
        -> ::std::basic_string<char, ::std::char_traits<char>, Allocator>;
    
    friend ::std::ostream& operator << (::std::ostream& out,
                                        address const& address) {
        return out << address.to_string();
    }
};

// ----------------------------------------------------------------------------

inline constexpr nstd::net::ip::address::address(::nstd::net::ip::address_v4 const& addr) noexcept
    : d_address(addr)
{
}
inline constexpr nstd::net::ip::address::address(::nstd::net::ip::address_v6 const& addr) noexcept
    : d_address(addr)
{
}

inline auto nstd::net::ip::address::operator=(::nstd::net::ip::address_v4 const& addr) noexcept
    -> ::nstd::net::ip::address&
{
    this->d_address = addr;
    return *this;
}
inline auto nstd::net::ip::address::operator=(::nstd::net::ip::address_v6 const& addr) noexcept
    -> ::nstd::net::ip::address&
{
    this->d_address = addr;
    return *this;
}

// ----------------------------------------------------------------------------

inline constexpr auto nstd::net::ip::address::operator<=> (::nstd::net::ip::address const& other) const noexcept
    -> ::std::partial_ordering
{
    return this->d_address.index() == other.d_address.index()
	    ?  (this->d_address.index() == 0
	   	? ::std::get<0>(this->d_address) <=> ::std::get<0>(other.d_address)
	   	: ::std::get<1>(this->d_address) <=> ::std::get<1>(other.d_address)
	   	)
	    : ::std::partial_ordering::unordered;
	;
}

inline constexpr auto nstd::net::ip::address::is_v4() const noexcept
    -> bool
{
    return this->d_address.index() == v4_index;
}

inline constexpr auto nstd::net::ip::address::is_v6() const noexcept
    -> bool
{
    return this->d_address.index() == v6_index;
}

inline constexpr auto nstd::net::ip::address::to_v4() const
    -> ::nstd::net::ip::address_v4
{
    return ::std::get<v4_index>(this->d_address);
}

inline constexpr auto nstd::net::ip::address::to_v6() const
    -> ::nstd::net::ip::address_v6
{
    return ::std::get<v6_index>(this->d_address);
}

inline constexpr auto nstd::net::ip::address::is_unspecified() const noexcept
    -> bool
{
    return ::std::visit([](auto&& addr){ return addr.is_unspecified(); },
                        this->d_address);
}

inline constexpr auto nstd::net::ip::address::is_loopback() const noexcept
    -> bool
{
    return ::std::visit([](auto&& addr){ return addr.is_loopback(); },
                        this->d_address);
}

inline constexpr auto nstd::net::ip::address::is_multicast() const noexcept
    -> bool
{
    return ::std::visit([](auto&& addr){ return addr.is_multicast(); },
                        this->d_address);
}

template <typename Allocator>
inline auto nstd::net::ip::address::to_string(Allocator const& allocator) const
    -> ::std::basic_string<char, ::std::char_traits<char>, Allocator>
{
    return ::std::visit([&allocator](auto&& addr){ return addr.to_string(allocator); },
                        this->d_address);
}

// ----------------------------------------------------------------------------

inline auto nstd::net::ip::address::get_address(::sockaddr_storage*        storage,
                                                ::nstd::net::ip::port_type port) const
    -> ::socklen_t
{
    return ::std::visit([storage, port](auto&& addr){
        return addr.get_address(storage, port);
        }, this->d_address);
}

// ----------------------------------------------------------------------------

#endif
