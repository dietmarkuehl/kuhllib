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
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include <algorithm>
#include <exception>
#include <cassert>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <system_error>
#include <cerrno>
#include <cstring>

namespace NF = ::nstd::file;
namespace UT = ::nstd::utility;

// ----------------------------------------------------------------------------

NF::poll_context::timer_event::timer_event(::std::chrono::steady_clock::time_point const& expiry,
                                           io_base*                                       continuation)
    : d_expiry(expiry)
    , d_continuation(continuation) {
}

auto NF::poll_context::timer_event::operator< (timer_event const& other) const
    -> bool
{
    return other.d_expiry < this->d_expiry;
}

// ----------------------------------------------------------------------------

NF::poll_context::operation::operation(int fd, short events, ::std::function<auto()->bool> operation, io_base* id)
    : d_fd(fd)
    , d_events(events)
    , d_operation(UT::move(operation))
    , d_id(id)
{
}

NF::poll_context::poll_context()
    : d_poll()
    , d_next_poll(this->d_poll.end())
    , d_outstanding()
{
    if (::pipe(this->d_pipe) < 0) {
        throw ::std::system_error(errno, ::std::system_category(),
                                  "failed to create self-pipe");
    }

    fcntl(this->d_pipe[0], F_SETFL, O_NONBLOCK);
    fcntl(this->d_pipe[1], F_SETFL, O_NONBLOCK);
    this->submit_io(this->d_pipe[0], POLLIN, [this]{
        char buffer[16];
        ::read(this->d_pipe[0], buffer, sizeof(buffer));
        return false;
        }, &this->d_completion);
}

NF::poll_context::~poll_context() {
    ::close(this->d_pipe[0]);
    ::close(this->d_pipe[1]);
}

// ----------------------------------------------------------------------------

auto NF::poll_context::submit_io(int fd, short events, ::std::function<auto()->bool> op, io_base* id) -> void
{
    if (0 <= fd) {
        this->d_outstanding.emplace_front(fd, events, UT::move(op), id);
    }
    else {
        this->d_outstanding.emplace_back(fd, events, UT::move(op), id);
    }
    ::write(this->d_pipe[1], "", 1);
}

template <typename Fun>
auto NF::poll_context::submit(int fd, short events, Fun&& fun, io_base* id) -> void
{
    if (!fun()) {
        this->submit_io(fd, events, UT::forward<Fun>(fun), id);
    }
}

// ----------------------------------------------------------------------------

auto NF::poll_context::handle_timer() -> NF::context::count_type {
    return 0u;
}

auto NF::poll_context::handle_scheduled() -> NF::context::count_type {
    if (!this->d_outstanding.empty() && this->d_outstanding.back().d_fd == -1) {
        auto it = --this->d_outstanding.end();
        it->d_operation();
        this->d_outstanding.erase(it);

        return 1u;
    }
    return 0u;
}

auto NF::poll_context::handle_io() -> NF::context::count_type {
    for (; this->d_poll.end() != (this->d_next_poll = ::std::find_if(this->d_next_poll, this->d_poll.end(), [](pollfd const& p){ return p.revents; }));
         ++this->d_next_poll) {
        for (auto rit{this->d_outstanding.begin()}; rit != this->d_outstanding.end(); ++rit) {
            if (this->d_next_poll->fd == rit->d_fd
                && 0 != (this->d_next_poll->revents & rit->d_events))
            {
                ::std::list<operation> tmp;
                auto pos(::std::next(rit));
                tmp.splice(tmp.begin(), this->d_outstanding, rit);
                if (tmp.front().d_operation()) {
                    ++this->d_next_poll;
                    return 1u;
                }
                else {
                    this->d_outstanding.splice(pos, tmp, tmp.begin());
                }
            }
        }
    }
    return 0u;
}

// ----------------------------------------------------------------------------

auto NF::poll_context::poll() -> bool
{
    this->d_poll.clear();
    for (auto const& operation: this->d_outstanding) {
        if (operation.d_fd < 0) {
            break;
        }
        short events = operation.d_events;
        this->d_poll.push_back(::pollfd{ operation.d_fd, events, 0 });
    }
    if (1u == this->d_poll.size()) {
        return 0u;
    }
    while (true)
    {
        errno = 0;
        int rc{::poll(this->d_poll.data(), this->d_poll.size(), -1)};
        if (0 <= rc) {
            this->d_next_poll = this->d_poll.begin();
            return true;
        }
        else if (errno != EINVAL) {
            return false;
        }
    }
}

// ----------------------------------------------------------------------------

auto NF::poll_context::do_run_one() -> NF::context::count_type
{
    while (true) {
        if (this->handle_timer() || this->handle_scheduled() || this->handle_io()) {
            return 1u;
        }
        if (!this->poll()) {
            return 0u;
        }
    }
}

// ----------------------------------------------------------------------------

auto NF::poll_context::do_cancel(NF::context::io_base* to_cancel, NF::context::io_base* completion) -> void
{
    this->submit_io(-1, 0, [completion, to_cancel, this]{
        auto it = ::std::find_if(this->d_outstanding.begin(), this->d_outstanding.end(),
                                 [to_cancel](operation const& op){ return op.d_id == to_cancel; });
        completion->result(it == this->d_outstanding.end()? -1: 0, 0);
        if (it != this->d_outstanding.end()) {
            it->d_id->result(-ECANCELED, 0);
            this->d_outstanding.erase(it);
        }
        return true;
    },
    completion);
}

