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

#include <array>
#include <concepts>
#include <iostream> //-dk:TODO remove
#include <optional>
#include <cstddef>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>

// ----------------------------------------------------------------------------

namespace toy {

// ----------------------------------------------------------------------------

enum class message_flags {
};

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------

template <typename B>
concept byte_type
    =   std::same_as<std::remove_cv_t<B>, char>
    ||  std::same_as<std::remove_cv_t<B>, signed char>
    ||  std::same_as<std::remove_cv_t<B>, unsigned char>
    ||  std::same_as<std::remove_cv_t<B>, std::byte>
    ;
template <typename B>
concept byte_array
    =   std::is_bounded_array_v<std::remove_cvref_t<B>>
    &&  toy::byte_type<std::remove_extent_t<std::remove_cvref_t<B>>>
    ;

template <toy::byte_array... B>
std::array<::iovec, sizeof...(B)> buffer(B&&... b) {
    return std::array<::iovec, sizeof...(B)>{
        ::iovec{
            .iov_base = const_cast<char*>(reinterpret_cast<char const*>(b)),
            .iov_len = ::std::extent_v<std::remove_cvref_t<B>>
        }...
    };
}
template <toy::byte_type B>
std::array<::iovec, 1> buffer(B* b, std::size_t n) {
    return std::array<::iovec, 1>{
        ::iovec{
            .iov_base = const_cast<char*>(reinterpret_cast<char const*>(b)),
            .iov_len  = n
        }
    };
}

// ----------------------------------------------------------------------------

struct socket;

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

        //toy::address address;
        sockaddr const*  addr;
        socklen_t        len;

        int start(auto& state) {
            return ::connect(state.fd, addr, len);
            //return ::connect(state.fd, state.address.as_addr(), state.address.len());
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
        using result_t = std::size_t;
        static constexpr event_kind event = event_kind::read;
        static constexpr char const* name = "receive";

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
