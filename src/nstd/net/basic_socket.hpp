// nstd/net/basic_socket.hpp                                          -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_BASIC_SOCKET
#define INCLUDED_NSTD_NET_BASIC_SOCKET

#include "nstd/net/netfwd.hpp"
#include "nstd/net/socket_base.hpp"
#include "nstd/file/ring_context.hpp"
#include "nstd/hidden_names/operation.hpp"
#include <iostream>
#include <optional>
#include <system_error>
#include <tuple>
#include <variant>

// ----------------------------------------------------------------------------

namespace nstd::net {
    template <typename> class basic_socket;

    struct async_connect_t {
        template <typename Receiver>
        struct base
            : public ::nstd::file::ring_context::io_base
        {
            Receiver           d_receiver;
            ::sockaddr_storage d_address{};
            ::socklen_t        d_len{sizeof(::sockaddr_storage)};

            template <typename R>
            explicit base(R&& receiver): d_receiver(::nstd::utility::forward<R>(receiver)) {}
            template <typename P, typename S>
            friend auto tag_invoke(::nstd::net::async_connect_t, base&& ob,
                                   ::nstd::net::basic_socket<P>& socket,
                                   S scheduler,
                                   typename ::nstd::net::basic_socket<P>::endpoint_type const& endpoint) -> void
            {
                ob.d_len = endpoint.get_address(&ob.d_address);
                ::std::cout << "scheduling connect\n";
                scheduler.context()->connect(socket.native_handle(),
                                             &ob.d_address,
                                             ob.d_len,
                                             &ob);
            }
            auto do_result(::std::int32_t rc, ::std::uint32_t) -> void override
            {
                if (0 == rc) {
                    ::std::cout << "connect returned success" << rc << "\n";
                    ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver), rc);
                }
                else {
                    ::std::cout << "connect returned: error=" << ::std::error_code(-rc, ::std::system_category()) << "\n";
                    //-dk:TODO are these exceptional or actually results?
                    ::nstd::execution::set_error(::nstd::utility::move(this->d_receiver),
                                                 ::std::error_code(-rc, ::std::system_category()));
                }
            }
        };
    };
    inline constexpr nstd::hidden_names::operation<
        ::nstd::net::async_connect_t,
        ::std::variant<::std::tuple<int>>,
        ::std::variant<::std::error_code, ::std::exception_ptr>
        > async_connect;
}

// ----------------------------------------------------------------------------

template <typename Protocol>
class nstd::net::basic_socket
    : public ::nstd::net::socket_base
{
public:
    using native_handle_type = int;
    using protocol_type = Protocol;
    using endpoint_type = typename protocol_type::endpoint;

private:
    ::std::optional<protocol_type> d_protocol;

public: //-dk:TODO protocted:
    basic_socket();
    basic_socket(protocol_type const&);
    basic_socket(protocol_type const&, native_handle_type const&);

public:
    auto is_open() const noexcept -> bool { return this->::nstd::net::socket_base::is_open(); }
    auto non_blocking() const noexcept -> bool { return false; }
    auto protocol() const noexcept -> protocol_type { return *this->d_protocol; }

    auto open(protocol_type const&) -> void;
};

// ----------------------------------------------------------------------------

template <typename Protocol>
nstd::net::basic_socket<Protocol>::basic_socket()
{
}

template <typename Protocol>
nstd::net::basic_socket<Protocol>::basic_socket(Protocol const& proto)
{
    this->open(proto);
}

template <typename Protocol>
nstd::net::basic_socket<Protocol>::basic_socket(Protocol const& proto,
                                                native_handle_type const& fd)
    : ::nstd::net::socket_base(fd)
    , d_protocol(proto)
{
}

// ----------------------------------------------------------------------------

template <typename Protocol>
auto nstd::net::basic_socket<Protocol>::open(Protocol const& proto)
    -> void
{
    this->d_protocol = proto;
    this->::nstd::net::socket_base::open(proto.family(),
                                         proto.type(),
                                         proto.protocol());
}

// ----------------------------------------------------------------------------

#endif
