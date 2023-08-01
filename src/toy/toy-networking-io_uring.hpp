// toy-networking-io_uring.hpp                                        -*-C++-*-
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

#ifndef INCLUDED_TOY_NETWORKING_IO_URING
#define INCLUDED_TOY_NETWORKING_IO_URING

#include "toy-networking-common.hpp"
#include "toy-networking-posix.hpp"
#include "toy-starter.hpp"
#include "toy-utility.hpp"

#include <chrono>
#include <tuple>
#include <type_traits>
#include <utility>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <liburing.h>

namespace toy
{

// ----------------------------------------------------------------------------

struct io: immovable {
    virtual ~io() = default;
    virtual std::size_t complete(int) = 0;
};

struct io_context;
struct io_scheduler {
    io_context* context;
};

struct io_context
    : starter<io_scheduler>
    , toy::io_context_base
{
    using scheduler = toy::io_scheduler;
    scheduler get_scheduler() { return { this }; }

    static constexpr bool has_timer = false; //-dk:TODO remove - used while adding timers to contexts
    static constexpr std::size_t entries{256};

    ::io_uring  ring;
    std::size_t outstanding{};

    io_context()
        : starter(scheduler{this}) {
        ::io_uring_queue_init(entries, &ring, 0);
    }
    ~io_context() { ::io_uring_queue_exit(&ring); }

    io_uring_sqe* get_sqe() { return ::io_uring_get_sqe(&ring); }
    void          submit() {
        ::io_uring_submit(&ring); ++outstanding;
    }

    void run() {
        while (outstanding) {
            ::io_uring_cqe* cqe;
            if (::io_uring_wait_cqe(&ring, &cqe))
                throw std::runtime_error("io_uring_wait_cqe");
            outstanding -= reinterpret_cast<io*>(cqe->user_data)->complete(cqe->res);
            ::io_uring_cqe_seen(&ring, cqe);
        }
    }
};

#if 0
//-dk:TODO remove
struct socket {
    int fd;

    socket(int fd): fd(fd) {}
    socket(socket&& other): fd(std::exchange(other.fd, -1)) {}
    ~socket() { if (0 <= fd) ::close(fd); }
};
#endif

// ----------------------------------------------------------------------------

namespace hidden_io_op {
    struct cancel_state: io {
        io*  completion{nullptr};
        int  res{};
        std::size_t complete(int) override final {
            return 1u + std::exchange(completion, nullptr)->complete(0);
        }
    };

    template <typename Result, typename Submit, typename... Args>
    struct io_op {
        using result_t = Result;

        template <typename Receiver>
        struct state: io {
            [[no_unique_address]] Submit const submit;
            std::remove_cvref_t<Receiver>      receiver;
            io_context&                        context;
            int                                fd;
            std::tuple<Args...>                args;
            cancel_state                       cancel;
            struct callback {
                state& s;
                void operator()() {
                    s.cancel.completion = &s;
                    auto& context = *get_scheduler(s.receiver).context;
                    ::io_uring_sqe* sqe{ context.get_sqe() };
                    io_uring_prep_cancel(sqe, &s, unsigned());
                    sqe->user_data = reinterpret_cast<std::uint64_t>(&s.cancel);
                    context.submit();
                }
            };
            using stop_token = decltype(get_stop_token(std::declval<Receiver>()));
            std::optional<typename stop_token::template callback_type<callback>> cb;

            template <typename R, typename A>
            state(R&& receiver, int fd, A&& a)
                : submit()
                , receiver(std::forward<R>(receiver))
                , context(*get_scheduler(receiver).context)
                , fd(fd)
                , args(std::forward<A>(a)) {
            }
            friend void start(state& self) {
                self.cb.emplace(get_stop_token(self.receiver), callback{self});
                ::io_uring_sqe* sqe{ self.context.get_sqe() };
                self.submit(sqe, self.fd, self.args);
                sqe->user_data = reinterpret_cast<std::uint64_t>(&self);
                self.context.submit();
            }
            std::size_t complete(int res) override {
                cb.reset();
                if (cancel.completion) {
                    cancel.res = res;
                    return 0u;
                }
                else if (0 <= res) {
                    if constexpr (requires(Submit const& op, state& s) { op.finalize(s); }) {
                        submit.finalize(args);
                    }
                    if constexpr (requires(io_context_base& context, decltype(res) res){ result_t(context, res); }) {
                        set_value(std::move(receiver), result_t(context, res));
                    }
                    else {
                        set_value(std::move(receiver), result_t(res));
                    }
                }
                else  {
                    set_error(std::move(receiver), std::make_exception_ptr(std::system_error(-res, std::system_category())));
                }
                return true;
            }
        };

