// nstd/file/open.t.cpp                                               -*-C++-*-
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

#include "nstd/file/open.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/sender/just.hpp"
#include "nstd/sender/then.hpp"
#include "nstd/execution/sync_wait.hpp"
#include "kuhl/test.hpp"
#include <thread>

namespace File = ::nstd::file;
namespace NET  = ::nstd::net;
namespace KT   = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("file open() existing file", []{
            bool            value(false);
            NET::io_context ctxt;
            ::std::jthread thread([&ctxt]{ ctxt.run_one(); });
            auto x = NET::just("/dev/null")
                   | File::open_in(ctxt)
                   | NET::then([&value](File::descriptor&&){ value = true; })
                   ;
            ::nstd::execution::sync_wait(x);
            return value;
        }),
    KT::expect_success("file open() non-existing file", []{
            bool            value(false);
            NET::io_context ctxt;
            ::std::jthread thread([&ctxt]{ ctxt.run_one(); });
            auto x = NET::just("/dev/nill")
                   | File::open_in(ctxt)
                   | NET::then([&value](File::descriptor&&){ value = true; })
                   ;
            try {
                ::nstd::execution::sync_wait(x);
                return false;
            }
            catch (::std::system_error const&) {
                return value == false;
            }
            catch (...) {
                return false;
            }
        }),
};

static KT::add_tests suite("file", ::tests);
