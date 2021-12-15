// nstd/execution/upon_done.t.cpp                                     -*-C++-*-
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

#include "nstd/execution/upon_done.hpp"
#include "nstd/execution/just_done.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"

namespace EX = ::nstd::execution;
namespace TT = ::nstd::this_thread;
namespace UT = ::nstd::utility;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("breathing", []{
            bool called{false};
            auto just = EX::just_done();
            auto sender = EX::upon_done(UT::move(just), [&called]{ called = true; return true; });
            auto rc = TT::sync_wait(UT::move(sender));

            return KT::use(sender)
                && KT::use(rc)
                && called
                ;
        }),
    KT::expect_success("pipe", []{
            bool first_then_called{false};
            bool second_then_called{false};
            bool upon_done_called{false};
            auto sender
                = EX::just_done()
                | EX::then([&first_then_called]{ first_then_called = true; return true; })
                | EX::upon_done([&upon_done_called]{ upon_done_called = true; return true; })
                | EX::then([&second_then_called](auto&&...){ second_then_called = true; return true; })
                ;
            auto rc = TT::sync_wait(UT::move(sender));

            return KT::use(sender)
                && KT::use(rc)
                && not first_then_called
                && second_then_called
                && upon_done_called
                ;
        }),
};

static KT::add_tests suite("just_done", ::tests);