// nstd/net/socket_base.hpp                                           -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_SOCKET_BASE
#define INCLUDED_NSTD_NET_SOCKET_BASE

#include "nstd/hidden_names/boolean_socket_option.hpp"
#include "nstd/file/descriptor.hpp"
#include <sys/socket.h>

// ----------------------------------------------------------------------------

namespace nstd::net {
    class socket_base;
}

// ----------------------------------------------------------------------------

class nstd::net::socket_base
{
private:
    ::nstd::file::descriptor d_descriptor;
    int                      d_flags;

protected:
    auto open(int domain, int type, int protocol) -> int;

public:
    using reuse_address = ::nstd::hidden_names::boolean_socket_option<SO_REUSEADDR>;
    //-dk:TODO define the various member types

    static constexpr int max_listen_connections = SOMAXCONN;

    socket_base() = default;
    socket_base(int fd): d_descriptor(fd) {}
    socket_base(int, int, int);
    socket_base(socket_base const&) = delete;
    socket_base(socket_base&&) = default;
    ~socket_base() = default;
    auto operator= (socket_base const&) -> socket_base& = delete;
    auto operator= (socket_base&&) -> socket_base& = default;

    using native_handle_type = int;
    auto native_handle() const -> native_handle_type { return this->d_descriptor.get(); }
    auto is_open() const noexcept -> bool { return this->d_descriptor.is_open(); }

    auto non_blocking() const noexcept -> bool;
};

// ----------------------------------------------------------------------------

#endif
