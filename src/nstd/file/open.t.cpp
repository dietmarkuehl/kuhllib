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
#include "nstd/execution/just.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"
#include <string_view>
#include <thread>

namespace test_declarations {}
namespace TD   = ::test_declarations;
namespace File = ::nstd::file;
namespace EX   = ::nstd::execution;
namespace TT   = ::nstd::this_thread;
namespace UT   = ::nstd::utility;
namespace NET  = ::nstd::net;
namespace KT   = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    inline namespace {
        struct jthread
            : ::std::thread {
            using ::std::thread::thread;
            ~jthread() { this->join(); }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("file open() existing file", []{
            bool            value(false);
            NET::io_context ctxt;
            TD::jthread thread([&ctxt]{ ctxt.run_one(); });
            auto x = EX::just(::std::string_view("/dev/null"))
                   | File::open_in(ctxt)
                   | EX::then([&value](File::descriptor&&){ value = true; })
                   ;
            TT::sync_wait(UT::move(x));
            return value;
        }),
    KT::expect_success("file open() non-existing file", []{
            bool            value(false);
            NET::io_context ctxt;
            TD::jthread thread([&ctxt]{ ctxt.run_one(); });
            auto x = EX::just(::std::string_view("/dev/nill"))
                   | File::open_in(ctxt)
                   | EX::then([&value](File::descriptor&&){ value = true; })
                   ;
            try {
                TT::sync_wait(UT::move(x));
                return false;
            }
            catch (::std::error_code const&) {
                return value == false;
            }
            catch (::std::system_error const&) {
                return value == false;
            }
            catch (...) {
                return false;
            }
        }),
};

static KT::add_tests suite("open", ::tests);
