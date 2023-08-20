// toy-posix.hpp                                                      -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2023 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#ifndef INCLUDED_TOY_POSIX
#define INCLUDED_TOY_POSIX

#include "toy-stop_token.hpp"
#include "toy-networking-sender.hpp"
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <string>
#include <cstring>
#include <utility>

namespace toy {

// ----------------------------------------------------------------------------

std::string strerror(int error) {
    return std::strerror(error);
}

// ----------------------------------------------------------------------------

toy::file std_in(toy::io_context_base& context) {
    return toy::file(context, 0);
}

// ----------------------------------------------------------------------------

namespace hidden::io_operation
{
    template <typename State, typename Receiver, bool Timer = false>
    struct stop_callback {
        struct callback {
            State& state;
            void operator()() {
                if constexpr (Timer)
                    state.scheduler().erase_timer(&state);
                else
                    state.scheduler().erase(&state);
                state.cb.disengage();
                set_stopped(std::move(state.receiver));
            }
        };
        using stop_token = decltype(get_stop_token(std::declval<Receiver>()));
        using token_callback = typename stop_token::template callback_type<callback>; 
            
        std::optional<token_callback> cb;
        void engage(State& state) {
            cb.emplace(get_stop_token(state.receiver), callback{state});
        }
        void disengage() {
            cb.reset();
        }
    };
}

// ----------------------------------------------------------------------------

namespace hidden::io_operation
{
    toy::event_kind start(toy::socket&, toy::poll_t::args&)
    {
        errno = EAGAIN;
        return toy::event_kind::none;
    }
    toy::event_kind operation(toy::socket&, toy::event_kind revents, toy::poll_t::args&)
    {
        return revents;
    }

    int start(toy::socket& socket, toy::connect_t::args& args)
    {
        return ::connect(socket.fd(), &args.address.as_addr(), args.address.size());
    }
    int operation(toy::socket& socket, toy::event_kind, toy::connect_t::args&)
    {
        int         rc{};
        ::socklen_t len{sizeof rc};
        if (::getsockopt(socket.fd(), SOL_SOCKET, SO_ERROR, &rc, &len)) {
            return -1;
        }
        else {
            errno = rc;
            return -rc;
        }
    }

    int operation(toy::socket& socket, toy::event_kind, toy::accept_t::args&)
    {
        ::sockaddr  addr{};
        ::socklen_t len{sizeof(addr)};
        return ::accept(socket.fd(), &addr, &len);
    }

    int operation(toy::socket& socket, toy::event_kind, toy::read_some_t::args& args)
    {
        return ::read(socket.fd(), args.buffer, args.len);
    }

    int operation(toy::socket& socket, toy::event_kind, toy::write_some_t::args& args) {
        return ::write(socket.fd(), args.buffer, args.len);
    }

    template <typename MBS>
    ssize_t operation(toy::socket& socket, toy::event_kind, toy::receive_t::args<MBS>& args) {
        msghdr msg{
            .msg_name = nullptr,
            .msg_namelen = 0,
            .msg_iov = args.buffer.data(),
            .msg_iovlen = decltype(std::declval<msghdr>().msg_iovlen)(args.buffer.size()),
            .msg_control = nullptr,
            .msg_controllen = 0,
            .msg_flags = 0
        };
        return recvmsg(socket.fd(), &msg, int(args.flags));
    }

    template <typename MBS>
    ::ssize_t operation(toy::socket& socket, toy::event_kind, toy::receive_from_t::args<MBS>& args) {
        msghdr msg{
            .msg_name = &args.addr.as_addr(),
            .msg_namelen = args.addr.capacity(),
            .msg_iov = args.buffer.data(),
            .msg_iovlen = decltype(std::declval<msghdr>().msg_iovlen)(args.buffer.size()),
            .msg_control = nullptr,
            .msg_controllen = 0,
            .msg_flags = 0
        };
        ::ssize_t rc{recvmsg(socket.fd(), &msg, int(args.flags))};
        if (0 <= rc) {
            args.addr.resize(msg.msg_namelen);
        }
        return rc;
    }

    template <typename MBS>
    ::ssize_t operation(toy::socket& socket, toy::event_kind, toy::send_t::args<MBS>& args) {
        msghdr msg{
            .msg_name = nullptr,
            .msg_namelen = 0,
            .msg_iov = args.buffer.data(),
            .msg_iovlen = decltype(std::declval<msghdr>().msg_iovlen)(args.buffer.size()),
            .msg_control = nullptr,
            .msg_controllen = 0,
            .msg_flags = 0
        };
        return sendmsg(socket.fd(), &msg, int(args.flags));
    }

    template <typename MBS>
    ::ssize_t operation(toy::socket& socket, toy::event_kind, toy::send_to_t::args<MBS>& args) {
        msghdr msg{
            .msg_name = &args.address.as_addr(),
            .msg_namelen = args.address.size(),
            .msg_iov = args.buffer.data(),
            .msg_iovlen = decltype(std::declval<msghdr>().msg_iovlen)(args.buffer.size()),
            .msg_control = nullptr,
            .msg_controllen = 0,
            .msg_flags = 0
        };
        return sendmsg(socket.fd(), &msg, int(args.flags));
    }
}

// ----------------------------------------------------------------------------
}


#endif
