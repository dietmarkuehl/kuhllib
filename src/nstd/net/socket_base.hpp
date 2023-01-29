// nstd/net/socket_base.hpp                                           -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_SOCKET_BASE
#define INCLUDED_NSTD_NET_SOCKET_BASE

#include "nstd/hidden_names/boolean_socket_option.hpp"
#include "nstd/hidden_names/integer_socket_option.hpp"
#include "nstd/hidden_names/message_flags.hpp"
#include "nstd/hidden_names/shutdown_type.hpp"
#include "nstd/hidden_names/wait_type.hpp"
#include "nstd/file/descriptor.hpp"
#include "nstd/file/socket.hpp"
#include <chrono>

// ----------------------------------------------------------------------------

namespace nstd::net {
    class socket_base;
}

// ----------------------------------------------------------------------------

class nstd::net::socket_base
{
public:
    using native_handle_type = ::nstd::file::descriptor::native_handle_type;

    using broadcast = ::nstd::hidden_names::boolean_socket_option_t<SO_BROADCAST>;
    using debug = ::nstd::hidden_names::boolean_socket_option_t<SO_DEBUG>;
    using do_not_route = ::nstd::hidden_names::boolean_socket_option_t<SO_DONTROUTE>;
    using keep_alive = ::nstd::hidden_names::boolean_socket_option_t<SO_KEEPALIVE>;
    class linger;
    using out_of_band_inline = ::nstd::hidden_names::boolean_socket_option_t<SO_OOBINLINE>;
    using receive_buffer_size = ::nstd::hidden_names::integer_socket_option_t<SO_RCVBUF>;
    using receive_low_watermark = ::nstd::hidden_names::integer_socket_option_t<SO_RCVLOWAT>;
    using reuse_address = ::nstd::hidden_names::boolean_socket_option_t<SO_REUSEADDR>;
    using send_buffer_size = ::nstd::hidden_names::integer_socket_option_t<SO_SNDBUF>;
    using send_low_watermark = ::nstd::hidden_names::integer_socket_option_t<SO_SNDLOWAT>;

    using shutdown_type = ::nstd::hidden_names::shutdown_type;
    static constexpr shutdown_type shutdown_receive = ::nstd::hidden_names::shutdown_type::receive;
    static constexpr shutdown_type shutdown_send    = ::nstd::hidden_names::shutdown_type::send;
    static constexpr shutdown_type shutdown_both    = ::nstd::hidden_names::shutdown_type::both;

    using wait_type = ::nstd::hidden_names::wait_type;
    static constexpr wait_type wait_read  = ::nstd::hidden_names::wait_type::read;
    static constexpr wait_type wait_write = ::nstd::hidden_names::wait_type::write;
    static constexpr wait_type wait_error = ::nstd::hidden_names::wait_type::error;

    using message_flags = ::nstd::hidden_names::message_flags;
    static constexpr message_flags message_peek = ::nstd::hidden_names::message_flags::peek;
    static constexpr message_flags message_out_of_band = ::nstd::hidden_names::message_flags::out_of_band;
    static constexpr message_flags message_do_not_route = ::nstd::hidden_names::message_flags::do_not_route;

    static constexpr int max_listen_connections = SOMAXCONN;

private:
    ::nstd::file::descriptor d_descriptor;
    int                      d_flags{};

protected:
    auto open(int domain, int type, int protocol) -> native_handle_type;

public:
    socket_base() = default;
    socket_base(int fd): d_descriptor(fd) {}
    socket_base(int, int, int);
    socket_base(socket_base const&) = delete;
    socket_base(socket_base&&) = default;
    ~socket_base() = default;
    auto operator= (socket_base const&) -> socket_base& = delete;
    auto operator= (socket_base&&) -> socket_base& = default;

    auto native_handle() const noexcept -> native_handle_type { return this->d_descriptor.get(); }
    auto is_open() const noexcept -> bool { return this->d_descriptor.is_open(); }

    auto non_blocking() const noexcept -> bool;
};

// ----------------------------------------------------------------------------

class nstd::net::socket_base::linger
{
private:
    ::linger d_value;

public:
    constexpr linger() noexcept = default;
    constexpr linger(bool, ::std::chrono::seconds) noexcept;

    auto enabled() const noexcept -> bool;
    auto enabled(bool) noexcept -> void;

    auto timeout() const noexcept -> ::std::chrono::seconds;
    auto timeout(::std::chrono::seconds) noexcept -> void;

    constexpr auto level(auto) const noexcept -> int { return SOL_SOCKET; }
    constexpr auto name(auto) const noexcept -> int { return SO_LINGER; }
    constexpr auto data(auto) noexcept -> void* { return &this->d_value; };
    constexpr auto data(auto) const noexcept -> void const* { return &this->d_value; };
    constexpr auto size(auto) const noexcept -> ::std::size_t;
    constexpr auto resize(auto, ::std::size_t) const noexcept -> void;

};

// ----------------------------------------------------------------------------

#endif
