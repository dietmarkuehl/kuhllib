// nstd/net/basic_stream_socket.t.cpp                                 -*-C++-*-
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

#include "nstd/net/basic_stream_socket.hpp"
#include "nstd/net/basic_socket_acceptor.hpp"
#include "nstd/net/ip/basic_endpoint.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/buffer/const_buffer.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/execution/when_all.hpp"
#include "nstd/execution/run.hpp"
#include "nstd/net/ip/tcp.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"
#include <concepts>

namespace test_declarations {}
namespace TD = test_declarations;
namespace KT = ::kuhl::test;
namespace NN = ::nstd::net;
namespace NI = NN::ip;
namespace EX = ::nstd::execution;
namespace UT = ::nstd::utility;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("base class", []{
            return ::std::derived_from<NN::basic_stream_socket<NI::tcp>,
                                       NN::basic_stream_socket<NI::tcp>>
                ;
        }),
    KT::expect_success("async_write_some", []{
            char const message[] = { 'h', 'e', 'l', 'l', 'o', '\n' };

            NN::io_context                         context;
            NI::basic_endpoint<NI::tcp>            ep(NI::address_v4::any(), 12345);

            NN::basic_socket_acceptor<NN::ip::tcp> accept(ep);
            NN::basic_stream_socket<NN::ip::tcp>   server;
            NN::basic_stream_socket<NN::ip::tcp>   client(NI::tcp::v4());

            EX::run(context, EX::when_all(
                NN::async_accept(accept, context.scheduler())
                | EX::then([&server](NN::basic_stream_socket<NN::ip::tcp>&& stream){
                    std::cout << "accepted\n";
                    server = UT::move(stream);
                    }),
                NN::async_connect(client, context.scheduler(), ep)
                ));

            auto write = NN::async_write_some(client, context.scheduler(), NN::buffer(message));
            EX::run(context, UT::move(write));

            return KT::use(accept)
                && KT::use(server)
                && KT::use(client)
                && KT::use(write)
                && EX::sender<decltype(NN::async_accept(accept, context.scheduler()))>
                && EX::sender<decltype(write)>
                ;
        }),
};

static KT::add_tests suite("basic_stream_socket", ::tests);
