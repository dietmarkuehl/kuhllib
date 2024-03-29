// nstd/net/basic_stream_socket.hpp                                   -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_NET_BASIC_STREAM_SOCKET
#define INCLUDED_NSTD_NET_BASIC_STREAM_SOCKET

#include "nstd/net/netfwd.hpp"
#include "nstd/net/async_read_some.hpp"
#include "nstd/net/async_write_some.hpp"
#include "nstd/net/basic_socket.hpp"
#include "nstd/file/operation.hpp"
#include "nstd/container/intrusive_list.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/schedule.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/repeat_effect_until.hpp"
#include "nstd/execution/let.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/buffer/const_buffer.hpp"
#include "nstd/buffer/mutable_buffer.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/file/socket.hpp"
#include <vector>

// ----------------------------------------------------------------------------

namespace nstd::net {
    template <typename> class basic_stream_socket;
}

// ----------------------------------------------------------------------------

template <typename Protocol>
class nstd::net::basic_stream_socket
    : public ::nstd::net::basic_socket<Protocol>
{
private:
    ::nstd::container::intrusive_list<::nstd::file::operation_send_base> d_send_queue;

public:
    using protocol_type = Protocol;
    using endpoint_type = typename protocol_type::endpoint;
    using native_handle_type = typename ::nstd::net::basic_socket<Protocol>::native_handle_type;

    basic_stream_socket();
    explicit basic_stream_socket(protocol_type const&);
    explicit basic_stream_socket(endpoint_type const&);
    basic_stream_socket(protocol_type const&, native_handle_type const&);
    basic_stream_socket(basic_stream_socket const&) = delete;
    basic_stream_socket(basic_stream_socket &&) = default;
    auto operator=(basic_stream_socket const&) -> basic_stream_socket& = delete;
    auto operator=(basic_stream_socket &&) -> basic_stream_socket& = default;

    template <typename ConstantBufferSequence>
    auto enqueue(::nstd::file::operation_send<ConstantBufferSequence>&) -> void;
    template <typename ConstantBufferSequence>
    auto complete(::nstd::file::operation_send<ConstantBufferSequence>&) -> void;

    template <typename MutableBufferSequence>
    auto read_some(MutableBufferSequence const&) -> ::std::size_t;
    template <typename MutableBufferSequence>
    auto read_some(MutableBufferSequence const&, ::std::error_code&) -> ::std::size_t;

    template <typename MutableBufferSequence>
    auto receive(MutableBufferSequence const&) -> ::std::size_t;
    template <typename MutableBufferSequence>
    auto receive(MutableBufferSequence const&, ::std::error_code&) -> ::std::size_t;
    template <typename MutableBufferSequence>
    auto receive(MutableBufferSequence const&, ::nstd::net::socket_base::message_flags) -> ::std::size_t;
    template <typename MutableBufferSequence>
    auto receive(MutableBufferSequence const&, ::nstd::net::socket_base::message_flags, ::std::error_code&) -> ::std::size_t;

    template <typename ConstBufferSequence>
    auto write_some(ConstBufferSequence const&) -> ::std::size_t;
    template <typename ConstBufferSequence>
    auto write_some(ConstBufferSequence const&, ::std::error_code&) -> ::std::size_t;

    template <typename ConstBufferSequence>
    auto send(ConstBufferSequence const&) -> ::std::size_t;
    template <typename ConstBufferSequence>
    auto send(ConstBufferSequence const&, ::std::error_code&) -> ::std::size_t;
    template <typename ConstBufferSequence>
    auto send(ConstBufferSequence const&, ::nstd::net::socket_base::message_flags) -> ::std::size_t;
    template <typename ConstBufferSequence>
    auto send(ConstBufferSequence const&, ::nstd::net::socket_base::message_flags, ::std::error_code&) -> ::std::size_t;
};

// ----------------------------------------------------------------------------

