// nstd/net/async_read_some.t.cpp                                     -*-C++-*-
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

#include "nstd/net/async_read_some.hpp"
#include "nstd/net/basic_stream_socket.hpp"
#include "nstd/net/ip/tcp.hpp"
#include "nstd/execution/get_completion_signatures.hpp"
#include "nstd/hidden_names/print_completion_signatures.hpp"
#include "kuhl/test.hpp"

namespace EX = ::nstd::execution;
namespace HN = ::nstd::hidden_names;
namespace NN = ::nstd::net;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("breathing", []{
            NN::basic_stream_socket<NN::ip::tcp> socket(NN::ip::tcp::v4());
            char buffer[10];
            auto const sender = NN::async_read_some(socket, NN::buffer(buffer));
            //HN::print_completion_signatures(sender);
            return KT::use(sender);
        }),
};

static KT::add_tests suite("async_read_some", ::tests);