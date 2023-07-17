// toy-networking-common.hpp                                          -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#ifndef INCLUDED_TOY_NETWORKING_COMMON
#define INCLUDED_TOY_NETWORKING_COMMON

#include <array>
#include <concepts>
#include <iostream> //-dk:TODO remove
#include <optional>
#include <string>
#include <cstddef>
#ifdef TOY_HAS_SYS_SOCKET
#include <sys/socket.h>
#endif
#ifdef TOY_HAS_NETINET_IN
#include <netinet/in.h>
#endif
#ifdef TOY_HAS_UNISTD
#include <unistd.h>
#endif
#ifdef TOY_HAS_POLL
#include <poll.h>
#endif
#ifdef TOY_HAS_WINSOCK2
#include <winsock2.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>
#endif

#ifdef _MSC_VER
using sa_family_t = unsigned short;
using in_port_t = unsigned short;
#endif

// ----------------------------------------------------------------------------

namespace toy {

// ----------------------------------------------------------------------------

enum class message_flags {
};

// ----------------------------------------------------------------------------

struct address {
    address()
        : len(sizeof(::sockaddr_storage))
        , addr{} {
    }
    address(sa_family_t family, in_port_t port, uint32_t addr) {
        len = sizeof(::sockaddr_in);
        as_addr_in().sin_family      = family;
        as_addr_in().sin_port        = port;
        as_addr_in().sin_addr.s_addr = addr;
    }
    ::socklen_t        len{};
    ::sockaddr_storage addr{};

    ::socklen_t size() const { return len; }
    ::socklen_t capacity() const { return sizeof(::sockaddr_storage); }
    void        resize(std::size_t s) { len = socklen_t(s); }

    ::sockaddr const&     as_addr()     const { return reinterpret_cast<::sockaddr const&>(addr); }
    ::sockaddr_in const&  as_addr_in()  const { return reinterpret_cast<::sockaddr_in const&>(addr); }
    ::sockaddr_in6 const& as_addr_in6() const { return reinterpret_cast<::sockaddr_in6 const&>(addr); }

    ::sockaddr&     as_addr()     { return reinterpret_cast<::sockaddr&>(addr); }
    ::sockaddr_in&  as_addr_in()  { return reinterpret_cast<::sockaddr_in&>(addr); }
    ::sockaddr_in6& as_addr_in6() { return reinterpret_cast<::sockaddr_in6&>(addr); }
};

// ----------------------------------------------------------------------------

template <typename B>
concept byte_type
    =   std::same_as<std::remove_cv_t<B>, char>
    ||  std::same_as<std::remove_cv_t<B>, signed char>
    ||  std::same_as<std::remove_cv_t<B>, unsigned char>
    ||  std::same_as<std::remove_cv_t<B>, std::byte>
    ;

// ----------------------------------------------------------------------------
// deal with different iovec definitions

#ifdef TOY_HAS_WINSOCK2
    using iovec = ::WSABUF;
#else
    using iovec = ::iovec;
#endif

toy::iovec make_iovec(toy::byte_type auto* buffer, ::std::size_t len) {
#ifdef TOY_HAS_WINSOCK2
    return toy::iovec{
            .len = static_cast<ULONG>(len),
            .buf = const_cast<char*>(reinterpret_cast<char const*>(buffer))
        };
#else
    return toy::iovec{
            .iov_base = const_cast<char*>(reinterpret_cast<char const*>(buffer)),
            .iov_len = len
        };
#endif
}

template <typename B>
concept byte_array
    =   std::is_bounded_array_v<std::remove_cvref_t<B>>
    &&  toy::byte_type<std::remove_extent_t<std::remove_cvref_t<B>>>
    ;

template <toy::byte_array... B>
std::array<toy::iovec, sizeof...(B)> buffer(B&&... b) {
    return std::array<toy::iovec, sizeof...(B)>{
        toy::make_iovec(b, ::std::extent_v<std::remove_cvref_t<B>>)...
    };
}
template <toy::byte_type B>
std::array<toy::iovec, 1> buffer(B* b, std::size_t n) {
    return std::array<toy::iovec, 1>{ toy::make_iovec(b, n) };
}
template <toy::byte_type B>
std::array<toy::iovec, 1> buffer(std::basic_string<B>& s) {
    return std::array<toy::iovec, 1>{ toy::make_iovec(s.data(), s.size()) };
}


// ----------------------------------------------------------------------------

}

#endif
