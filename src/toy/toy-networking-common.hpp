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

#include <cstddef>
#include <sys/socket.h>
#include <netinet/in.h>

// ----------------------------------------------------------------------------

namespace toy {
    enum class message_flags {
    };
}

// ----------------------------------------------------------------------------

namespace toy {
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
        void        resize(std::size_t s) { len = s; }

        ::sockaddr const&     as_addr()     const { return reinterpret_cast<::sockaddr const&>(addr); }
        ::sockaddr_in const&  as_addr_in()  const { return reinterpret_cast<::sockaddr_in const&>(addr); }
        ::sockaddr_in6 const& as_addr_in6() const { return reinterpret_cast<::sockaddr_in6 const&>(addr); }

        ::sockaddr&     as_addr()     { return reinterpret_cast<::sockaddr&>(addr); }
        ::sockaddr_in&  as_addr_in()  { return reinterpret_cast<::sockaddr_in&>(addr); }
        ::sockaddr_in6& as_addr_in6() { return reinterpret_cast<::sockaddr_in6&>(addr); }
    };
}

// ----------------------------------------------------------------------------

#endif
