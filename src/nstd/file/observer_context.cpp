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
#include <nstd/utility/forward.hpp>
#include <nstd/utility/move.hpp>
#include <algorithm>
#include <iostream>

// ----------------------------------------------------------------------------

namespace nstd::file {
    int observer_context_dummy = 0;
}

// ----------------------------------------------------------------------------

template <typename Fun>
struct nstd::file::observer_context::continuation
    : nstd::file::observer_context::continuation_base
{
    ::nstd::file::observer_context* d_context;
    Fun                             d_fun;
    continuation(::nstd::file::observer_context* context,
                 ::nstd::file::context::io_base* cont,
                 Fun&& fun)
        : continuation_base(cont)
        , d_context(context)
        , d_fun(::nstd::utility::move(fun)) {
    }
    auto do_result(::std::int32_t rc, ::std::uint32_t flags) -> void override {
        ::nstd::file::context::io_base* cont(this->d_cont);
        this->d_fun(rc, flags);
        {
            ::std::lock_guard kerberos(this->d_context->d_mutex);
            this->d_context->d_outstanding.erase(this->d_context->d_outstanding.make_iterator(this));
        }
        cont->result(rc, flags);
    }
};

// ----------------------------------------------------------------------------

nstd::file::observer_context::observer_context(::nstd::file::context& context)
    : d_context(context)
{
}

// ----------------------------------------------------------------------------

template <typename Fun>
auto nstd::file::observer_context::add(nstd::file::context::io_base* cont, Fun&& fun)
    -> continuation_base*
{
    continuation_base* rc(new continuation(this, cont, ::nstd::utility::forward<Fun>(fun)));
    ::std::lock_guard kerberos(this->d_mutex);
    this->d_outstanding.insert(this->d_outstanding.begin(), *rc);
    return rc;
}

// ----------------------------------------------------------------------------

auto nstd::file::observer_context::do_run_one() -> count_type
{
    ::std::cout << "> run_one()\n";
    count_type rc(this->d_context.run_one());
    ::std::cout << "< run_one(): rc=" << rc << "\n";
    return rc;
}

auto nstd::file::observer_context::do_cancel(io_base* to_cancel, io_base* cont) -> void
{
    io_base* upstream(nullptr);
    {
        ::std::lock_guard kerberos(this->d_mutex);
        auto it(::std::find_if(this->d_outstanding.begin(), this->d_outstanding.end(),
                               [to_cancel](auto const& c){ return to_cancel == c.d_cont; }));
        if (it == this->d_outstanding.end()) {
            cont->result(-ENOENT, 0);
            return;
        }
        upstream = &*it;
    }
    this->d_context.cancel(upstream, this->add(cont, [](int rc, int flags){
        ::std::cout << "< cancel: rc=" << rc << ", flags=" << flags << "\n";
    }));
}

// ----------------------------------------------------------------------------

auto nstd::file::observer_context::do_nop(io_base* cont) -> void
{
    ::std::cout << "> nop()\n";
    this->d_context.nop(this->add(cont, [](int rc, int flags){
        ::std::cout << "< nop: rc=" << rc << ", flags=" << flags << "\n";
    }));
}

auto nstd::file::observer_context::do_timer(time_spec* spec, io_base* cont) -> void
{
    ::std::cout << "> timer()\n";
    this->d_context.timer(spec, this->add(cont, [](int rc, int flags){
        ::std::cout << "< timer: rc=" << rc << ", flags=" << flags << "\n";
    }));
}

auto nstd::file::observer_context::do_accept(native_handle_type fd, ::sockaddr* addr, ::socklen_t* len, int flags, io_base* cont) -> void
{
    ::std::cout << "> accept(" << fd << ", ...)\n";
    this->d_context.accept(fd, addr, len, flags, this->add(cont, [fd](int rc, int flags){
        ::std::cout << "< accept(" << fd << "): rc=" << rc << ", flags=" << flags << "\n";
    }));
}

auto nstd::file::observer_context::do_connect(native_handle_type fd, ::sockaddr const* addr, ::socklen_t len, io_base* cont) -> void
{
    ::std::cout << "> connect(" << fd << ", ...)\n";
    this->d_context.connect(fd, addr, len, this->add(cont, [fd](int rc, int flags){
        ::std::cout << "< connect(" << fd << "): rc=" << rc << ", flags=" << flags << "\n";
    }));
}

auto nstd::file::observer_context::do_sendmsg(native_handle_type fd, ::msghdr const* hdr, int len, io_base* cont) -> void
{
    ::std::cout << "> sendmsg(" << fd << ", ...)\n";
    this->d_context.sendmsg(fd, hdr, len, this->add(cont, [fd](int rc, int flags){
        ::std::cout << "< sendmsg(" << fd << "): rc=" << rc << ", flags=" << flags << "\n";
    }));
}

auto nstd::file::observer_context::do_recvmsg(native_handle_type fd, ::msghdr* hdr, int len, io_base* cont) -> void
{
    ::std::cout << "> recvmsg(" << fd << ", ...)\n";
    this->d_context.recvmsg(fd, hdr, len, this->add(cont, [fd](int rc, int flags){
        ::std::cout << "< recvmsg(" << fd << "): rc=" << rc << ", flags=" << flags << "\n";
    }));
}

auto nstd::file::observer_context::do_read(int fd, ::iovec* vec, ::std::size_t len, io_base* cont) -> void
{
    ::std::cout << "> read(" << fd << ", ...)\n";
    this->d_context.read(fd, vec, len, this->add(cont, [fd](int rc, int flags){
        ::std::cout << "< read(" << fd << "): rc=" << rc << ", flags=" << flags << "\n";
    }));
}

auto nstd::file::observer_context::do_open_at(int fd, char const* path, int flags, io_base* cont) -> void
{
    ::std::cout << "> open_at(" << fd << ", '" << path << "', " << std::hex << flags << std::dec << ")\n";
    this->d_context.open_at(fd, path, flags, this->add(cont, [fd](int rc, int flags){
        ::std::cout << "< open_at(" << fd << "): rc=" << rc << ", flags=" << flags << "\n";
    }));
}

auto nstd::file::observer_context::do_recvfrom(native_handle_type fd, void* buffer, ::std::size_t length, int flags, ::sockaddr* addr, ::socklen_t* addrlen, io_base* cont) -> void
{
    ::std::cout << "> recvfrom(" << fd << ", " << buffer << ", " << length << ", " << std::hex << flags << std::dec << ", " << addr << ", " << addrlen << ")\n";
    this->d_context.recvfrom(fd, buffer, length, flags, addr, addrlen, this->add(cont, [=](int size, int){
        ::std::cout << "< recvfrom(" << fd << ", " << buffer << ", " << length << ", " << std::hex << flags << std::dec << ", " << addr << ", " << addrlen << ")->" << size << "\n";
    }));
}

auto nstd::file::observer_context::do_sendto(native_handle_type fd, void const* buffer, ::std::size_t length, int flags, ::sockaddr* addr, ::socklen_t addrlen, io_base* cont) -> void
{
    ::std::cout << "> sendto(" << fd << ", " << buffer << ", " << length << ", " << std::hex << flags << std::dec << ", " << addr << ", " << addrlen << ")\n";
    this->d_context.sendto(fd, buffer, length, flags, addr, addrlen, this->add(cont, [=](int size, int){
        ::std::cout << "< sendto(" << fd << ", " << buffer << ", " << length << ", " << std::hex << flags << std::dec << ", " << addr << ", " << addrlen << ")->" << size << "\n";
    }));
}