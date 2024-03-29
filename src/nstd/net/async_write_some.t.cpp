// nstd/net/async_write_some.t.cpp                                    -*-C++-*-
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

#include <nstd/net/async_write_some.hpp>
#include <nstd/buffer/const_buffer.hpp>
#include <nstd/execution/schedule.hpp>
#include <nstd/execution/run.hpp>
#include <nstd/execution/on.hpp>
#include <nstd/execution/then.hpp>
#include <nstd/net/io_context.hpp>
#include <nstd/net/basic_stream_socket.hpp>
#include <nstd/net/ip/tcp.hpp>
#include "kuhl/test.hpp"

namespace EX = ::nstd::execution;
namespace NF = ::nstd::file;
namespace NN = ::nstd::net;
namespace NI = ::nstd::net::ip;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("breathing", []{
            char const message[] = { 'h', 'e', 'l', 'l', 'o', '\n' };
            NN::io_context                       context;
            NN::basic_stream_socket<NN::ip::tcp> client(NI::tcp::v4());
            auto sender = EX::on(context.scheduler(),
                      NN::async_write_some(client, NN::buffer(message))
                    | EX::then([](auto){ })
            );
            EX::run(context, sender);
            return KT::use(client)
                && KT::use(sender)
                ;
        }),
};

static KT::add_tests suite("async_write_some", ::tests);
