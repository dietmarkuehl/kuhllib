// nstd/file/observer_context.cpp                                     -*-C++-*-
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

#include <nstd/file/observer_context.hpp>
#include <iostream>

// ----------------------------------------------------------------------------

namespace nstd::file {
    int observer_context_dummy = 0;
}

// ----------------------------------------------------------------------------

nstd::file::observer_context::observer_context(::nstd::file::context& context)
    : d_context(context)
{
}

// ----------------------------------------------------------------------------

auto nstd::file::observer_context::do_run_one() -> count_type
{
    ::std::cout << "<> run_one()\n";
    return this->d_context.run_one();
}

auto nstd::file::observer_context::do_nop(io_base* cont) -> void
{
    ::std::cout << "<> nop()\n";
    this->d_context.nop(cont);
}

auto nstd::file::observer_context::do_timer(time_spec* spec, io_base* cont) -> void
{
    ::std::cout << "<> timer()\n";
    this->d_context.timer(spec, cont);
}

auto nstd::file::observer_context::do_accept(native_handle_type fd, ::sockaddr* addr, ::socklen_t* len, int flags, io_base* cont) -> void
{
    ::std::cout << "<> accept(" << fd << ", ...)\n";
    this->d_context.accept(fd, addr, len, flags, cont);
}

auto nstd::file::observer_context::do_connect(native_handle_type fd, ::sockaddr const* addr, ::socklen_t len, io_base* cont) -> void
{
    ::std::cout << "<> connect(" << fd << ", ...)\n";
    this->d_context.connect(fd, addr, len, cont);
}

auto nstd::file::observer_context::do_sendmsg(native_handle_type fd, ::msghdr const* hdr, int len, io_base* cont) -> void
{
    ::std::cout << "<> sendmsg(" << fd << ", ...)\n";
    this->d_context.sendmsg(fd, hdr, len, cont);
}

auto nstd::file::observer_context::do_recvmsg(native_handle_type fd, ::msghdr* hdr, int len, io_base* cont) -> void
{
    ::std::cout << "<> recvmsg(" << fd << ", ...)\n";
    this->d_context.recvmsg(fd, hdr, len, cont);
}

auto nstd::file::observer_context::do_read(int fd, ::iovec* vec, ::std::size_t len, io_base* cont) -> void
{
    ::std::cout << "<> read(" << fd << ", ...)\n";
    this->d_context.read(fd, vec, len, cont);
}

auto nstd::file::observer_context::do_open_at(int fd, char const* path, int flags, io_base* cont) -> void
{
    ::std::cout << "<> open_at(" << fd << ", '" << path << "', " << std::hex << flags << std::dec << ")\n";
    this->d_context.open_at(fd, path, flags, cont);
}
