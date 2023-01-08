// nstd/file/test_context.cpp                                         -*-C++-*-
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

#include <nstd/file/test_context.hpp>

namespace NF = ::nstd::file;

// ----------------------------------------------------------------------------

namespace nstd::file {
    int test_context_dummy = 0;
}

// ----------------------------------------------------------------------------

NF::test_context::test_context()
    : on_cancel([this](::nstd::file::context::io_base*, ::nstd::file::context::io_base* cont){ this->make_ready(-1, 0, cont); })
    , on_nop([this](::nstd::file::context::io_base* cont){ this->make_ready(-1, 0, cont); })
    , on_timer([this](::nstd::file::context::time_spec*, ::nstd::file::context::io_base* cont){ this->make_ready(-1, 0, cont); })
    , on_accept([this](int, ::sockaddr*, ::socklen_t*, int, ::nstd::file::context::io_base* cont){ this->make_ready(-1, 0, cont); })
    , on_connect([this](int, ::sockaddr const*, ::socklen_t, ::nstd::file::context::io_base* cont){ this->make_ready(-1, 0, cont); })
    , on_sendmsg([this](int, ::msghdr const*, int, ::nstd::file::context::io_base* cont){ this->make_ready(-1, 0, cont); })
    , on_recvmsg([this](int, ::msghdr*, int, ::nstd::file::context::io_base* cont){ this->make_ready(-1, 0, cont); })
    , on_read([this](int, ::iovec*, ::std::size_t, ::nstd::file::context::io_base* cont){ this->make_ready(-1, 0, cont); })
    , on_open_at([this](int, char const*, int, ::nstd::file::context::io_base* cont){ this->make_ready(-1, 0, cont); })
{
}

// ----------------------------------------------------------------------------

auto NF::test_context::do_run_one() -> ::nstd::file::context::count_type
{
    if (this->d_ready.empty()) {
        return 0;
    }

    auto[rc, flags, cont] = this->d_ready.back();
    this->d_ready.pop_back();
    cont->result(rc, flags);
    return 1;
}

auto NF::test_context::do_cancel(io_base* to_cancel, io_base* cont) -> void
{
    this->on_cancel(to_cancel, cont);
}

auto NF::test_context::do_nop(io_base* cont) -> void
{
    this->on_nop(cont);
}

auto NF::test_context::do_timer(::nstd::file::context::time_spec* spec, io_base* cont) -> void
{
    this->on_timer(spec, cont);
}

auto NF::test_context::do_accept(int fd, ::sockaddr* addr, ::socklen_t* len, int flags, io_base* cont) -> void
{
    this->on_accept(fd, addr, len, flags, cont);
}

auto NF::test_context::do_connect(int fd, ::sockaddr const* addr, ::socklen_t len, io_base* cont) -> void
{
    this->on_connect(fd, addr, len, cont);
}

auto NF::test_context::do_sendmsg(int fd, ::msghdr const* msg, int len, io_base* cont) -> void
{
    this->on_sendmsg(fd, msg, len, cont);
}

auto NF::test_context::do_recvmsg(int fd, ::msghdr* msg, int len, io_base* cont) -> void
{
    this->on_recvmsg(fd, msg, len, cont);
}

auto NF::test_context::do_read(int fd, ::iovec* data, ::std::size_t len, io_base* cont) -> void
{
    this->on_read(fd, data, len, cont);
}

auto NF::test_context::do_open_at(int fd, char const* path, int flags, io_base* cont) -> void
{
    this->on_open_at(fd, path, flags, cont);
}

// ----------------------------------------------------------------------------

auto NF::test_context::make_ready(int rc, int flags, ::nstd::file::context::io_base* cont) -> void
{
    this->d_ready.emplace_front(rc, flags, cont);
}
