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

struct io_context_base {
    io_context_base() = default;
    io_context_base(io_context_base&&) = delete;
    ~io_context_base() = default;
    virtual int make_socket(int domain, int type, int protocol)
    {
        return ::socket(domain, type, protocol);
    }
    virtual int make_fd(int fd) { return fd; }
    virtual int fd(int id) { return id; }

    virtual int close(int fd)   { return ::close(fd); }
    virtual void fcntl(int fd, int cmd, int flag)
    {
        if (::fcntl(fd, cmd, flag) < 0) {
            throw std::runtime_error("fcntl");
        }
    }
};

// ----------------------------------------------------------------------------

struct socket
{
private:
    enum class tag {};
    io_context_base* d_context;
    int              d_id;

    socket(io_context_base& context, int id, tag)
        : d_context(&context)
        , d_id(id)
    {
        context.fcntl(fd(), F_SETFL, O_NONBLOCK);
    }
public:
    socket(io_context_base& context, int domain, int type, int protocol)
        : socket(context, context.make_socket(domain, type, protocol), tag{})
    {
    }
    socket(io_context_base& context, int fd)
        : socket(context, context.make_fd(fd), tag{})
    {
    }
    socket(socket&& other)
        : d_context(std::exchange(other.d_context, nullptr))
        , d_id(std::exchange(other.d_id, -1))
    {
    }
    ~socket()
    {
        if (d_id != -1) {
            d_context->close(d_id);
        }
    }
    int                   fd() const { return d_context->fd(d_id); }
    int                   id() const { return d_id; }
    toy::io_context_base* context() const { return d_context; }
};

using file = socket;

// ----------------------------------------------------------------------------

struct std_context: io_context_base {};
toy::file& std_in() {
    static std_context context;
    static toy::file rc(context, 0);
    return rc;
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

    enum class event_kind { none = 0x0, read = 0x1, write = 0x2, both = 0x3 };
    event_kind operator| (event_kind a, event_kind b) {
        return event_kind(int(a) | int(b));
    }
    event_kind operator& (event_kind a, event_kind b) {
        return event_kind(int(a) & int(b));
    }
    std::ostream& operator<<(std::ostream& out, event_kind event) {
        switch (event) {
            default: return out << "unknown";
            case event_kind::none:  return out << "none";
            case event_kind::read:  return out << "read";
            case event_kind::write: return out << "write";
            case event_kind::both:  return out << "both";
        }
    }

    struct poll_op {
        using result_t = event_kind;
        event_kind event{};
        static constexpr char const* name = "poll";

        event_kind start(auto&) {
            return event_kind::none;
        }
        event_kind operator()(auto&, event_kind revents) {
            return revents;
        }
    };

    struct connect_op {
        using result_t = int;
        static constexpr event_kind event = event_kind::write;
        static constexpr char const* name = "connect";

        toy::address address;

        int start(auto& state) {
            return ::connect(state.fd, &state.op.address.as_addr(), state.op.address.size());
        }
        int operator()(auto& state, event_kind) {
            int         rc{};
            ::socklen_t len{sizeof rc};
            return (::getsockopt(state.fd, SOL_SOCKET, SO_ERROR, &rc, &len) || rc)? -1: rc;
        }
    };

    struct accept_op {
        using result_t = toy::socket;
        static constexpr event_kind event = event_kind::read;
        static constexpr char const* name = "accept";

        int operator()(auto& state, event_kind) {
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

        int operator()(auto& state, event_kind) {
            return ::read(state.fd, buffer, len);
        }
    };

    struct write_some_op {
        using result_t = int;
        static constexpr event_kind event = event_kind::write;
        static constexpr char const* name = "write_some";

        char const* buffer;
        std::size_t len;

        int operator()(auto& state, event_kind) {
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
        ssize_t operator()(auto& state, event_kind) {
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
        ::ssize_t operator()(auto& state, event_kind) {
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
        ::ssize_t operator()(auto& state, event_kind) {
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
        ::ssize_t operator()(auto& state, event_kind) {
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
