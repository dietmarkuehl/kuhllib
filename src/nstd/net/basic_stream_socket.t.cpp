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
#include "nstd/execution/schedule.hpp"
#include "nstd/execution/when_all.hpp"
#include "nstd/execution/run.hpp"
#include "nstd/net/ip/tcp.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"
#include <concepts>

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace NF = ::nstd::file;
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
            ::std::cout << ::std::unitbuf;
            char const message[] = { 'h', 'e', 'l', 'l', 'o', '\n' };
            char       buffer[1024];

            NN::io_context                         context;
            NI::basic_endpoint<NI::tcp>            ep(NI::address_v4::any(), 12345);

            NN::basic_socket_acceptor<NN::ip::tcp> accept(ep);
            NN::basic_stream_socket<NN::ip::tcp>   server;
            NN::basic_stream_socket<NN::ip::tcp>   client(NI::tcp::v4());

            EX::run(context,
                    EX::schedule(context.scheduler())
                  | EX::then([]{}) //-dk:TODO remove
                  | NN::async_connect(client, ep)
                  | NN::async_accept(accept)
                  | EX::then([&server](::std::error_code, NN::basic_stream_socket<NN::ip::tcp>&& stream){
                      server = UT::move(stream);
                      })
                  );

            auto write = EX::schedule(context.scheduler())
                       | EX::then([]{}) //-dk:TODO remove
                       | NN::async_write_some(client, NN::buffer(message))
                       ;
            auto read = EX::schedule(context.scheduler())
                      | EX::then([]{}) //-dk:TODO remove
                      | NN::async_read_some(server, NN::buffer(buffer))
                      | EX::then([](::std::int64_t){})
                      ;
            EX::run(context, EX::when_all(UT::move(read), UT::move(write)));

            return KT::use(accept)
                && KT::use(server)
                && KT::use(client)
                && KT::use(write)
                && EX::sender<decltype(NN::async_accept(EX::schedule(context.scheduler()), accept))>
                && EX::sender<decltype(write)>
                ;
        }),
    KT::expect_success("async_write", []{
            char message[] = { 'h', 'e', 'l', 'l', 'o', '\n' };
            NN::basic_stream_socket<NN::ip::tcp>  client(NI::tcp::v4());
            NN::io_context                        context;
            auto write
                = EX::schedule(context.scheduler())
                | EX::then([]{}) //-dk:TODO remove
                | NN::async_write(client, NN::buffer(message))
                ;
            return true
                && KT::use(client)
                && KT::use(write)
                ;
        }),
};

static KT::add_tests suite("basic_stream_socket", ::tests);