        int                 fd;
        std::tuple<Args...> args;

        template <typename... A>
        io_op(socket& sock, A&&... args)
            : fd(sock.fd())
            , args(args...) {
        }

        template <typename R>
        friend state<R> connect(io_op const& self, R&& receiver) {
            return state<R>(std::forward<R>(receiver), self.fd, self.args);
        }
    };
}

using async_accept_op = decltype([](auto sqe, int fd, auto& state){
    ::io_uring_prep_accept(sqe, fd, reinterpret_cast<::sockaddr*>(&std::get<0>(state)), &std::get<1>(state), 0);
});
using async_accept = hidden_io_op::io_op<socket, async_accept_op, ::sockaddr_in, ::socklen_t>;

using async_connect_op = decltype([](auto sqe, int fd, auto& state){
    ::io_uring_prep_connect(sqe, fd, reinterpret_cast<::sockaddr const*>(std::get<0>(state)), std::get<1>(state));
});
using async_connect = hidden_io_op::io_op<int, async_connect_op, ::sockaddr const*, ::socklen_t>;

using async_read_some_op = decltype([](auto sqe, int fd, auto& state){
    ::io_uring_prep_read(sqe, fd, std::get<0>(state), std::get<1>(state), 0);
});
using async_read_some = hidden_io_op::io_op<int, async_read_some_op, char*, std::size_t>;

using async_write_some_op = decltype([](auto sqe, int fd, auto& state){
    ::io_uring_prep_write(sqe, fd, std::get<0>(state), std::get<1>(state), 0);
});
using async_write_some = hidden_io_op::io_op<int, async_write_some_op, char const*, std::size_t>;

using async_receive_op = decltype([](auto sqe, int fd, auto& state){
    std::get<1>(state).msg_iov = const_cast<iovec*>(std::get<0>(state).data());
    std::get<1>(state).msg_iovlen = decltype(std::declval<msghdr>().msg_iovlen)(std::get<0>(state).size());
    ::io_uring_prep_recvmsg(sqe, fd, &std::get<1>(state), int(std::get<2>(state)));
});
template <typename MBS>
using async_receive_t = hidden_io_op::io_op<int, async_receive_op, MBS, msghdr, toy::message_flags>;
template <typename MBS>
async_receive_t<MBS> async_receive(toy::socket& s, toy::message_flags f, const MBS& b) {
    return {s, b, msghdr{}, f};
}
template <typename MBS>
async_receive_t<MBS> async_receive(toy::socket& s, const MBS& b) {
    return async_receive(s, toy::message_flags{}, b);
}

struct async_receive_from_op
{
    void operator()(auto sqe, int fd, auto& state) const {
        std::get<2>(state).msg_name = &std::get<1>(state)->as_addr();
        std::get<2>(state).msg_namelen = std::get<1>(state)->size();
        std::get<2>(state).msg_iov = const_cast<iovec*>(std::get<0>(state).data());
        std::get<2>(state).msg_iovlen = decltype(std::declval<msghdr>().msg_iovlen)(std::get<0>(state).size());
        ::io_uring_prep_recvmsg(sqe, fd, &std::get<2>(state), int(std::get<3>(state)));
    }
    void finalize(auto& state) const {
        std::get<1>(state)->resize(std::get<2>(state).msg_namelen);
    }
};
template <typename MBS>
using async_receive_from_t = hidden_io_op::io_op<int, async_receive_from_op, MBS, toy::address*, msghdr, toy::message_flags>;
template <typename MBS>
async_receive_from_t<MBS> async_receive_from(toy::socket& s, toy::message_flags f, const MBS& b, toy::address& addr) {
    return {s, b, &addr, msghdr{}, f};
}
template <typename MBS>
async_receive_from_t<MBS> async_receive_from(toy::socket& s, const MBS& b, toy::address& addr) {
    return async_receive_from(s, toy::message_flags{}, b, addr);
}

using async_send_op = decltype([](auto sqe, int fd, auto& state){
    std::get<1>(state).msg_iov = const_cast<iovec*>(std::get<0>(state).data());
    std::get<1>(state).msg_iovlen = decltype(std::declval<msghdr>().msg_iovlen)(std::get<0>(state).size());
    ::io_uring_prep_sendmsg(sqe, fd, &std::get<1>(state), int(std::get<2>(state)));
});
template <typename MBS>
using async_send_t = hidden_io_op::io_op<int, async_send_op, MBS, msghdr, toy::message_flags>;
template <typename MBS>
async_send_t<MBS> async_send(toy::socket& s, toy::message_flags f, const MBS& b) {
    return {s, b, msghdr{}, f};
}
template <typename MBS>
async_send_t<MBS> async_send(toy::socket& s, const MBS& b) {
    return async_send(s, toy::message_flags{}, b);
}

using async_send_to_op = decltype([](auto sqe, int fd, auto& state){
    std::get<2>(state).msg_name = &std::get<1>(state).as_addr();
    std::get<2>(state).msg_namelen = std::get<1>(state).size();
    std::get<2>(state).msg_iov = const_cast<iovec*>(std::get<0>(state).data());
    std::get<2>(state).msg_iovlen = decltype(std::declval<msghdr>().msg_iovlen)(std::get<0>(state).size());
    ::io_uring_prep_sendmsg(sqe, fd, &std::get<2>(state), int(std::get<3>(state)));
});
template <typename MBS>
using async_send_to_t = hidden_io_op::io_op<int, async_send_to_op, MBS, toy::address, msghdr, toy::message_flags>;
template <typename MBS>
async_send_to_t<MBS> async_send_to(toy::socket& s, const MBS& b, toy::address addr, toy::message_flags f) {
    return {s, b, addr, msghdr{}, f};
}
template <typename MBS>
async_send_to_t<MBS> async_send_to(toy::socket& s, const MBS& b, toy::address addr) {
    return async_send_to(s, b, addr, toy::message_flags{});
}

// ----------------------------------------------------------------------------

namespace hidden_async_sleep_for {
    struct async_sleep_for {
        using result_t = none;

