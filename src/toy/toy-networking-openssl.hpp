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

#include "toy-sender.hpp"
#include "toy-networking-common.hpp"
#include "toy-networking-sender.hpp"
#include "toy-networking-poll.hpp"
#include "toy-starter.hpp"
#include "toy-utility.hpp"
#include "toy-ring_buffer.hpp"
#include "toy-task.hpp"

#include <algorithm>
#include <chrono>
#include <list>
#include <optional>
#include <queue>
#include <stdexcept>
#include <string>
#include <system_error>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <stddef.h>
#include <string.h>
#include <cassert>

#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

// ----------------------------------------------------------------------------

namespace toy::openssl
{
    template <typename, typename> struct signaller;

    struct ssl_error_category;
    inline ssl_error_category const& ssl_category();

    class socket;
    class context_base;
    template <typename UpstreamContext = toy::poll::context> class context;
    template <typename UpstreamContext> struct scheduler;
    struct io_base;
    template <typename Receiver, typename Operation> struct io_state;

    template <typename Receiver> struct io_state<Receiver, toy::io::connect_t::args>;
}

// ----------------------------------------------------------------------------

struct toy::openssl::ssl_error_category
    : std::error_category
{
    ssl_error_category(): std::error_category() {}
    char const* name() const noexcept override { return "ssl"; }
    std::string message(int err) const override
    {
        char error[256];
        ERR_error_string_n(err, error, sizeof(error));
        return error;
    }
};

inline toy::openssl::ssl_error_category const& toy::openssl::ssl_category()
{
    static ssl_error_category rc{};
    return rc;
}

// ----------------------------------------------------------------------------

template <typename T, typename Fun>
struct toy::openssl::signaller
{
    struct state_base
    {
        virtual void complete() = 0;
    };
    template <typename Receiver>
    struct state
        : state_base
    {
        struct stop_callback
        {
            state* s;
            void operator()()
            {
                s->obj->awaiting = nullptr;
                set_stopped(std::move(s->receiver));
            }
        };
        using callback_t = decltype(get_stop_token(std::declval<Receiver>()))::template callback_type<stop_callback>;

        signaller*                    obj;
        std::remove_cvref_t<Receiver> receiver;
        std::optional<callback_t>     callback;
        template <typename R>
        state(signaller* obj, R&& receiver)
            : obj(obj)
            , receiver(std::forward<R>(receiver))
        {
        }
        friend void start(state& self)
        {
            if (self.obj->flag) // Fun()(self.obj->object))
            {
                set_value(std::move(self.receiver), toy::none());
            }
            else
            {
                self.callback.emplace(get_stop_token(self.receiver), stop_callback{&self});
                self.obj->awaiting = &self;
            }
        }
        void complete() override
        {
            callback.reset();
            set_value(std::move(receiver), toy::none());
        }
    };
    struct sender {
        using result_t = toy::none;
        signaller* obj;
        template <typename Receiver>
        friend state<Receiver> connect(sender const& self, Receiver&& receiver)
        {
            return { self.obj, std::forward<Receiver>(receiver) };
        }
    };
    T*          object;
    state_base* awaiting{};
    bool        flag{false};

    void resume()
    {
        flag = true;
        if (awaiting)
        {
            std::exchange(awaiting, nullptr)->complete();
        }
    }
    void stop()
    {
        flag = false;
    }
    sender wait()
    {
        return { this };
    }
};

// ----------------------------------------------------------------------------

struct toy::openssl::socket
{
    using ssl_free_t = decltype([](auto s){ SSL_free(s); });

    static constexpr int buffer_size = 1024;

    toy::socket                      upstream;
    std::unique_ptr<SSL, ssl_free_t> ssl;
    toy::ring_buffer<buffer_size>    from_network{};
    toy::ring_buffer<buffer_size>    to_network{};
    BIO*                             network_to_ssl{BIO_new(BIO_s_mem())}; 
    BIO*                             ssl_to_network{BIO_new(BIO_s_mem())};
    signaller<SSL, decltype([](SSL* ssl){ return SSL_want_read(ssl); })> await_write{};
    bool done                        {false};
    toy::openssl::io_base*           outstanding{nullptr};