template <typename Protocol>
nstd::net::basic_stream_socket<Protocol>::basic_stream_socket()
    : ::nstd::net::basic_socket<Protocol>()
{
}

template <typename Protocol>
nstd::net::basic_stream_socket<Protocol>::basic_stream_socket(protocol_type const& protocol)
    : ::nstd::net::basic_socket<Protocol>(protocol)
{
}

template <typename Protocol>
nstd::net::basic_stream_socket<Protocol>::basic_stream_socket(protocol_type const& protocol,
                                                              native_handle_type const& fd)
    : ::nstd::net::basic_socket<Protocol>(protocol, fd)
{
}

// ----------------------------------------------------------------------------

template <typename Protocol>
    template <typename ConstantBufferSequence>
auto nstd::net::basic_stream_socket<Protocol>::enqueue(::nstd::file::operation_send<ConstantBufferSequence>& op) -> void
{
    this->d_send_queue.push_back(op);
    if (&op == &this->d_send_queue.front()) {
        op.submit();
    }
}

template <typename Protocol>
    template <typename ConstantBufferSequence>
auto nstd::net::basic_stream_socket<Protocol>::complete(::nstd::file::operation_send<ConstantBufferSequence>& op) -> void
{
    bool front(&this->d_send_queue.front() == &op);
    this->d_send_queue.erase(&op);
    if (front && !this->d_send_queue.empty()) {
        this->d_send_queue.front().submit();
    }
}

// ----------------------------------------------------------------------------

template <typename P>
template <typename MutableBufferSequence>
auto nstd::net::basic_stream_socket<P>::receive(MutableBufferSequence const& buffers)
    -> ::std::size_t
{
    ::std::error_code ec;
    std::size_t rc = this->receive(buffers, ec);
    if (ec) {
        throw ::std::system_error(ec);
    }
    return rc;
}

template <typename P>
template <typename MutableBufferSequence>
auto nstd::net::basic_stream_socket<P>::receive(MutableBufferSequence const& buffers,
                                                  ::std::error_code&           ec)
    -> ::std::size_t
{
    return this->receive(buffers, ::nstd::net::socket_base::message_flags(), ec);
}

template <typename P>
template <typename MutableBufferSequence>
auto nstd::net::basic_stream_socket<P>::receive(MutableBufferSequence const&            buffers,
                                                ::nstd::net::socket_base::message_flags flags)
    -> ::std::size_t
{
    ::std::error_code ec;
    std::size_t rc = this->receive(buffers, flags, ec);
    if (ec) {
        throw ::std::system_error(ec);
    }
    return rc;
}

template <typename P>
template <typename MutableBufferSequence>
auto nstd::net::basic_stream_socket<P>::receive(MutableBufferSequence const&            buffers,
                                                ::nstd::net::socket_base::message_flags flags,
                                                ::std::error_code&                      ec)
    -> ::std::size_t
{
    //-dk:TODO: if (buffer_size(buffers) == 0) { return ::std::size_t{}; }

    auto size = ::std::distance(::nstd::net::buffer_sequence_begin(buffers), ::nstd::net::buffer_sequence_end(buffers));
    ::std::vector<::iovec> v;
    for (auto it = ::nstd::net::buffer_sequence_begin(buffers),
              end = ::nstd::net::buffer_sequence_end(buffers); it != end; ++it) {
        ::iovec vec;
        vec.iov_base = it->data();
        vec.iov_len  = it->size();
        v.push_back(vec);
    }

    ::msghdr message{};
    message.msg_name = nullptr;
    message.msg_namelen = 0u;
    message.msg_iov = v.data();
    message.msg_iovlen = size;
    message.msg_control = nullptr;
    message.msg_controllen = 0u;
    message.msg_flags = 0u;

    auto rc = ::recvmsg(this->native_handle(), &message, static_cast<int>(flags));
    if (rc < 0) {
        ec = ::std::error_code(errno, ::std::system_category());
        return 0u;
    }

    return rc;
}

