// include/socket/stream_socket.hpp                                   -*-C++-*-
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

#ifndef INCLUDED_SOCKET_STREAM_SOCKET
#define INCLUDED_SOCKET_STREAM_SOCKET

#include <netfwd.hpp>
#include <io_context.hpp>
#include <socket/socket.hpp>
#include <execution/sender_base.hpp>

#include <system_error>

#include <iostream> //-dk:TODO remove

// ----------------------------------------------------------------------------

namespace cxxrt::net
{
    template <typename> class basic_stream_socket;
}

// ----------------------------------------------------------------------------

template<typename Protocol>
class cxxrt::net::basic_stream_socket
    : public cxxrt::net::basic_socket<Protocol>
{
public:
    using native_handle_type = int;
    using protocol_type = Protocol;
    using endpoint_type = typename protocol_type::endpoint;

    explicit basic_stream_socket(cxxrt::net::io_context& ctx);
    basic_stream_socket(cxxrt::net::io_context& ctx, protocol_type const& protocol);
    basic_stream_socket(cxxrt::net::io_context& ctx, endpoint_type const& endpoint);
    basic_stream_socket(cxxrt::net::io_context& ctx, protocol_type const& protocol,
                        native_handle_type const& native_socket);
    basic_stream_socket(basic_stream_socket const&) = delete;
    basic_stream_socket(basic_stream_socket&& rhs);
    template<typename OtherProtocol>
    basic_stream_socket(basic_stream_socket<OtherProtocol>&& rhs);
    ~basic_stream_socket() = default;

    basic_stream_socket& operator=(basic_stream_socket const&) = delete;
    basic_stream_socket& operator=(basic_stream_socket&& rhs);
    template<typename OtherProtocol>
    basic_stream_socket& operator=(basic_stream_socket<OtherProtocol>&& rhs);

    template<typename MutableBufferSequence>
    size_t receive(MutableBufferSequence const& buffers);
    template<typename MutableBufferSequence>
    size_t receive(MutableBufferSequence const& buffers,
                   std::error_code& ec);
    template<typename MutableBufferSequence>
    size_t receive(MutableBufferSequence const& buffers,
                   socket_base::message_flags flags);
    template<typename MutableBufferSequence>
    size_t receive(MutableBufferSequence const& buffers,
                   socket_base::message_flags flags, std::error_code& ec);

    template <typename MutableBufferSequence>
    struct receive_sender;
    template <typename MutableBufferSequence>
    receive_sender<MutableBufferSequence>
    async_receive(MutableBufferSequence const& buffers);
    template <typename MutableBufferSequence>
    receive_sender<MutableBufferSequence>
    async_receive(MutableBufferSequence const& buffers,
                  socket_base::message_flags);
    struct async_receive_object
    {
        basic_stream_socket* d_s;
        template <typename MutableBufferSequence>
        receive_sender<MutableBufferSequence> operator()(MutableBufferSequence const& buffers)
        {
            return this->d_s->async_receive(buffers);
        }
    };
    friend async_receive_object async_receive(basic_stream_socket& s) { return {&s}; }
    template <typename MutableBufferSequence>
    friend receive_sender<MutableBufferSequence>
    async_receive(basic_stream_socket& s,
                  MutableBufferSequence const& buffers,
                  socket_base::message_flags flags) {
        return {&s, buffers, flags};
    }
    template <typename MutableBufferSequence>
    friend receive_sender<MutableBufferSequence>
    async_receive(basic_stream_socket& s,
                  MutableBufferSequence const& buffers) {
        return {&s, buffers, socket_base::message_flags()};
    }
    //-dk:TODO template<typename MutableBufferSequence, typename CompletionToken>
    //-dk:TODO DEDUCED async_receive(MutableBufferSequence const& buffers,
    //-dk:TODO                       CompletionToken&& token);
    //-dk:TODO template<typename MutableBufferSequence, typename CompletionToken>
    //-dk:TODO DEDUCED async_receive(MutableBufferSequence const& buffers,
    //-dk:TODO                       socket_base::message_flags flags,
    //-dk:TODO                       CompletionToken&& token);
    template<typename ConstBufferSequence>
    size_t send(ConstBufferSequence const& buffers);
    template<typename ConstBufferSequence>
    size_t send(ConstBufferSequence const& buffers, std::error_code& ec);
    template<typename ConstBufferSequence>
    size_t send(ConstBufferSequence const& buffers,
                socket_base::message_flags flags);
    template<typename ConstBufferSequence>
    size_t send(ConstBufferSequence const& buffers,
                socket_base::message_flags flags, std::error_code& ec);

    template <typename ConstBufferSequence>
    struct send_sender;
    template <typename ConstBufferSequence>
    send_sender<ConstBufferSequence> async_send(ConstBufferSequence const& buffers);
    struct async_send_object
    {
        basic_stream_socket* d_s;
        template <typename ConstBufferSequence>
        send_sender<ConstBufferSequence> operator()(ConstBufferSequence const& buffers)
        {
            return this->d_s->async_send(buffers);
        }
    };
    friend async_send_object async_send(basic_stream_socket& s) { return {&s}; }
    
    //-dk:TODO template<typename ConstBufferSequence, typename CompletionToken>
    //-dk:TODO DEDUCED async_send(ConstBufferSequence const& buffers,
    //-dk:TODO                    CompletionToken&& token);
    //-dk:TODO template<typename ConstBufferSequence, typename CompletionToken>
    //-dk:TODO DEDUCED async_send(ConstBufferSequence const& buffers,
    //-dk:TODO                    socket_base::message_flags flags,
    //-dk:TODO                    CompletionToken&& token);
    template<typename MutableBufferSequence>
    size_t read_some(MutableBufferSequence const& buffers);
    template<typename MutableBufferSequence>
    size_t read_some(MutableBufferSequence const& buffers,
                     std::error_code& ec);
    //-dk:TODO template<typename MutableBufferSequence, typename CompletionToken>
    //-dk:TODO DEDUCED async_read_some(MutableBufferSequence const& buffers,
    //-dk:TODO                         CompletionToken&& token);
    template<typename ConstBufferSequence>
    size_t write_some(ConstBufferSequence const& buffers);
    template<typename ConstBufferSequence>
    size_t write_some(ConstBufferSequence const& buffers,
                      std::error_code& ec);
    //-dk:TODO template<typename ConstBufferSequence, typename CompletionToken>
    //-dk:TODO DEDUCED async_write_some(ConstBufferSequence const& buffers,
    //-dk:TODO                          CompletionToken&& token);
};

