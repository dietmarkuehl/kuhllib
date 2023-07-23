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

struct socket
{
    int fd = -1;
    socket(auto&&, int domain, int type, int protocol)
        : socket(::socket(domain, type, protocol)) {
    }
    socket(int fd)
        : fd(fd) {
        if (::fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
            throw std::runtime_error("fcntl");
        }
    }
    socket(socket&& other): fd(std::exchange(other.fd, -1)) {}
    ~socket() { if (fd != -1) ::close(fd); }
};

using file = socket;

// ----------------------------------------------------------------------------

toy::file std_in() {
    return toy::file(0);
}

// ----------------------------------------------------------------------------

namespace hidden::io_operation {
    template <typename State, typename Receiver, bool Timer = false>
    struct stop_callback {
        struct callback {
            State& state;
            void operator()() {
                if constexpr (Timer)
                    state.c.erase_timer(&state);
                else
                    state.c.erase(&state);
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

    enum class event_kind { read, write };

    struct connect_op {
        using result_t = int;
        static constexpr event_kind event = event_kind::write;
        static constexpr char const* name = "connect";

        toy::address address;
        sockaddr const*  addr;
        socklen_t        len;

        int start(auto& state) {
            return ::connect(state.fd, state.address.as_addr(), state.address.len());
        }
        int operator()(auto& state) {
            int         rc{};
            ::socklen_t len{sizeof rc};
            return (::getsockopt(state.fd, SOL_SOCKET, SO_ERROR, &rc, &len) || rc)? -1: rc;
        }
    };

    struct accept_op {
        using result_t = toy::socket;
        static constexpr event_kind event = event_kind::read;
        static constexpr char const* name = "accept";

        int operator()(auto& state) {
            ::sockaddr  addr{};
            ::socklen_t len{sizeof(addr)};
            return ::accept(state.fd, &addr, &len);
        }
    };

    struct read_some_op {
        using result_t = int;
        static constexpr event_kind event = event_kind::read;
        static constexpr char const* name = "read_some";

        char*       buffer;
        std::size_t len;

        int operator()(auto& state) {
            return ::read(state.fd, buffer, len);
        }
    };

    struct write_some_op {
        using result_t = int;
        static constexpr event_kind event = event_kind::write;
        static constexpr char const* name = "write_some";

        char const* buffer;
        std::size_t len;

        int operator()(auto& state) {
            return ::write(state.fd, buffer, len);
        }
    };

    template <typename MBS>
    struct receive_op {
        using result_t = int;
        static constexpr event_kind event = event_kind::read;
        static constexpr char const* name = "receive";

        toy::message_flags flags;
        MBS                buffer;
        ssize_t operator()(auto& state) {
            msghdr msg{
                .msg_name = nullptr,
                .msg_namelen = 0,
                .msg_iov = buffer.data(),
                .msg_iovlen = decltype(std::declval<msghdr>().msg_iovlen)(buffer.size()),
                .msg_control = nullptr,
                .msg_controllen = 0,
                .msg_flags = 0
            };
            return recvmsg(state.fd, &msg, int(flags));
        }
    };

    template <typename MBS>
    struct receive_from_op {
        using result_t = std::size_t;
        static constexpr event_kind event = event_kind::read;
        static constexpr char const* name = "receive_from";

        MBS                buffer;
        toy::address&      addr;
        toy::message_flags flags;
        ::ssize_t operator()(auto& state) {
            msghdr msg{
                .msg_name = &addr.as_addr(),
                .msg_namelen = addr.capacity(),
                .msg_iov = buffer.data(),
                .msg_iovlen = decltype(std::declval<msghdr>().msg_iovlen)(buffer.size()),
                .msg_control = nullptr,
                .msg_controllen = 0,
                .msg_flags = 0
            };
            ::ssize_t rc{recvmsg(state.fd, &msg, int(flags))};
            if (0 <= rc) {
                addr.resize(msg.msg_namelen);
            }
            return rc;
        }
    };

    template <typename MBS>
    struct send_op {
        using result_t = std::size_t;
        static constexpr event_kind event = event_kind::write;
        static constexpr char const* name = "send";

        toy::message_flags flags;
        MBS                buffer;
        ::ssize_t operator()(auto& state) {
            msghdr msg{
                .msg_name = nullptr,
                .msg_namelen = 0,
                .msg_iov = buffer.data(),
                .msg_iovlen = decltype(std::declval<msghdr>().msg_iovlen)(buffer.size()),
                .msg_control = nullptr,
                .msg_controllen = 0,
                .msg_flags = 0
            };
            return sendmsg(state.fd, &msg, int(flags));
        }
    };

    template <typename MBS>
    struct send_to_op {
        using result_t = std::size_t;
        static constexpr event_kind event = event_kind::write;
        static constexpr char const* name = "send_to";

        MBS                buffer;
        toy::address       address;
        toy::message_flags flags;
        ::ssize_t operator()(auto& state) {
            msghdr msg{
                .msg_name = &address.as_addr(),
                .msg_namelen = address.size(),
                .msg_iov = buffer.data(),
                .msg_iovlen = decltype(std::declval<msghdr>().msg_iovlen)(buffer.size()),
                .msg_control = nullptr,
                .msg_controllen = 0,
                .msg_flags = 0
            };
            return sendmsg(state.fd, &msg, int(flags));
        }
    };
}

// ----------------------------------------------------------------------------

}

#endif
