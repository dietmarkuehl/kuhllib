// nstd/net/socket_base.t.cpp                                         -*-C++-*-
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

#include "nstd/net/socket_base.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace NN = ::nstd::net;
namespace HN = ::nstd::hidden_names;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("broadcast", []{
        return HN::boolean_socket_option<NN::socket_base::broadcast>
            && NN::socket_base::broadcast().level(0) == SOL_SOCKET
            && NN::socket_base::broadcast().name(0) == SO_BROADCAST
            && KT::type<NN::socket_base::broadcast::type> == KT::type<int>
            ;
    }),
    KT::expect_success("debug", []{
        return HN::boolean_socket_option<NN::socket_base::debug>
            && NN::socket_base::debug().level(0) == SOL_SOCKET
            && NN::socket_base::debug().name(0) == SO_DEBUG
            && KT::type<NN::socket_base::debug::type> == KT::type<int>
            ;
    }),
    KT::expect_success("do_not_route", []{
        return HN::boolean_socket_option<NN::socket_base::do_not_route>
            && NN::socket_base::do_not_route().level(0) == SOL_SOCKET
            && NN::socket_base::do_not_route().name(0) == SO_DONTROUTE
            && KT::type<NN::socket_base::do_not_route::type> == KT::type<int>
            ;
    }),
    KT::expect_success("keep_alive", []{
        return HN::boolean_socket_option<NN::socket_base::keep_alive>
            && NN::socket_base::keep_alive().level(0) == SOL_SOCKET
            && NN::socket_base::keep_alive().name(0) == SO_KEEPALIVE
            && KT::type<NN::socket_base::keep_alive::type> == KT::type<int>
            ;
    }),
    KT::expect_success("linger", []{
        return true
            && NN::socket_base::linger().level(0) == SOL_SOCKET
            && NN::socket_base::linger().name(0) == SO_LINGER
            ;
    }),
    KT::expect_success("out_of_band_inline", []{
        return HN::boolean_socket_option<NN::socket_base::out_of_band_inline>
            && NN::socket_base::out_of_band_inline().level(0) == SOL_SOCKET
            && NN::socket_base::out_of_band_inline().name(0) == SO_OOBINLINE
            && KT::type<NN::socket_base::out_of_band_inline::type> == KT::type<int>
            ;
    }),
    KT::expect_success("receiver_buffer_size", []{
        return HN::integer_socket_option<NN::socket_base::receive_buffer_size>
            && NN::socket_base::receive_buffer_size().level(0) == SOL_SOCKET
            && NN::socket_base::receive_buffer_size().name(0) == SO_RCVBUF
            && KT::type<NN::socket_base::receive_buffer_size::type> == KT::type<int>
            ;
    }),
    KT::expect_success("receive_low_watermark", []{
        return HN::integer_socket_option<NN::socket_base::receive_low_watermark>
            && NN::socket_base::receive_low_watermark().level(0) == SOL_SOCKET
            && NN::socket_base::receive_low_watermark().name(0) == SO_RCVLOWAT
            && KT::type<NN::socket_base::send_low_watermark::type> == KT::type<int>
            ;
    }),
    KT::expect_success("reuse_address", []{
        return HN::boolean_socket_option<NN::socket_base::reuse_address>
            && NN::socket_base::reuse_address().level(0) == SOL_SOCKET
            && NN::socket_base::reuse_address().name(0) == SO_REUSEADDR
            && KT::type<NN::socket_base::reuse_address::type> == KT::type<int>
            ;
    }),
    KT::expect_success("send_buffer_size", []{
        return HN::integer_socket_option<NN::socket_base::send_buffer_size>
            && NN::socket_base::send_buffer_size().level(0) == SOL_SOCKET
            && NN::socket_base::send_buffer_size().name(0) == SO_SNDBUF
            && KT::type<NN::socket_base::send_buffer_size::type> == KT::type<int>
            ;
    }),
    KT::expect_success("send_low_watermark", []{
        return HN::integer_socket_option<NN::socket_base::send_low_watermark>
            && NN::socket_base::send_low_watermark().level(0) == SOL_SOCKET
            && NN::socket_base::send_low_watermark().name(0) == SO_SNDLOWAT
            && KT::type<NN::socket_base::send_low_watermark::type> == KT::type<int>
            ;
    }),
    KT::expect_success("shutdown_type", []{
        return KT::type<NN::socket_base::shutdown_type const>
                == KT::type<decltype(NN::socket_base::shutdown_receive)>
            && KT::type<NN::socket_base::shutdown_type const>
                == KT::type<decltype(NN::socket_base::shutdown_send)>
            && KT::type<NN::socket_base::shutdown_type const>
                == KT::type<decltype(NN::socket_base::shutdown_both)>
            ;
    }),
    KT::expect_success("wait_type", []{
        return KT::type<NN::socket_base::wait_type const>
                == KT::type<decltype(NN::socket_base::wait_read)>
            && KT::type<NN::socket_base::wait_type const>
                == KT::type<decltype(NN::socket_base::wait_write)>
            && KT::type<NN::socket_base::wait_type const>
                == KT::type<decltype(NN::socket_base::wait_error)>
            ;
    }),
    KT::expect_success("message_flags", []{
        return KT::type<NN::socket_base::message_flags const>
                == KT::type<decltype(NN::socket_base::message_peek)>
            && KT::type<NN::socket_base::message_flags const>
                == KT::type<decltype(NN::socket_base::message_out_of_band)>
            && KT::type<NN::socket_base::message_flags const>
                == KT::type<decltype(NN::socket_base::message_do_not_route)>
            ;
    }),
    KT::expect_success("max_connections", []{
        return NN::socket_base::max_listen_connections == SOMAXCONN;
    }),
    KT::expect_success("breathing", []{
            NN::socket_base base;
            return KT::use(base)
                ;
        }),
};

static KT::add_tests suite("socket_base", ::tests);
