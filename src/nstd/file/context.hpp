// nstd/file/context.hpp                                              -*-C++-*-
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

#ifndef INCLUDED_NSTD_FILE_CONTEXT
#define INCLUDED_NSTD_FILE_CONTEXT

#include <sys/socket.h>
#include <sys/types.h>
#include <linux/time_types.h>
#include <cstdint>

// ----------------------------------------------------------------------------

namespace nstd::file {
    // This class is used as the base class for different asynchronous
    // implementation of various operations.
    class context;
}

// ----------------------------------------------------------------------------

class nstd::file::context
{
public:
    class io_base;
    using count_type = ::std::size_t;
    using native_handle_type = int;

protected:
    virtual auto do_run_one() -> count_type = 0;

    virtual auto do_timer(::__kernel_timespec*, io_base*) -> void = 0;
    virtual auto do_accept(native_handle_type, ::sockaddr*, ::socklen_t*, int, io_base*) -> void = 0;
    virtual auto do_connect(native_handle_type, ::sockaddr const*, ::socklen_t, io_base*) -> void = 0;
    virtual auto do_sendmsg(native_handle_type, ::msghdr const*, int, io_base*) -> void = 0;
    virtual auto do_recvmsg(native_handle_type, ::msghdr*, int, io_base*) -> void = 0;
    virtual auto do_read(int, ::iovec*, ::std::size_t, io_base*) -> void = 0;
    virtual auto do_open_at(int, char const*, int, io_base*) -> void = 0;

public:

    context() = default;
    context(context const&) = delete;
    auto operator= (context const&) -> context& = delete;
    virtual ~context();

    auto run_one() -> count_type;

    auto timer(::__kernel_timespec*, io_base*) -> void;
    auto accept(native_handle_type, ::sockaddr*, ::socklen_t*, int, io_base*) -> void;
    auto connect(native_handle_type, ::sockaddr const*, socklen_t, io_base*) -> void;
    auto sendmsg(native_handle_type, ::msghdr const*, int, io_base*) -> void;
    auto recvmsg(native_handle_type, ::msghdr*, int, io_base*) -> void;
    auto read(native_handle_type, ::iovec*, ::std::size_t, io_base*) -> void;
    auto open_at(int, char const*, int, io_base*) -> void;
};

// ----------------------------------------------------------------------------

class nstd::file::context::io_base
{
protected:
    virtual ~io_base() = default;
    virtual auto do_result(::std::int32_t, ::std::uint32_t) -> void = 0;

public:
    auto result(::std::int32_t res, ::std::uint32_t flags) -> void { this->do_result(res, flags); }
};

// ----------------------------------------------------------------------------

inline auto nstd::file::context::run_one() -> count_type
{
    return this->do_run_one();
}

inline auto nstd::file::context::timer(::__kernel_timespec* time,
                                       ::nstd::file::context::io_base* handler)
    -> void
{
    this->do_timer(time, handler);
}

inline auto nstd::file::context::accept(::nstd::file::context::native_handle_type fd,
                                        ::sockaddr* addr,
                                        ::socklen_t* len,
                                        int flags,
                                        ::nstd::file::context::io_base* handler)
    -> void
{
    this->do_accept(fd, addr, len, flags, handler);
}

inline auto nstd::file::context::connect(::nstd::file::context::native_handle_type fd,
                                         ::sockaddr const* addr,
                                         ::socklen_t len,
                                         ::nstd::file::context::io_base* handler)
    -> void
{
    this->do_connect(fd, addr, len, handler);
}

inline auto nstd::file::context::sendmsg(::nstd::file::context::native_handle_type fd,
                                         ::msghdr const* msg,
                                         int len,
                                         ::nstd::file::context::io_base* handler)
    -> void
{
    this->do_sendmsg(fd, msg, len, handler);
}

inline auto nstd::file::context::recvmsg(::nstd::file::context::native_handle_type fd,
                                         ::msghdr* msg,
                                         int len,
                                         ::nstd::file::context::io_base* handler)
    -> void
{
    this->do_recvmsg(fd, msg, len, handler);
}

inline auto nstd::file::context::read(::nstd::file::context::native_handle_type fd,
                                      ::iovec* vec,
                                      ::std::size_t size,
                                      io_base* handler) -> void
{
    this->do_read(fd, vec, size, handler);
}

inline auto nstd::file::context::open_at(int fd,
                                         char const* name,
                                         int flags,
                                         io_base* handler) -> void
{
    this->do_open_at(fd, name, flags, handler);
}

// ----------------------------------------------------------------------------

#endif
