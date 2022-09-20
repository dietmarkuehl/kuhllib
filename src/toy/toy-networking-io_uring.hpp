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

#include "toy-starter.hpp"
#include "toy-utility.hpp"

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
    virtual void complete(int) = 0;
};

struct io_context
    : starter
{
    static constexpr bool has_timer = false; //-dk:TODO remove - used while adding timers to contexts
    static constexpr std::size_t entries{256};

    ::io_uring  ring;
    std::size_t outstanding{};

    io_context()  { ::io_uring_queue_init(entries, &ring, 0); }
    ~io_context() { ::io_uring_queue_exit(&ring); }

    io_uring_sqe* get_sqe() { return ::io_uring_get_sqe(&ring); }
    void          submit() { ::io_uring_submit(&ring); ++outstanding; }

    void run() {
        while (outstanding) {
            ::io_uring_cqe* cqe;
            if (::io_uring_wait_cqe(&ring, &cqe))
                throw std::runtime_error("io_uring_wait_cqe");
            reinterpret_cast<io*>(cqe->user_data)->complete(cqe->res);
            ::io_uring_cqe_seen(&ring, cqe);
            --outstanding;
        }
    }
};

struct socket {
    int fd;

    socket(int fd): fd(fd) {}
    socket(socket&& other): fd(std::exchange(other.fd, -1)) {}
    ~socket() { if (0 <= fd) ::close(fd); }
};

// ----------------------------------------------------------------------------

namespace hidden_io_op {
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

            template <typename R, typename A>
            state(R&& receiver, io_context& context, int fd, A&& a)
                : submit()
                , receiver(std::forward<R>(receiver))
                , context(context)
                , fd(fd)
                , args(std::forward<A>(a)) {
            }
            friend void start(state& self) {
                ::io_uring_sqe* sqe{ self.context.get_sqe() };
                self.submit(sqe, self.fd, self.args);
                sqe->user_data = reinterpret_cast<std::uint64_t>(&self);
                self.context.submit();
            }
            void complete(int fd) override {
                if (0 <= fd)
                    set_value(receiver, result_t(fd));
                else 
                    set_error(receiver, std::make_exception_ptr(std::runtime_error("accept")));
            }
        };

        io_context&         context;
        int                 fd;
        std::tuple<Args...> args;

        template <typename... A>
        io_op(io_context& context, socket& sock, A&&... args)
            : context(context)
            , fd(sock.fd)
            , args(args...) {
        }

        template <typename R>
        friend state<R> connect(io_op const& self, R&& receiver) {
            return state<R>(std::forward<R>(receiver), self.context, self.fd, self.args);
        }
    };
}

using async_accept_op = decltype([](auto sqe, int fd, auto& state){
    ::io_uring_prep_accept(sqe, fd, reinterpret_cast<sockaddr*>(&std::get<0>(state)), &std::get<1>(state), 0);
});
using async_accept = hidden_io_op::io_op<socket, async_accept_op, ::sockaddr_in, ::socklen_t>;

using async_connect_op = decltype([](auto sqe, int fd, auto& state){
    ::io_uring_prep_connect(sqe, fd, std::get<0>(state), std::get<1>(state));
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

// ----------------------------------------------------------------------------

namespace hidden_async_sleep_for {
    struct async_sleep_for {
        using result_t = none;
        struct state {
            friend void start(state&) {}
        };
        friend state connect(async_sleep_for, auto) {
            return {};
        }
    };
}
using async_sleep_for = hidden_async_sleep_for::async_sleep_for;

// ----------------------------------------------------------------------------

}

#endif
