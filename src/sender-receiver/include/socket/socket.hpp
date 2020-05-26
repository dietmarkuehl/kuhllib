// include/socket/socket.hpp                                          -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2020 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_SOCKET_SOCKET
#define INCLUDED_SOCKET_SOCKET

#include <netfwd.hpp>
#include <io_context.hpp>
#include <io_operation.hpp>
#include <socket/socket_base.hpp>
#include <execution/set_value.hpp>
#include <execution/set_error.hpp>
#include <sender/then.hpp>

#include <memory>
#include <system_error>
#include <cerrno>
#include <iostream> //-dk:TODO remove

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

// ----------------------------------------------------------------------------

namespace cxxrt::net
{
    template <typename> class basic_socket;
    template <typename...> class connect_operation;
}

// ----------------------------------------------------------------------------

template <typename Protocol>
class cxxrt::net::basic_socket
    : cxxrt::net::socket_base
{
private:
    cxxrt::net::io_context* d_context;
    int                     d_fd;

public:
    using executor_type      = cxxrt::net::io_context::executor_type;
    using native_handle_type = int;
    using protocol_type      = Protocol;
    using endpoint_type      = typename protocol_type::endpoint;

    cxxrt::net::io_context* context() noexcept { return this->d_context; } //-dk:TODO remove
    executor_type      get_executor() noexcept;
    native_handle_type native_handle() { return this->d_fd; }

    void open(protocol_type const& protocol = protocol_type());
    void open(protocol_type const& protocol, std::error_code& ec);
    void assign(protocol_type const& protocol,
                native_handle_type const& native_socket);
    void assign(protocol_type const& protocol,
                native_handle_type const& native_socket,
                std::error_code& ec);
    native_handle_type release(); // see 18.2.3
    native_handle_type release(std::error_code& ec); // see 18.2.3
    bool is_open() const noexcept { return this->d_fd != -1; }
    void close();
    void close(std::error_code& ec);
    void cancel();
    void cancel(std::error_code& ec);
    template<typename SettableSocketOption>
    void set_option(SettableSocketOption const& option);
    template<typename SettableSocketOption>
    void set_option(SettableSocketOption const& option, std::error_code& ec);
    template<typename GettableSocketOption>
    void get_option(GettableSocketOption& option) const;
    template<typename GettableSocketOption>
    void get_option(GettableSocketOption& option, std::error_code& ec) const;
    template<typename IoControlCommand>
    void io_control(IoControlCommand& command);
    template<typename IoControlCommand>
    void io_control(IoControlCommand& command, std::error_code& ec);
    void non_blocking(bool mode);
    void non_blocking(bool mode, std::error_code& ec);
    bool non_blocking() const;
    void native_non_blocking(bool mode);
    void native_non_blocking(bool mode, std::error_code& ec);
    bool native_non_blocking() const;
    bool at_mark() const;
    bool at_mark(std::error_code& ec) const;
    size_t available() const;
    size_t available(std::error_code& ec) const;
    void bind(endpoint_type const& endpoint);
    void bind(endpoint_type const& endpoint, std::error_code& ec);
    void shutdown(shutdown_type what);
    void shutdown(shutdown_type what, std::error_code& ec);
    endpoint_type local_endpoint() const;
    endpoint_type local_endpoint(std::error_code& ec) const;
    endpoint_type remote_endpoint() const;
    endpoint_type remote_endpoint(std::error_code& ec) const;
    void connect(endpoint_type const& endpoint);
    void connect(endpoint_type const& endpoint, std::error_code& ec);

    struct connect_sender;
    connect_sender async_connect(endpoint_type const& endpoint);
    template<cxxrt::execution::receiver R>
    void async_connect(endpoint_type const& endpoint, R&& r);
    //-dk:TODO template<typename CompletionToken>
    //-dk:TODO DEDUCED async_connect(endpoint_type const& endpoint,
    //-dk:TODO                       CompletionToken&& token);
    void wait(wait_type w);
    void wait(wait_type w, std::error_code& ec);
    //-dk:TODO template<typename CompletionToken>
    //-dk:TODO DEDUCED async_wait(wait_type w, CompletionToken&& token);

    struct async_connect_object;
    async_connect_object async_connect() { return {this}; }
    friend async_connect_object async_connect(basic_socket& s) { return s.async_connect(); }
    

protected:
    explicit basic_socket(cxxrt::net::io_context& ctx);
    basic_socket(cxxrt::net::io_context& ctx, protocol_type const& protocol);
    basic_socket(cxxrt::net::io_context& ctx, endpoint_type const& endpoint);
    basic_socket(cxxrt::net::io_context& ctx, protocol_type const& protocol,
                 native_handle_type const& native_socket);
    basic_socket(basic_socket const&) = delete;
    basic_socket(basic_socket&& rhs);
    template<typename OtherProtocol>
    basic_socket(basic_socket<OtherProtocol>&& rhs);
    ~basic_socket();

    basic_socket& operator=(basic_socket const&) = delete;
    basic_socket& operator=(basic_socket&& rhs);
    template<typename OtherProtocol>
    basic_socket& operator=(basic_socket<OtherProtocol>&& rhs);
};

// ----------------------------------------------------------------------------

