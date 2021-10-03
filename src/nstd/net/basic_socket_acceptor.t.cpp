// nstd/net/basic_socket_acceptor.t.cpp                               -*-C++-*-
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

#include "nstd/net/basic_socket_acceptor.hpp"
#include "nstd/net/basic_stream_socket.hpp"
#include "nstd/net/ip/basic_endpoint.hpp"
#include "nstd/net/ip/make_address_v4.hpp"
#include "nstd/net/ip/tcp.hpp"
#include "nstd/execution/run.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/when_all.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/type_traits/is_same.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace KT = ::kuhl::test;
namespace EX = ::nstd::execution;
namespace NN = ::nstd::net;
namespace NI = ::nstd::net::ip;
namespace UT = ::nstd::utility;
namespace TT = ::nstd::type_traits;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
#if 0
        struct receiver {
            receiver() = default;
            template <typename T>
            receiver(T&&) { static_assert(TT::is_same_v<T, receiver>); }
            friend auto tag_invoke(EX::set_value_t, receiver, auto&&...) noexcept -> void {}
            friend auto tag_invoke(EX::set_error_t, receiver, auto&&) noexcept -> void {}
            friend auto tag_invoke(EX::set_done_t, receiver) noexcept -> void {}
        };
#endif
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("basic construction", []{
            NN::io_context                         context;
            NN::basic_socket_acceptor<NN::ip::tcp> acceptor;
            return KT::use(acceptor)
                && acceptor.is_open() == false
                ;
        }),
    KT::expect_success("construction with protocol", []{
            NN::io_context                         context;
            NN::basic_socket_acceptor<NN::ip::tcp> acceptor(NN::ip::tcp::v4());
            return KT::use(acceptor)
                && acceptor.is_open() == true
                && acceptor.non_blocking() == false
                && acceptor.enable_connection_aborted() == false
                && acceptor.protocol() == NN::ip::tcp::v4()
                ;
        }),
    KT::expect_success("construction with endpoint", []{
            NN::io_context                         context;
            NI::basic_endpoint<NI::tcp>            ep(NI::address_v4::any(), 12345);

            NN::basic_socket_acceptor<NN::ip::tcp> acceptor(ep);
            return KT::use(acceptor)
                && acceptor.is_open() == true
                && acceptor.non_blocking() == false
                && acceptor.enable_connection_aborted() == false
                && acceptor.protocol() == NN::ip::tcp::v4()
                ;
        }),
    KT::expect_success("async_accept", []{
            NN::io_context                         context;
            NI::basic_endpoint<NI::tcp>            ep(NI::address_v4::any(), 12345);

            NN::basic_socket_acceptor<NN::ip::tcp> server(ep);
            NN::basic_stream_socket<NN::ip::tcp>   client(NI::tcp::v4());
            EX::run(context, EX::when_all(
                NN::async_accept(server, context.scheduler()),
                NN::async_connect(client, ep, context.scheduler())
                ));
            return KT::use(server)
                && KT::use(client)
                && EX::sender<decltype(NN::async_accept(server, context.scheduler()))>
                ;
        }),
};

static KT::add_tests suite("basic_socket_acceptor", ::tests);
