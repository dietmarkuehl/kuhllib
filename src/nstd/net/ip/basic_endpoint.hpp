// nstd/net/ip/basic_endpoint.hpp                                     -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_IP_BASIC_ENDPOINT
#define INCLUDED_NSTD_NET_IP_BASIC_ENDPOINT

#include "nstd/net/netfwd.hpp"
#include "nstd/net/ip/address.hpp"
#include "nstd/net/ip/tcp.hpp"
#include "nstd/net/ip/types.hpp"
#include <ostream>

// ----------------------------------------------------------------------------

namespace nstd::net::ip {
    template <typename InternetProtocol>
    class basic_endpoint;
}

// ----------------------------------------------------------------------------

template <typename InternetProtocol>
class nstd::net::ip::basic_endpoint
{
public:
    using protocol_type = InternetProtocol;

private: 
    protocol_type              d_protocol{InternetProtocol::v6()};
    ::nstd::net::ip::address   d_address{};
    ::nstd::net::ip::port_type d_port{};

public:
    constexpr basic_endpoint() noexcept = default;
    constexpr basic_endpoint(protocol_type const& protocol, ::nstd::net::ip::port_type port) noexcept;
    constexpr basic_endpoint(::nstd::net::ip::address const& addr, ::nstd::net::ip::port_type port) noexcept;

    constexpr auto protocol() const noexcept -> protocol_type;
    constexpr auto address() const noexcept -> ::nstd::net::ip::address;
    constexpr auto port() const noexcept -> ::nstd::net::ip::port_type;

    auto address(::nstd::net::ip::address const& addr) noexcept -> void;
    auto port(::nstd::net::ip::port_type port) noexcept -> void;

    constexpr auto operator== (basic_endpoint const&) const noexcept -> bool = default;
    constexpr auto operator<=>(basic_endpoint const&) const noexcept = default;

    template <typename CT, typename Traits>
    auto print(std::basic_ostream<CT, Traits>& out) const -> std::basic_ostream<CT, Traits>&;
    
    template <typename CT, typename Traits>
    friend auto operator<< (std::basic_ostream<CT, Traits>& out, basic_endpoint const& ep)
        -> std::basic_ostream<CT, Traits>& {
        return ep.print(out);
    }

    auto get_address(::sockaddr_storage*) const -> ::socklen_t;       // implementation specific
    auto set_address(::sockaddr_storage const*, ::socklen_t) -> void; // implementation specific
};

// ----------------------------------------------------------------------------

template <typename InternetProtocol>
inline constexpr nstd::net::ip::basic_endpoint<InternetProtocol>::basic_endpoint(
    protocol_type const& protocol,
    ::nstd::net::ip::port_type port) noexcept
    : d_protocol(protocol)
    , d_port(port)
{
}

template <typename InternetProtocol>
inline constexpr nstd::net::ip::basic_endpoint<InternetProtocol>::basic_endpoint(
    ::nstd::net::ip::address const& address,
    ::nstd::net::ip::port_type port) noexcept
    : d_protocol(address.is_v4()? ::nstd::net::ip::tcp::v4(): ::nstd::net::ip::tcp::v6())
    , d_address(address)
    , d_port(port)
{
}

// ----------------------------------------------------------------------------

template <typename InternetProtocol>
inline constexpr auto nstd::net::ip::basic_endpoint<InternetProtocol>::protocol() const noexcept
    -> ::nstd::net::ip::basic_endpoint<InternetProtocol>::protocol_type
{
    return this->d_protocol;
}

template <typename InternetProtocol>
inline constexpr auto nstd::net::ip::basic_endpoint<InternetProtocol>::address() const noexcept
    -> ::nstd::net::ip::address
{
    return this->d_address;
}

template <typename InternetProtocol>
constexpr auto nstd::net::ip::basic_endpoint<InternetProtocol>::port() const noexcept
    -> port_type
{
    return this->d_port;
}

// ----------------------------------------------------------------------------

template <typename InternetProtocol>
template <typename CT, typename Traits>
auto nstd::net::ip::basic_endpoint<InternetProtocol>::print(std::basic_ostream<CT, Traits>& out) const
    -> std::basic_ostream<CT, Traits>&
{
    return out << this->address() << ':' << this->port();

}

// ----------------------------------------------------------------------------

template <typename InternetProtocol>
auto nstd::net::ip::basic_endpoint<InternetProtocol>::get_address(::sockaddr_storage* storage) const
    -> ::socklen_t
{
    return this->d_address.get_address(storage, this->d_port);
}

template <typename InternetProtocol>
auto nstd::net::ip::basic_endpoint<InternetProtocol>::set_address(::sockaddr_storage const* storage,
                                                                  ::socklen_t length)
    -> void
{
    if (sizeof(::sockaddr_in) == length) {
        ::sockaddr_in const* addr(reinterpret_cast<::sockaddr_in const*>(storage));
        this->d_port = ntohs(addr->sin_port);
        this->d_address = ::nstd::net::ip::address_v4(addr->sin_addr.s_addr);
    }
    else if (sizeof(::sockaddr_in6) == length) {
        ::sockaddr_in6 const* addr(reinterpret_cast<::sockaddr_in6 const*>(storage));
        this->d_port = ntohs(addr->sin6_port);
        //-dk:TODO
    }
    (void)storage;
    (void)length;
}

// ----------------------------------------------------------------------------

#endif
