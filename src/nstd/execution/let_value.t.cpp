// nstd/execution/let_value.t.cpp                                     -*-C++-*-
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

#include "nstd/execution/let_value.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace KT = ::kuhl::test;
namespace EX = ::nstd::execution;
namespace TT = ::nstd::this_thread;
namespace UT = ::nstd::utility;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("breathing", []{
        ::std::optional<::std::variant<::std::string>> value;
            auto sender = EX::let_value(EX::just(::std::string("hello, "), ::std::string("world")),
                                    [](auto&&... a){ return EX::just((a + ...)); })
                                    ;
            value = TT::sync_wait(UT::move(sender));
            return KT::use(sender)
                && value
                && value->index() == 0
                && ::std::get<0>(*value) == "hello, world"
                ;
        }),
};

static KT::add_tests suite("let_value", ::tests);