template <typename Protocol>
cxxrt::net::basic_socket<Protocol>::basic_socket(cxxrt::net::io_context& ctx)
    : d_context(std::addressof(ctx))
    , d_fd(-1)
{
}

template <typename Protocol>
cxxrt::net::basic_socket<Protocol>::~basic_socket()
{
    if (this->d_fd != -1)
    {
        ::close(this->d_fd);
    }
}

// ----------------------------------------------------------------------------

template <typename Protocol>
void cxxrt::net::basic_socket<Protocol>::open(protocol_type const& protocol,
                                              std::error_code& ec)
{
    this->d_fd = ::socket(protocol.domain(), SOCK_STREAM, protocol.protocol());
    if (this->d_fd == -1)
    {
        ec.assign(errno, std::system_category());
    }
    else
    {
        int flags(::fcntl(this->d_fd, F_GETFL, 0));
        if (flags == 1
            || ::fcntl(this->d_fd, F_SETFL, flags | O_NONBLOCK) == -1)
        {
            ec.assign(errno, std::system_category());
            ::close(this->d_fd);
            this->d_fd = -1;
        }
    }
}

// ----------------------------------------------------------------------------

template <typename... A>
class cxxrt::net::connect_operation
    : public cxxrt::net::io_operation_base<A...>
{
protected:
    bool do_notify(int fd, short events) override
    {
        if (events & POLLOUT)
        {
            this->set_value();
        }
        else
        {
            socklen_t size(sizeof(int));
            int err(0), rc(::getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &size));
            this->set_error(std::error_code(rc == -1? errno: err,
                                            std::system_category()));
        }
        return true;
    }

    template <typename S, typename P>
    bool open(S* sock, P const& protocol)
    {
        if (sock->is_open())
        {
            return true;
        }
        std::error_code ec;
        sock->open(protocol, ec);
        return ec? this->set_error(ec), false: true;
    }

public:
    template <typename S, typename E>
    void connect(S* sock, E const& ep)
    {
        if (!this->open(sock, ep.protocol()))
        {
            return;
        }
        if (::connect(sock->native_handle(), ep.data(), ep.size()) == -1)
        {
            if (errno != EINTR && errno != EINPROGRESS)
            {
                this->set_error(std::error_code(errno, std::system_category()));
                return;
            }
            sock->context()->add(sock->native_handle(), POLLOUT, this);
        }
        else
        {
            this->set_value();
        }
    }
};

// ----------------------------------------------------------------------------

template <typename Protocol>
struct cxxrt::net::basic_socket<Protocol>::async_connect_object
{
    template <typename R>
    struct receiver
    {
        cxxrt::net::io_operation<R, cxxrt::net::connect_operation> d_op;
        basic_socket* d_s;

        void set_value(auto&& ep) noexcept { this->d_op.connect(this->d_s, ep); }
        void set_error(auto&& e)  noexcept { this->d_op.set_error(e); }
        void set_done()           noexcept { this->d_op.set_done(); }
    };
    template <typename Sender>
    struct sender
        : cxxrt::execution::sender_base
    {
        Sender        d_sender;
        basic_socket* d_s;
        
        template <typename R>
        auto connect(R&& r) &&
        {
            return cxxrt::execution::connect(std::move(this->d_sender),
                                             receiver<std::remove_cvref_t<R>>
                                             {
                                                 std::forward<R>(r),
                                                 this->d_s
                                              });
        }
    };

    basic_socket* d_s;
    template <typename S>
    sender<std::remove_cvref_t<S>> operator()(S&& s)
    {
        return { {}, std::forward<S>(s), this->d_s };
    }
};

// ----------------------------------------------------------------------------

template <typename Protocol>
struct cxxrt::net::basic_socket<Protocol>::connect_sender
{
private:
    friend class basic_socket;

    basic_socket* d_socket;
    endpoint_type d_endpoint;
    connect_sender(basic_socket* s, endpoint_type const& e): d_socket(s), d_endpoint(e) {}

public:
    //-dk:TODO report back the other side endpoint
    template <template <typename...> class T,
              template <typename...> class V>
    using value_types = V<T<>>;
    template <template <typename...> class V>
    using error_types = V<std::error_code, std::exception_ptr>;

    static constexpr bool sends_done = false;

    template <typename R>
    struct state
        : cxxrt::net::io_operation<R, cxxrt::net::connect_operation>
    {
    private:
        basic_socket* d_socket;
        endpoint_type d_endpoint;

    public:
        state(basic_socket* s, endpoint_type e, R r)
            : io_operation<R, cxxrt::net::connect_operation>(std::forward<R>(r))
            , d_socket(s)
            , d_endpoint(std::move(e))
        {
        }
        void operator= (state&&) = delete;
        void start() noexcept
        {
            this->connect(this->d_socket, this->d_endpoint);
        }
    };

    template <typename R>
    state<R> connect(R&& r) {
        return state<R>(this->d_socket,
                     std::move(this->d_endpoint),
                     std::forward<R>(r));
    }
};

template <typename Protocol>
auto cxxrt::net::basic_socket<Protocol>::async_connect(endpoint_type const& endpoint)
    -> connect_sender
{
    return connect_sender{this, endpoint};
}

// ----------------------------------------------------------------------------

#endif