auto NF::poll_context::do_nop(NF::context::io_base* continuation) -> void
{
    this->submit_io(-1, 0, [continuation]{
        continuation->result(0, 0);
        return true;
    },
    continuation);
}

auto NF::poll_context::do_timer(NF::context::time_spec* ts, NF::context::io_base* continuation) -> void
{
    ::std::chrono::duration<::std::uint64_t, ::std::nano> offset(::std::uint64_t(ts->sec) * 1000000000 + ts->nsec);
    this->d_timers.emplace(::std::chrono::steady_clock::now() + offset, continuation);
    //-dk:TODO possible retrigger poll
}

auto NF::poll_context::do_accept(NF::context::native_handle_type fd,
                                 ::sockaddr*                     address,
                                 ::socklen_t*                    length,
                                 int                             /*flags*/,
                                 NF::context::io_base*           continuation) -> void
{
    this->submit_io(fd, POLLIN, [fd, address, length, continuation]{
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
        case EAGAIN:
        case (EAGAIN != EWOULDBLOCK? EWOULDBLOCK: 0):
         // Already pending errors on the new socket (Linux specific):
        case ENETDOWN:
        case EPROTO:
        case ENOPROTOOPT:
        case EHOSTDOWN:
#ifndef __APPLE__
        case ENONET:
#endif
        case EHOSTUNREACH:
        case EOPNOTSUPP:
        case ENETUNREACH:
            return false;
        }
    },
    continuation);
}

auto NF::poll_context::do_connect(NF::context::native_handle_type fd, ::sockaddr const* address, ::socklen_t length, NF::context::io_base* continuation)
    -> void
{
    fcntl(fd, F_SETFL, O_NONBLOCK);
        auto rc = ::connect(fd, address, length);
    if (0 == rc) {
        continuation->result(0, 0);
    }
    else {
        this->submit_io(fd, POLLIN | POLLOUT, [fd, continuation]{
            int error;
            socklen_t len = sizeof error;

            ::getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len);

            if (error == 0) {
                continuation->result(0, 0);
            }
            else {
                continuation->result(-errno, 0);
            }
            return true;
        },
        continuation);
    }
}

auto NF::poll_context::do_sendmsg(NF::context::native_handle_type fd,
                                  ::msghdr const*                 msg,
                                  int                             flags,
                                  NF::context::io_base*           continuation) -> void
{
    this->submit(fd, POLLOUT, [fd, msg, flags, continuation]{
        auto rc{::sendmsg(fd, msg, flags | MSG_DONTWAIT | MSG_NOSIGNAL)};
        if (0 <= rc) {
           continuation->result(rc, 0);
           return true;
        }
        switch (errno) {
        default:
            continuation->result(-errno, 0);
            return true;
        case EAGAIN:
        case (EAGAIN != EWOULDBLOCK? EWOULDBLOCK: 0):
        case EALREADY:
        case EINTR:
            return false;
        }
    },
    continuation);
}

auto NF::poll_context::do_recvmsg(NF::context::native_handle_type fd,
                                  ::msghdr*                       msg,
                                  int                             flags,
                                  NF::context::io_base*           continuation) -> void
{
    this->submit(fd, POLLIN, [fd, msg, flags, continuation]{
        auto rc{::recvmsg(fd, msg, flags | MSG_DONTWAIT)};
        if (0 <= rc) {
            continuation->result(rc, 0);
            return true;
        }
        switch (errno) {
        default:
            continuation->result(-errno, 0);
            return true;
        case EAGAIN:
        case (EAGAIN != EWOULDBLOCK? EWOULDBLOCK: 0):
        case EINTR:
            return false;
        }
    },
    continuation);
}

auto NF::poll_context::do_read(int fd, ::iovec* vec, ::std::size_t length, NF::context::io_base* continuation) -> void
{
    this->submit(fd, POLLIN, [fd, vec, length, continuation]{
        auto rc{::readv(fd, vec, length)};
        if (0 <= rc) {
            continuation->result(rc, 0);
            return true;
        }
        switch (errno) {
        default:
            continuation->result(-errno, 0);
            return true;
        case EAGAIN:
        case (EAGAIN != EWOULDBLOCK? EWOULDBLOCK: 0):
        case EINTR:
            return false;
        }
    },
    continuation);
}

auto NF::poll_context::do_write(int fd, ::iovec* vec, ::std::size_t length, NF::context::io_base* continuation) -> void
{
    this->submit(fd, POLLOUT, [fd, vec, length, continuation]{
        auto rc{::writev(fd, vec, length)};
        if (0 <= rc) {
            continuation->result(rc, 0);
            return true;
        }
        switch (errno) {
        default:
            continuation->result(-errno, 0);
            return true;
        case EAGAIN:
        case (EAGAIN != EWOULDBLOCK? EWOULDBLOCK: 0):
        case EINTR:
            return false;
        }
    },
    continuation);
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