// ----------------------------------------------------------------------------

template <typename P>
template <typename MutableBufferSequence>
auto nstd::net::basic_stream_socket<P>::read_some(MutableBufferSequence const& buffers)
    -> ::std::size_t
{
    ::std::error_code ec;
    std::size_t rc = this->read_some(buffers, ec);
    if (ec) {
        throw ::std::system_error(ec);
    }
    return rc;
}

template <typename P>
template <typename MutableBufferSequence>
auto nstd::net::basic_stream_socket<P>::read_some(MutableBufferSequence const& buffers,
                                                  ::std::error_code&           ec)
    -> ::std::size_t
{
    return this->receive(buffers, ec);
}

// ----------------------------------------------------------------------------

template <typename P>
template <typename ConstBufferSequence>
auto nstd::net::basic_stream_socket<P>::write_some(ConstBufferSequence const& buffers) -> ::std::size_t
{
    ::std::error_code ec;
    std::size_t rc = this->send(buffers, ec);
    if (ec) {
        throw ::std::system_error(ec);
    }
    return rc;
}

template <typename P>
template <typename ConstBufferSequence>
auto nstd::net::basic_stream_socket<P>::write_some(ConstBufferSequence const& buffers, ::std::error_code& ec) -> ::std::size_t
{
    return this->send(buffers, ec);
}

// ----------------------------------------------------------------------------

template <typename P>
template <typename ConstBufferSequence>
auto nstd::net::basic_stream_socket<P>::send(ConstBufferSequence const& buffers) -> ::std::size_t
{
    ::std::error_code ec;
    std::size_t rc = this->send(buffers, ec);
    if (ec) {
        throw ::std::system_error(ec);
    }
    return rc;
}

template <typename P>
template <typename ConstBufferSequence>
auto nstd::net::basic_stream_socket<P>::send(ConstBufferSequence const& buffers, ::std::error_code& ec) -> ::std::size_t
{
    return this->send(buffers, ::nstd::net::socket_base::message_flags{}, ec);
}

template <typename P>
template <typename ConstBufferSequence>
auto nstd::net::basic_stream_socket<P>::send(ConstBufferSequence const& buffers, ::nstd::net::socket_base::message_flags flags) -> ::std::size_t
{
    ::std::error_code ec;
    std::size_t rc = this->send(buffers, flags, ec);
    if (ec) {
        throw ::std::system_error(ec);
    }
    return rc;
}

template <typename P>
template <typename ConstBufferSequence>
auto nstd::net::basic_stream_socket<P>::send(ConstBufferSequence const&              buffers,
                                             ::nstd::net::socket_base::message_flags flags,
                                             ::std::error_code&                      ec)
    -> ::std::size_t
{
    //-dk:TODO: if (buffer_size(buffers) == 0) { return ::std::size_t{}; }

    auto size = ::std::distance(::nstd::net::buffer_sequence_begin(buffers), ::nstd::net::buffer_sequence_end(buffers));
    ::std::vector<::iovec> v;
    for (auto it = ::nstd::net::buffer_sequence_begin(buffers),
              end = ::nstd::net::buffer_sequence_end(buffers); it != end; ++it) {
        ::iovec vec;
        vec.iov_base = it->data();
        vec.iov_len  = it->size();
        v.push_back(vec);
    }

    ::msghdr message{};
    message.msg_name = nullptr;
    message.msg_namelen = 0u;
    message.msg_iov = v.data();
    message.msg_iovlen = size;
    message.msg_control = nullptr;
    message.msg_controllen = 0u;
    message.msg_flags = 0u;

    auto rc = ::sendmsg(this->native_handle(), &message, static_cast<int>(flags));
    if (rc < 0) {
        ec = ::std::error_code(errno, ::std::system_category());
        return 0u;
    }

    return rc;
}

// ----------------------------------------------------------------------------

#endif