	    std::chrono::milliseconds duration;
        template <typename Receiver>
        struct state
            : io {
            Receiver                   receiver;
	        std::chrono::milliseconds  duration;
            __kernel_timespec          spec;
            hidden_io_op::cancel_state cancel;
            struct callback {
                state& s;
                void operator()() {
                    s.cancel.completion = &s;
                    auto& context = *get_scheduler(s.receiver).context;
                    ::io_uring_sqe* sqe{ context.get_sqe() };
                    io_uring_prep_timeout_remove(sqe, reinterpret_cast<std::uint64_t>(&s), unsigned());
                    sqe->user_data = reinterpret_cast<std::uint64_t>(&s.cancel);
                    context.submit();
                }
            };
            using stop_token = decltype(get_stop_token(std::declval<Receiver>()));
            std::optional<typename stop_token::template callback_type<callback>> cb;

            template <typename R>
            state(R&& receiver, std::chrono::milliseconds duration)
                : receiver(receiver)
                , duration(duration) {
            }
            friend void start(state& self) {
                self.cb.emplace(get_stop_token(self.receiver), callback{self});
                auto& context = *get_scheduler(self.receiver).context;
                ::io_uring_sqe* sqe{ context.get_sqe() };
                self.spec.tv_sec = self.duration.count() / 1000;
                self.spec.tv_nsec = (self.duration.count() % 1000) * 1000000;
                io_uring_prep_timeout(sqe, &self.spec, ~unsigned(), unsigned());
                sqe->user_data = reinterpret_cast<std::uint64_t>(&self);
                context.submit();
            }
            std::size_t complete(int) override {
                cb.reset();
                if (!cancel.completion) {
                    set_value(std::move(receiver), none{});
                    return 1u;
                }
                return 0u;
            }
        };
        template <typename R>
        friend state<R> connect(async_sleep_for self, R receiver) {
            return state<R>(receiver, self.duration);
        }
    };
}
using async_sleep_for = hidden_async_sleep_for::async_sleep_for;

// ----------------------------------------------------------------------------

}

#endif
