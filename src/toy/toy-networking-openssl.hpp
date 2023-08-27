// toy-networking-openssl.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_TOY_NETWORKING_OPENSSL
#define INCLUDED_TOY_NETWORKING_OPENSSL

// ----------------------------------------------------------------------------

#include "toy-networking-common.hpp"
#include "toy-networking-sender.hpp"
#include "toy-starter.hpp"
#include "toy-utility.hpp"

#include <algorithm>
#include <chrono>
#include <optional>
#include <queue>
#include <stdexcept>
#include <system_error>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <stddef.h>
#include <string.h>

#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace toy::openssl
{

// ----------------------------------------------------------------------------

struct xsocket
{
    static SSL_CTX* context() {
        static SSL_CTX* rc = []{
            SSL_library_init();
            SSL_load_error_strings();
            SSL_CTX* rc = SSL_CTX_new(TLS_method());

            if (SSL_CTX_use_certificate_file(rc, "tmp/cert.pem", SSL_FILETYPE_PEM) != 1) {
                throw std::runtime_error("failed to use certificated");
            }
            if (SSL_CTX_use_PrivateKey_file(rc, "tmp/cert.pem", SSL_FILETYPE_PEM) != 1) {
                throw std::runtime_error("failed to use private key");
            }
            return rc;
        }();
        return rc;
    }
    int  fd = -1;
    private:
    SSL* ssl{nullptr};
    public:
    xsocket() = default;
    xsocket(int fd)
        : fd(fd)
    {
        if (::fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
            throw std::runtime_error("fcntl");
        }
    }
    void make_ssl() {
        auto ctx = context();
        ssl = SSL_new(ctx);
        if (!ssl) {
            throw std::runtime_error("failed to create SSL socket");
        }
        if (!SSL_set_fd(ssl, fd)) {
            throw std::runtime_error("failed to set file descriptor for SSL socket");
        }
    }
    SSL* get_ssl() {
        if (!ssl) {
            make_ssl();
        }
        return ssl;
    }
    xsocket(xsocket&& other)
        : fd(std::exchange(other.fd, -1))
        , ssl(std::exchange(other.ssl, nullptr))
    {
    }
    xsocket& operator= (xsocket&& other) {
        if (this != &other) {
            std::swap(fd, other.fd);
            std::swap(ssl, other.ssl);
        }
        return *this;
    }
    ~xsocket() {
        if (ssl != nullptr) {
            SSL_free(ssl);
        }
        if (fd != -1) ::close(fd);
    }
    friend std::ostream& operator<< (std::ostream& out, xsocket const& self) {
        return out << "fd=" << self.fd << ", ssl=" << self.ssl;
    }
};

class context_base;
template <typename BaseContext = int>
class context;
struct scheduler;

struct io_base
    : toy::immovable {
    int         fd;
    short int   events;
    virtual void complete() = 0;
    io_base(int fd, short int events): fd(fd), events(events)  {}
};

template <typename Receiver, typename Operation>
struct io_state
    : toy::openssl::io_base
{
    Receiver receiver;
    io_state(Receiver receiver, toy::socket&, toy::event_kind, Operation)
        : io_base(0, 0)
        , receiver(receiver)
    {
    }
    friend void start(io_state& self)
    {
        set_error(std::move(self.receiver),
                  std::make_exception_ptr(std::runtime_error("openssl " + std::string(Operation::name) + " is not, yet, implemented")));
    }
    void complete() override {}
};

template <typename Receiver>
struct io_state<Receiver, toy::io::connect_t::args>
    : toy::openssl::io_base
{
    Receiver receiver;
    io_state(Receiver receiver, toy::socket&, toy::event_kind, toy::io::connect_t::args)
        : io_base(0, 0)
        , receiver(receiver)
    {
    }
    friend void start(io_state& self)
    {
        set_error(std::move(self.receiver),
                  std::make_exception_ptr(std::runtime_error("openssl connect is being implemented")));
    }
    void complete() override {}
};

class context_base
    : public toy::io_context_base
{
};

struct scheduler {
    template <typename Receiver, typename Operation, typename Stream>
    using io_state = toy::openssl::io_state<Receiver, Operation>;

    context_base* context;
    friend std::ostream& operator<< (std::ostream& out, scheduler const& s) {
        return out << s.context;
    }
};

template <typename>
class context
    : public starter<toy::openssl::scheduler>
    , public toy::openssl::context_base
{
public:
    using scheduler = toy::openssl::scheduler;
    scheduler get_scheduler() { return { this }; }

private:
    using time_point_t = std::chrono::system_clock::time_point;
    std::vector<toy::openssl::io_base*>      ios;
    std::vector<::pollfd> fds;
    toy::timer_queue<toy::openssl::io_base*> times;

public:
    static constexpr bool has_timer = true; //-dk:TODO remove - used while adding timers to contexts

    context()
        : starter(get_scheduler()) {
    }
    void add(toy::openssl::io_base* i) {
        ios.push_back(i);
        fds.push_back(  pollfd{ .fd = i->fd, .events = i->events });
    }
    void add(time_point_t time, toy::openssl::io_base* op) {
        times.push(time, op);
    }
    void erase(toy::openssl::io_base* i) {
        auto it = std::find(ios.begin(), ios.end(), i);
        if (it != ios.end()) {
            fds.erase(fds.begin() + (it - ios.begin()));
            ios.erase(it);
        }
    }
    void erase_timer(toy::openssl::io_base* i) {
        times.erase(i);
    }
    void run() {
        while ( (!ios.empty() || not times.empty())) { 
            auto now{std::chrono::system_clock::now()};

            bool timed{false};
            while (!times.empty() && times.top().first <= now) {
                toy::openssl::io_base* op{times.top().second};
                times.pop();
                op->complete();
                timed = true;
            }
            if (timed) {
                continue;
            }
            auto time{times.empty()
                     ? -1
                : std::chrono::duration_cast<std::chrono::milliseconds>(times.top().first - now).count()};
            if (0 < ::poll(fds.data(), fds.size(), time)) {
                for (size_t i = fds.size(); i--; )
                    // The check for i < fds.size() is added as complete() may
                    // cause elements to get canceled and be removed from the
                    // list.
                    if (i < fds.size() && fds[i].events & fds[i].revents) {
                        fds[i] = fds.back();
                        fds.pop_back();
                        auto c = std::exchange(ios[i], ios.back());
                        ios.pop_back();
                        c->complete();
                    }
            }
        }
    }
};

// ----------------------------------------------------------------------------

struct ssl_error_category
    : std::error_category {
    ssl_error_category(): std::error_category() {}
    char const* name() const noexcept override { return "ssl"; }
    std::string message(int err) const override {
        char error[256];
        ERR_error_string_n(err, error, sizeof(error));
        return error;
    }
};

ssl_error_category const& ssl_category() {
    static ssl_error_category rc{};
    return rc;
}

// ----------------------------------------------------------------------------

#if 0
namespace hidden_async_accept {
    struct sender {
        using result_t = toy::socket;

        socket& d_socket;
        sender(socket& s): d_socket(s) {}

        template <typename R>
        struct state: io {
            socket&            d_socket;
            ::sockaddr_storage addr{};
            ::socklen_t        len{sizeof(::sockaddr_storage)};
            bool               shaking_hands{false};
            socket             d_result;
            R                  receiver;
            hidden::io_operation::stop_callback<state, R> cb;

            state(socket& s, R r)
                : io(*get_scheduler(r).context, s.fd, POLLIN)
                , d_socket(s)
                , receiver(r) {
            }
            friend void start(state& self) { self.try_accept_or_submit(); }
            void submit() {
                cb.engage(*this);
                c.add(this);
            }
            void try_accept_or_submit() {
                int rc(::accept(fd, reinterpret_cast<::sockaddr*>(&addr), &len));
                if (0 <= rc) {
                    d_result = toy::openssl::xsocket(rc);
                    auto ssl = d_result.get_ssl();
                    SSL_set_accept_state(ssl);
                    shaking_hands = true;
                    this->io::fd = rc;
                    ssl_accept();
                }
                else if (errno == EINTR || errno == EWOULDBLOCK) {
                    submit();
                }
                else {
                    set_error(receiver, std::make_exception_ptr(std::runtime_error(std::string("accept: ") + ::strerror(errno))));
                }
            }
            void complete() override final {
                cb.disengage();

                if (shaking_hands) {
                    ssl_accept();
                }
                else {
                    try_accept_or_submit();
                }
            }
            void ssl_accept() {
                ERR_clear_error();
                auto ssl = d_result.get_ssl();
                switch (int ret = SSL_accept(ssl)) {
                default:
                    switch (int error = SSL_get_error(ssl, ret)) {
                        default:
                            set_error(receiver, std::make_exception_ptr(std::system_error(error, ssl_category())));
                            break;
                        case SSL_ERROR_WANT_READ:
                            ERR_get_error();
                            this->events = POLLIN;
                            submit();
                            break;
                        case SSL_ERROR_WANT_WRITE:
                            ERR_get_error();
                            this->events = POLLOUT;
                            submit();
                            break;
                    }
                    break;
                case 1: // success
                    set_value(receiver, std::move(d_result));
                    break;
                case 0:
                    set_error(receiver, std::make_exception_ptr(std::system_error(SSL_get_error(ssl, ret), ssl_category())));
                    break;
                }
            }
        };

        template <typename R>
        friend state<R> connect(sender const& self, R r) {
            return state<R>(self.d_socket, r);
        }
    };
}
using async_accept = hidden_async_accept::sender;

// ----------------------------------------------------------------------------

namespace hidden_async_connect {
    struct sender {
        using result_t = int;

        socket&            d_socket;
        ::sockaddr const*  addr;
        ::socklen_t        len;

        sender(socket& s, auto addr, auto len): d_socket(s), addr(addr), len(len) {}

        template <typename R>
        struct state: io {
            socket&            d_socket;
            ::sockaddr const*  addr;
            ::socklen_t        len;
            bool               shaking_hands{false};
            R                  receiver;
            hidden::io_operation::stop_callback<state, R> cb;

            state(socket& s, auto addr, auto len, R r)
                : io(*get_scheduler(r).context, s.fd, POLLOUT)
                , d_socket(s)
                , addr(addr)
                , len(len)
                , receiver(r) {
            }
            friend void start(state& self) {
                if (0 <= ::connect(self.fd, self.addr, self.len)) {
                    // immediate connect completed
                    self.connect();
                }
                else if (errno == EAGAIN || errno == EINPROGRESS) {
                    self.cb.engage(self);
                    self.c.add(&self);
                }
                else {
                    set_error(self.receiver, std::make_exception_ptr(std::runtime_error(std::string("connect: ") + ::strerror(errno))));
                }
            }
            void complete() override final {
                if (shaking_hands) {
                    connect();
                    return;
                }

                cb.disengage();
                int         rc{};
                ::socklen_t len{sizeof rc};
                if (::getsockopt(fd, SOL_SOCKET, SO_ERROR, &rc, &len)) {
                    set_error(receiver, std::make_exception_ptr(std::runtime_error(std::string("getsockopt: ") + ::strerror(errno))));
                }
                else if (rc) {
                    set_error(receiver, std::make_exception_ptr(std::runtime_error(std::string("connect: ") + ::strerror(rc))));
                }
                else {
                    this->connect();
                }
            }
            void connect() {
                auto ssl = d_socket.get_ssl();
                if (not shaking_hands) {
                    SSL_set_connect_state(ssl);
                    shaking_hands = true;
                }
                switch (int ret = SSL_connect(ssl)) {
                default:
                    switch (int error = SSL_get_error(ssl, ret)) {
                        default:
                            set_error(receiver, std::make_exception_ptr(std::system_error(error, ssl_category())));
                            break;
                        case SSL_ERROR_WANT_READ:
                            this->events = POLLIN;
                            this->cb.engage(*this);
                            this->c.add(this);
                            break;
                        case SSL_ERROR_WANT_WRITE:
                            this->events = POLLOUT;
                            this->cb.engage(*this);
                            this->c.add(this);
                            break;
                    }
                    break;
                case 1: // success
                    set_value(receiver, ret);
                    break;
                case 0:
                    set_error(receiver, std::make_exception_ptr(std::system_error(SSL_get_error(ssl, ret), ssl_category())));
                    break;
                }
            }
        };

        template <typename R>
        friend state<R> connect(sender const& self, R r) {
            return state<R>(self.d_socket, self.addr, self.len, r);
        }
    };
}
using async_connect = hidden_async_connect::sender;

// ----------------------------------------------------------------------------

namespace hidden_async_write_some {
    struct sender {
        using result_t = int;

        toy::socket&  socket;
        char const*   buffer;
        std::size_t   len;

        sender(toy::socket& socket, char const* buffer, std::size_t len)
            : socket(socket)
            , buffer(buffer)
            , len(len) {
        }
        sender(toy::socket& socket, std::array<::iovec, 1> const& buffer)
            : socket(socket)
            , buffer(static_cast<char*>(buffer[0].iov_base))
            , len(buffer[0].iov_len)
        {
        }

        template <typename R>
        struct state
            : toy::io
        {
            toy::socket&                                  d_socket;
            R                                             receiver;
            char const*                                   buffer;
            std::size_t                                   len;
            hidden::io_operation::stop_callback<state, R> cb;

            state(R            receiver,
                  toy::socket& socket,
                  char const*  buffer,
                  std::size_t  len)
                : io(*get_scheduler(receiver).context, socket.fd, POLLOUT)
                , d_socket(socket)
                , receiver(receiver)
                , buffer(buffer)
                , len(len)
                , cb() {
            }

            friend void start(state& self) {
                self.cb.engage(self);
                self.c.add(&self);
            }
            void complete() override final {
                cb.disengage();
                auto ssl = d_socket.get_ssl();
                auto res{SSL_write(ssl, buffer, len)};
                if (0 < res) {
                    set_value(std::move(receiver), result_t(res));
                }
                else {
                    switch (int error = SSL_get_error(ssl, res)) {
                        default:
                            set_error(std::move(receiver), std::make_exception_ptr(std::system_error(error, ssl_category())));
                            break;
                        case SSL_ERROR_WANT_WRITE:
                            this->events = POLLOUT;
                            start(*this);
                            break;
                        case SSL_ERROR_WANT_READ:
                            this->events = POLLIN;
                            start(*this);
                            break;
                    }
                }  
            }
        };
        template <typename R>
        friend state<R> connect(sender const& self, R receiver) {
            return state<R>(receiver, self.socket, self.buffer, self.len);
        }
    };
}

using async_write_some = hidden_async_write_some::sender;
using async_send = hidden_async_write_some::sender;

// ----------------------------------------------------------------------------

namespace hidden_async_read_some {
    struct sender {
        using result_t = int;

        toy::socket&  socket;
        char*         buffer;
        std::size_t   len;

        sender(toy::socket& socket, char* buffer, std::size_t len)
            : socket(socket)
            , buffer(buffer)
            , len(len) {
        }
        sender(toy::socket& socket, std::array<::iovec, 1> const& buffer)
            : socket(socket)
            , buffer(static_cast<char*>(buffer[0].iov_base))
            , len(buffer[0].iov_len)
        {
        }

        template <typename R>
        struct state
            : toy::io
        {
            toy::socket&                                  d_socket;
            R                                             receiver;
            char*                                         buffer;
            std::size_t                                   len;
            hidden::io_operation::stop_callback<state, R> cb;

            state(R            receiver,
                  toy::socket& socket,
                  char*        buffer,
                  std::size_t  len)
                : io(*get_scheduler(receiver).context, socket.fd, POLLIN)
                , d_socket(socket)
                , receiver(receiver)
                , buffer(buffer)
                , len(len)
                , cb() {
            }

            friend void start(state& self) {
                self.cb.engage(self);
                self.c.add(&self);
            }
            void complete() override final {
                cb.disengage();
                auto ssl = d_socket.get_ssl();
                auto res{SSL_read(ssl, buffer, len)};
                if (0 < res) {
                    set_value(std::move(receiver), result_t(res));
                }
                else {
                    switch (int error = SSL_get_error(ssl, res)) {
                        default:
                            set_error(std::move(receiver), std::make_exception_ptr(std::system_error(error, ssl_category())));
                            break;
                        case SSL_ERROR_WANT_WRITE:
                            this->events = POLLOUT;
                            start(*this);
                            break;
                        case SSL_ERROR_WANT_READ:
                            this->events = POLLIN;
                            start(*this);
                            break;
                    }
                }  
            }
        };
        template <typename R>
        friend state<R> connect(sender const& self, R receiver) {
            return state<R>(receiver, self.socket, self.buffer, self.len);
        }
    };
}

using async_read_some = hidden_async_read_some::sender;
using async_receive = hidden_async_read_some::sender;

// ----------------------------------------------------------------------------

namespace hidden::io_operation {
    template <typename Operation>
    struct sender {
        using result_t = typename Operation::result_t;

        toy::socket&  socket;
        Operation     op;

        template <typename R>
        struct state
            : toy::io
        {
            R                       receiver;
            Operation               op;
            stop_callback<state, R> cb;

            state(R         receiver,
                  int       fd,
                  Operation op)
                : io(*get_scheduler(receiver).context, fd, Operation::event == event_kind::read? POLLIN: POLLOUT)
                , receiver(receiver)
                , op(op)
                , cb() {
            }

            friend void start(state& self) {
                self.cb.engage(self);
                self.c.add(&self);
            }
            void complete() override final {
                cb.disengage();
                auto res{op(*this)};
                if (0 <= res)
                    set_value(std::move(receiver), typename Operation::result_t(res));
                else {
                    set_error(std::move(receiver), std::make_exception_ptr(std::system_error(errno, std::system_category())));
                }  
            }
        };
        template <typename R>
        friend state<R> connect(sender const& self, R receiver) {
            return state<R>(receiver, self.socket.fd, self.op);
        }
    };
}

template <typename MBS>
hidden::io_operation::sender<hidden::io_operation::receive_from_op<MBS>>
async_receive_from(toy::socket& s, const MBS& b, toy::address& addr, toy::message_flags f) {
    return {s, {b, addr, f}};
}
template <typename MBS>
hidden::io_operation::sender<hidden::io_operation::receive_from_op<MBS>>
async_receive_from(toy::socket& s, const MBS& b, toy::address& addr) {
    return {s, {b, addr, toy::message_flags{}}};
}

template <typename MBS>
hidden::io_operation::sender<hidden::io_operation::send_to_op<MBS>>
async_send_to(toy::socket& s, const MBS& b, toy::address addr, toy::message_flags f) {
    return {s, {b, addr, f}};
}
template <typename MBS>
hidden::io_operation::sender<hidden::io_operation::send_to_op<MBS>>
async_send_to(toy::socket& s, const MBS& b, toy::address addr) {
    return {s, {b, addr, toy::message_flags{}}};
}

// ----------------------------------------------------------------------------

namespace hidden_async_sleep_for {
    struct async_sleep_for {
        using result_t = toy::none;
        using duration_t = std::chrono::milliseconds;

        duration_t  duration;

        template <typename R>
        struct state
            : io {
            R                            receiver;
            duration_t                   duration;
            hidden::io_operation::stop_callback<state, R, true> cb;

            state(R receiver, duration_t duration)
                : io(*get_scheduler(receiver).context, 0, 0)
                , receiver(receiver)
                , duration(duration) {
            }
            friend void start(state& self) {
                self.cb.engage(self);
                self.c.add(std::chrono::system_clock::now() + self.duration, &self);;
            }
            void complete() override {
                cb.disengage();
                set_value(receiver, result_t{});
            }
        };
        template <typename R>
        friend state<R> connect(async_sleep_for self, R receiver) {
            return state<R>(receiver, self.duration);
        }
    };
}
using async_sleep_for = hidden_async_sleep_for::async_sleep_for;
#endif

// ----------------------------------------------------------------------------

}

// ----------------------------------------------------------------------------

#endif
