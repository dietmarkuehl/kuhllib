// nstd/net/async_accept.t.cpp                                        -*-C++-*-
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

#include "nstd/net/async_accept.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/file/test_context.hpp"
#include "nstd/execution/schedule.hpp"
#include "nstd/execution/start_detached.hpp"
#include "nstd/execution/then.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace EX = ::nstd::execution;
namespace KT = ::kuhl::test;
namespace NF = ::nstd::file;
namespace NI = ::nstd::net::ip;
namespace NN = ::nstd::net;
namespace TD = test_declarations;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    namespace {
        class stream
        {
        public:
            using protocol_type = int;
            using native_handle_type = int;

            stream() = default;
            stream(protocol_type, native_handle_type) {}
        };

        class acceptor
        {
        public:
            using socket_type = TD::stream;
            using native_handle_type = int;

            auto protocol() const -> socket_type::protocol_type { return {}; }
            auto native_handle() const -> native_handle_type { return {}; }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("breathing", []{
            NF::test_context test_context;
            NN::io_context   context(&test_context);
            test_context.on_accept = [&test_context](int, ::sockaddr*, ::socklen_t*, int, NF::context::io_base* cont){
                test_context.make_ready(1, 0, cont);
            };
            test_context.on_nop = [&test_context](NF::context::io_base* cont){
                test_context.make_ready(0, 0, cont);
            };

            bool predecessor_called(false);
            bool completion_called(false);
            TD::acceptor acceptor;
            auto accept
                = EX::schedule(context.scheduler())
                | EX::then([&]{ predecessor_called = true; })
                | NN::async_accept(acceptor)
                | EX::then([&](auto, TD::stream){ completion_called = true; })
                ;
            EX::start_detached(accept);
            auto rc(context.run());
            return true
                && KT::use(acceptor)
                && KT::use(accept)
                && rc == 2
                && predecessor_called
                && completion_called
                ;
        }),
};

static KT::add_tests suite("async_accept", ::tests);
