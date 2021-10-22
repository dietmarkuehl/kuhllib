// nstd/file/poll_context.cpp                                         -*-C++-*-
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

#include "nstd/file/poll_context.hpp"
#include "nstd/utility/move.hpp"
#include <algorithm>
#include <iostream>
#include <cassert>
#include <cerrno>
#include <fcntl.h>

namespace NF = ::nstd::file;
namespace UT = ::nstd::utility;

// ----------------------------------------------------------------------------

NF::poll_context::operation::operation(int fd, short events, ::std::function<auto()->bool> operation)
    : d_fd(fd)
    , d_events(events)
    , d_operation(UT::move(operation))
{
}

NF::poll_context::poll_context()
    : d_poll()
    , d_next_poll(this->d_poll.end())
    , d_outstanding()
{
}

// ----------------------------------------------------------------------------

auto NF::poll_context::submit_io(int fd, short events, ::std::function<auto()->bool> op) -> void
{
    this->d_outstanding.emplace_back(fd, events, UT::move(op));
}

// ----------------------------------------------------------------------------

auto NF::poll_context::do_run_one() -> NF::context::count_type
{
    while (true) {
        auto oit{::std::find_if(this->d_outstanding.begin(), this->d_outstanding.end(),
                                [](operation const& op){ return op.d_fd == -1; })};
        if (oit != this->d_outstanding.end()) {
            oit->d_operation();
            this->d_outstanding.erase(oit);
            return 1u;
        }
        for (; this->d_poll.end() != (this->d_next_poll = ::std::find_if(this->d_next_poll, this->d_poll.end(), [](pollfd const& p){ return p.revents; }));
             ++this->d_next_poll) {
            for (auto rit{this->d_outstanding.begin()}; rit != this->d_outstanding.end(); ++rit) {
                if (this->d_next_poll->fd == rit->d_fd
                    && 0 != (this->d_next_poll->revents & rit->d_events)
                    && rit->d_operation())
                {
                     return 1u;
                }
            }
        }
        this->d_poll.clear();
        for (auto const& operation: this->d_outstanding) {
            if (0 <= operation.d_fd) {
                this->d_poll.push_back(::pollfd{ operation.d_fd, operation.d_events, 0 });
            }
        }
        if (this->d_poll.empty()) {
            return 0u;
        }
        int rc{::poll(this->d_poll.data(), this->d_poll.size(), -1)};
        if (0 < rc) {
            return 0u;
        }
    }
}

// ----------------------------------------------------------------------------

auto NF::poll_context::do_nop(NF::context::io_base*) -> void
{
    ::std::cout << "poll_context::do_nop isn't implemented\n" << ::std::flush;
    assert("poll_context::do_nop isn't implemented" == nullptr);
}

auto NF::poll_context::do_timer(NF::context::time_spec*, NF::context::io_base*) -> void
{
    ::std::cout << "poll_context::do_timer isn't implemented\n" << ::std::flush;
    assert("poll_context::do_timer isn't implemented" == nullptr);
}

auto NF::poll_context::do_accept(NF::context::native_handle_type fd,
                                 ::sockaddr*                     address,
                                 ::socklen_t*                    length,
                                 int                             /*flags*/,
                                 NF::context::io_base*           continuation) -> void
{
    auto tryAccept = [fd, address, length, continuation]{
        int rc{::accept(fd, address, length)};
        if (0 <= rc) {
            continuation->result(rc, 0);
            return true;
        }
        switch (errno) {
        default:
            continuation->result(-errno, 0);
            return true;
        case EINTR:
        case EWOULDBLOCK:
            return false;
        }
    };
    if (!tryAccept()) {
        this->submit_io(fd, POLLIN, UT::move(tryAccept));
    }
}

auto NF::poll_context::do_connect(NF::context::native_handle_type fd, ::sockaddr const* address, ::socklen_t length, NF::context::io_base* continuation)
    -> void
{
    auto tryConnect = [fd, address, length, continuation]{
        if (0 == ::connect(fd, address, length)) {
            continuation->result(0, 0);
            return true;
        }
        switch (errno) {
        default:
            continuation->result(-errno, 0);
            return true;
        case EALREADY:
        case EINPROGRESS:
        case EINTR:
            return false;
        }
    };
    if (!tryConnect()) {
        this->submit_io(fd, POLLOUT, UT::move(tryConnect));
    }
}

auto NF::poll_context::do_sendmsg(NF::context::native_handle_type, ::msghdr const*, int, NF::context::io_base*) -> void
{
    ::std::cout << "poll_context::do_sendmsg isn't implemented\n" << ::std::flush;
    assert("poll_context::do_sendmsg isn't implemented" == nullptr);
}

auto NF::poll_context::do_recvmsg(NF::context::native_handle_type, ::msghdr*, int, NF::context::io_base*) -> void
{
    ::std::cout << "poll_context::do_recvmsg isn't implemented\n" << ::std::flush;
    assert("poll_context::do_recvmsg isn't implemented" == nullptr);
}

auto NF::poll_context::do_read(int, ::iovec*, ::std::size_t, NF::context::io_base*) -> void
{
    ::std::cout << "poll_context::do_read isn't implemented\n" << ::std::flush;
    assert("poll_context::do_read isn't implemented" == nullptr);
}

auto NF::poll_context::do_open_at(int fd, char const* name, int flags, NF::context::io_base* continuation)
    -> void
{
    int rc(::openat(fd, name, flags));
    continuation->result(rc < 0? -errno: rc, 0);
}

// ----------------------------------------------------------------------------

namespace nstd::file {
    int poll_context_dummy = 0;
}