// ----------------------------------------------------------------------------

template<typename Protocol>
cxxrt::net::basic_stream_socket<Protocol>::basic_stream_socket(cxxrt::net::io_context& ctx)
    : cxxrt::net::basic_socket<Protocol>(ctx)
{
}

// ----------------------------------------------------------------------------

template <typename Protocol>
template <typename ConstBufferSequence>
struct cxxrt::net::basic_stream_socket<Protocol>::send_sender
    : cxxrt::execution::sender_base
{
private:
    friend class basic_stream_socket;

    basic_stream_socket*       d_socket;
    ConstBufferSequence const& d_buffers;
    send_sender(basic_stream_socket* s, ConstBufferSequence const& b)
        : d_socket(s),
          d_buffers(b)
    {
    }

public:
    //-dk:TODO report amount write?
    template <template <typename...> class T,
              template <typename...> class V>
    using value_types = V<T<>>;
    template <template <typename...> class V>
    using error_types = V<std::error_code, std::exception_ptr>;

    static constexpr bool sends_done = false;

    template <typename R>
    struct state
        : cxxrt::net::detail::waiter
    {
    private:
        basic_stream_socket*       d_socket;
        ConstBufferSequence const& d_buffers;
        R                          d_r;

        bool do_notify(int, short events) override
        {
            if (events & POLLOUT)
            {
                //-dk:TODO write some and signal upon done/error
            }
            return true;
        }

    public:
        state(basic_stream_socket* s, ConstBufferSequence const& b, R r)
            : d_socket(s)
            , d_buffers(b)
            , d_r(std::forward<R>(r))
        {
        }
        void operator= (state&&) = delete;
        void start() noexcept
        {
            //-dk:TODO 
            if (!this->d_socket->is_open())
            {
                std::error_code ec;
                execution::set_error(std::move(this->d_r), ec);
                return;
            }
            this->d_socket->context()->add(this->d_socket->native_handle(), POLLOUT, this);
        }
    };

    template <typename R>
    state<R> connect(R&& r) {
        return state<R>(this->d_socket, this->d_buffers, std::forward<R>(r));
    }
};

