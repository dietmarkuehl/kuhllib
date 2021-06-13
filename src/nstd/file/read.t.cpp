// nstd/file/read.t.cpp                                               -*-C++-*-
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

#include "nstd/file/read.hpp"
#include "nstd/file/open.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/sender/just.hpp"
#include "nstd/sender/then.hpp"
#include "nstd/execution/sync_wait.hpp"
#include <thread>
#include "kuhl/test.hpp"

namespace File = ::nstd::file;
namespace Ex   = ::nstd::execution;
namespace Net  = ::nstd::net;
namespace KT   = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("read usage", []{
            Net::io_context ctxt;
            //-dk:TODO ::std::jthread thread([&cxt]{ ctxt.run(); });
            Ex::sync_wait(Net::just("/dev/null")
                          | File::open(ctxt, File::open_flags::in)
                          | File::read<10>(ctxt)
                          | Net::then([](::std::byte*, ::std::size_t){})
                          );
           return true;
        }),
};

static KT::add_tests suite("read", ::tests);