    socket(toy::socket&& upstream, SSL* ssl)
        : upstream(std::move(upstream))
        , ssl(ssl)
        , await_write(this->ssl.get())
    {
        if (!ssl)
        {
            throw std::system_error(ERR_get_error(), toy::openssl::ssl_category(), "creating SSL socket");
        }
        SSL_set_bio(this->ssl.get(), this->network_to_ssl, this->ssl_to_network);
    }
};

// ----------------------------------------------------------------------------

struct toy::openssl::io_base
    : toy::immovable
{
    virtual void complete() = 0;
};

// ----------------------------------------------------------------------------

template <typename Receiver, typename Operation>
struct toy::openssl::io_state final
    : toy::openssl::io_base
{
    Receiver receiver;
    io_state(Receiver receiver, toy::socket&, toy::event_kind, Operation)
        : io_base()
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

// ----------------------------------------------------------------------------

template <typename Receiver>
struct toy::openssl::io_state<Receiver, toy::io::connect_t::args> final
    : toy::openssl::io_base
{
    Receiver receiver;
    io_state(Receiver receiver, toy::socket&, toy::event_kind, toy::io::connect_t::args)
        : io_base()
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

// ----------------------------------------------------------------------------

toy::task<toy::poll::context::scheduler> reader(toy::openssl::socket& client)
{
    while (!client.done)
    {
        auto buffer = co_await client.from_network.consume();
        if (buffer.buffer.size() == 0u) {
            client.done = true;
            co_return;
        }
        auto n = BIO_write(client.network_to_ssl, buffer.buffer.data(), buffer.buffer.size());
        std::cout << "wrote " << n << " to SSL\n";
        if (n < 0)
        {
            co_return; //-dk:TODO produce an error
        }
        if (!SSL_is_init_finished(client.ssl.get()))
        {
            std::cout << "init isn't finished\n";
            switch (auto error = SSL_get_error(client.ssl.get(), SSL_accept(client.ssl.get())))
            {
            case SSL_ERROR_SSL:
                std::cout << "no error\n";
                break;
            case SSL_ERROR_NONE:
            case SSL_ERROR_WANT_READ:
                std::cout << "want read\n";
                client.await_write.resume();
                break;
            case SSL_ERROR_WANT_WRITE:
                std::cout << "want write\n";
                break;
            default:
                {
                    char buffer[256];
                    ERR_error_string_n(error, buffer, sizeof(buffer));
                    std::cout << "SSL accept failure: " << error << ": " << buffer << "\n";
                    co_return; //-dk:TODO produce an error
                }
            }
            if (SSL_is_init_finished(client.ssl.get()))
            {
                std::cout << "*** SSL accept is complete\n";
                if (client.outstanding)
                {
                    std::exchange(client.outstanding, nullptr)->complete();
                }
            }
        }
        else {
            char rbuf[1024];
            for (int r; 0 < (r = SSL_read(client.ssl.get(), rbuf, sizeof(rbuf))); )
            {
                std::cout << "received n=" << n << " bytes of data: '" << std::string_view(rbuf, r) << "'\n";
                SSL_write(client.ssl.get(), rbuf, r);
                client.await_write.resume();
            }
        }
        if (n == 0) {
            client.done = true;
        }
        buffer.advance(n);
    }
}

toy::task<toy::poll::context::scheduler> writer(toy::openssl::socket& client)
{
    while (!client.done)
    {
        co_await client.await_write.wait();

        auto buffer = co_await client.to_network.produce();
        int n = BIO_read(client.ssl_to_network, buffer.data(), buffer.size());
        if (n <= 0)
        {
            client.await_write.stop();
            n = 0;
        }
        buffer.advance(n);
    }
}

template <typename Receiver>
struct toy::openssl::io_state<Receiver, toy::io::accept_t::args> final
    : toy::openssl::io_base
{
    Receiver     receiver;
    toy::socket& socket;
    toy::socket  client;

    io_state(Receiver receiver, toy::socket& socket, toy::event_kind, toy::io::accept_t::args)
        : io_base()
        , receiver(receiver)
        , socket(socket)
    {
    }
    friend void start(io_state& self)
    {
        auto scheduler = get_scheduler(self.receiver);
        SSL_set_accept_state(scheduler.get_socket(self.socket).ssl.get());
        scheduler.upstream_spawn(
            toy::let_value(
                toy::async_accept(scheduler.upstream_socket(self.socket)),
                [&self](auto upstream){
                    self.client = get_scheduler(self.receiver).make_socket(std::move(upstream));
                    toy::openssl::socket& client = get_scheduler(self.receiver).get_socket(self.client);
                    client.outstanding = &self;
                    return toy::when_any(
                        toy::repeat_effect_until(
                            toy::let_value(client.from_network.produce(), [&client, n = 1](auto buffer) mutable {
                                return toy::then(toy::async_receive(client.upstream, toy::buffer(buffer.buffer)),
                                                 [&client, buffer, &n](int r) mutable {
                                                    n = std::max(0, r);
                                                    if (r <= 0)
                                                    {
                                                        client.done = true;
                                                    }
                                                    std::cout << "received(" << r << ")\n";
                                                    buffer.advance(r);
                                                 });
                            })
                            , [&client]{ return client.done; }
                        )
                        , reader(client)
                        , writer(client)
                        , toy::repeat_effect_until(
                                toy::let_value(client.to_network.consume(), [&client](auto buffer) {
                                    std::cout << "sending to the network\n";
                                    return toy::then(toy::async_send(client.upstream, toy::buffer(buffer.buffer)),
                                        [buffer](int r) mutable { std::cout << "sent(" << r << ")\n"; buffer.advance(std::max(0, r)); });
                                })
                                , [&client]{ return client.done; }
                        )
                    );
                }
            )
        );
    }
    void complete() override
    {
        set_value(std::move(this->receiver), std::move(this->client));
    }
};

// ----------------------------------------------------------------------------

class toy::openssl::context_base
    : public toy::io_context_base
{
protected:
    using ssl_ctx_free_t = decltype([](auto c){ SSL_CTX_free(c); });

    socket_handle                                           d_next_id{16};
    std::unordered_map<socket_handle, toy::openssl::socket> d_sockets;

    std::unique_ptr<SSL_CTX, ssl_ctx_free_t>                d_ssl_context;
    context_base()
        : d_ssl_context(SSL_CTX_new(TLS_server_method())) //-dk:TODO TLS_method?
    {
        if (!this->d_ssl_context)
        {
            throw std::system_error(ERR_get_error(), toy::openssl::ssl_category(), "allocating SSL context");
        }
    }
    context_base(std::string const& certificate, std::string const& key)
        : context_base()
    {
        if(1 != SSL_CTX_use_certificate_file(this->d_ssl_context.get(), certificate.c_str(),  SSL_FILETYPE_PEM))
        {
            throw std::system_error(ERR_get_error(), toy::openssl::ssl_category(), "load certificate file");
        }
        if (1 != SSL_CTX_use_PrivateKey_file(this->d_ssl_context.get(), key.c_str(), SSL_FILETYPE_PEM))
        {
            throw std::system_error(ERR_get_error(), toy::openssl::ssl_category(), "load private key file");
        }
        if (1 != SSL_CTX_check_private_key(this->d_ssl_context.get()))
        {
            throw std::system_error(ERR_get_error(), toy::openssl::ssl_category(), "check private key");
        }
        SSL_CTX_set_options(this->d_ssl_context.get(), SSL_OP_ALL|SSL_OP_NO_SSLv2|SSL_OP_NO_SSLv3);
    }

    toy::openssl::socket& get_socket(socket_handle id)
    {
        auto it(this->d_sockets.find(id));
        assert(it != this->d_sockets.end());
        return it->second;
    }
    socket_handle fd(socket_handle id) override { return this->get_socket(id).upstream.fd(); }

public:
    toy::socket& upstream_socket(toy::socket& s)
    {
        std::cout << "upstream_socket: handle=" << s.id() << "\n";
        auto it(this->d_sockets.find(s.id()));
        assert(it != this->d_sockets.end());
        return it->second.upstream;
    }
    toy::openssl::socket& get_socket(toy::socket& s) { return this->get_socket(s.id()); }
};

template <typename UpstreamContext>
struct toy::openssl::scheduler {
    template <typename Receiver, typename Operation, typename Stream>
    using io_state = toy::openssl::io_state<Receiver, Operation>;

    toy::openssl::context<UpstreamContext>* context;
    friend std::ostream& operator<< (std::ostream& out, scheduler const& s) {
        return out << s.context;
    }

    toy::socket& upstream_socket(toy::socket& s) { return this->context->upstream_socket(s); }
    toy::socket  make_socket(toy::socket&& upstream) { return this->context->make_socket(std::move(upstream)); }
    toy::openssl::socket& get_socket(toy::socket& s) { return this->context->get_socket(s); }
    

    template <typename Sender>
    void upstream_spawn(Sender&& sender)
    {
        this->context->upstream_spawn(std::forward<Sender>(sender));
    }
};

// ----------------------------------------------------------------------------

template <typename UpstreamContext>
class toy::openssl::context
    : public toy::openssl::context_base
{
public:
    friend class toy::openssl::scheduler<UpstreamContext>;

    using scheduler = toy::openssl::scheduler<UpstreamContext>;
    scheduler get_scheduler() { return { this }; }

private:
    std::unique_ptr<UpstreamContext> d_owned_context;
    UpstreamContext&                 d_upstream_context;

    socket_handle make_socket(int domain, int type, int protocol) override
    {
        socket_handle handle = ++this->d_next_id;
        this->d_sockets.emplace( handle, toy::openssl::socket(toy::socket(this->d_upstream_context, domain, type, protocol), SSL_new(this->d_ssl_context.get())));
        std::cout << "openssl::make_socket: handle=" << handle << "\n";
        return handle;
    }
    toy::socket make_socket(toy::socket&& upstream)
    {
        std::cout << "openssl::make_socket\n";
        auto id(++this->d_next_id);
        this->d_sockets.emplace( id, toy::openssl::socket(std::move(upstream), SSL_new(this->d_ssl_context.get())) );
        return toy::socket(*this, toy::socket::from_id(), id);
    }

    socket_handle make_fd(socket_handle fd) override
    {
        std::cout << "openssl::make_fd\n";
        socket_handle handle = ++this->d_next_id;
        this->d_sockets.emplace( handle, toy::openssl::socket(toy::socket(this->d_upstream_context, fd), SSL_new(this->d_ssl_context.get())));
        return handle;
    }

public:
    static constexpr bool has_timer = true; //-dk:TODO remove - used while adding timers to contexts

    context()
        : toy::openssl::context_base()
        , d_owned_context(new UpstreamContext())
        , d_upstream_context(*this->d_owned_context)
    {
    }
    context(std::string const& certificate, std::string const& key)
        : toy::openssl::context_base(certificate, key)
        , d_owned_context(new UpstreamContext())
        , d_upstream_context(*this->d_owned_context)
    {
    }
    context(UpstreamContext& context)
        : toy::openssl::context_base()
        , d_owned_context()
        , d_upstream_context(context)
    {
    }
    void run()
    {
        return this->d_upstream_context.run();
    }
    template <typename Sender>
    void spawn(Sender&& sender) { this->d_upstream_context.spawn(toy::on(this->get_scheduler(), std::forward<Sender>(sender))); }
    template <typename Sender>
    void upstream_spawn(Sender&& sender) { this->d_upstream_context.spawn(std::forward<Sender>(sender)); }
};

// ----------------------------------------------------------------------------

#endif
