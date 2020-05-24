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

#include <system_error>

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

#endif