template <typename Protocol>
template <typename ConstBufferSequence>
auto cxxrt::net::basic_stream_socket<Protocol>::async_send(ConstBufferSequence const& b)
    -> send_sender<ConstBufferSequence>
{
    return send_sender<ConstBufferSequence>{this, b};
}


// ----------------------------------------------------------------------------

template <typename Protocol>
template <typename MutableBufferSequence>
struct cxxrt::net::basic_stream_socket<Protocol>::receive_sender
    : cxxrt::execution::sender_base
{
private:
    friend class basic_stream_socket;

    basic_stream_socket*         d_socket;
    MutableBufferSequence const& d_buffers;
    socket_base::message_flags   d_flags;

public:
    receive_sender(basic_stream_socket*         stream,
                   MutableBufferSequence const& buffers,
                   socket_base::message_flags   flags)
        : d_socket(stream)
        , d_buffers(buffers)
        , d_flags(flags)
    {
    }

    template <typename R>
    struct state
        : cxxrt::net::detail::waiter
    {
    private:
        basic_stream_socket*         d_socket;
        MutableBufferSequence const& d_buffers;
        socket_base::message_flags   d_flags;
        R                            d_r;

        bool do_notify(int, short events) override
        {
            std::cout << "receiver_sender::state::do_notify (" << bool(events & POLLIN) << ")\n";
            if (events & POLLIN) //-dk:TODO is that actually needed?
            {
                int rc = ::recv(this->d_socket->native_handle(),
                                this->d_buffers.data(),
                                this->d_buffers.size(),
                                0);
                std::cout << "recv=" << rc << "\n";
                if (0 < rc) {
                    execution::set_value(std::move(this->d_r), std::size_t(rc));
                }
                else if (rc == 0) {
                    std::error_code ec;
                    this->d_socket->close(ec);
                    if (ec) {
                        execution::set_error(std::move(this->d_r), ec);
                    }
                    else {
                        execution::set_value(std::move(this->d_r), std::size_t(rc));
                    }
                }
                else {
                    execution::set_error(std::move(this->d_r), rc);
                }
            }
            return true;
        }

    public:
        state(basic_stream_socket* s, MutableBufferSequence const& b, R r)
            : d_socket(s)
            , d_buffers(b)
            , d_r(std::forward<R>(r))
        {
            std::cout << "receiver_sender::state::state()\n";
        }
        void operator= (state&&) = delete;
        void start() noexcept
        {
            std::cout << "receiver_sender::state::start()\n";
            //-dk:TODO 
            if (!this->d_socket->is_open())
            {
                std::error_code ec;
                execution::set_error(std::move(this->d_r), ec);
                return;
            }
            this->d_socket->context()->add(this->d_socket->native_handle(), POLLIN, this);
        }
    };

    template <typename R>
    state<R> connect(R&& r) &&
    {
        std::cout << "receiver_sender::connect()\n";
        return state<R>(this->d_socket, this->d_buffers, std::forward<R>(r));
    }
};

template <typename Protocol>
template <typename MutableBufferSequence>
cxxrt::net::basic_stream_socket<Protocol>::receive_sender<MutableBufferSequence>
cxxrt::net::basic_stream_socket<Protocol>::async_receive(MutableBufferSequence const& buffers)
{
    return { this, buffers };
}

// ----------------------------------------------------------------------------

#endif
