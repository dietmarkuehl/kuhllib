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
#include <socket/socket_base.hpp>

#include <memory>
#include <system_error>

#include <unistd.h>

// ----------------------------------------------------------------------------

namespace cxxrt::net
{
    template <typename> class basic_socket;
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
    bool is_open() const noexcept;
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
    connect_sender sender_connect(endpoint_type const& endpoint);
    //-dk:TODO template<typename CompletionToken>
    //-dk:TODO DEDUCED async_connect(endpoint_type const& endpoint,
    //-dk:TODO                       CompletionToken&& token);
    void wait(wait_type w);
    void wait(wait_type w, std::error_code& ec);
    //-dk:TODO template<typename CompletionToken>
    //-dk:TODO DEDUCED async_wait(wait_type w, CompletionToken&& token);

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

#endif
