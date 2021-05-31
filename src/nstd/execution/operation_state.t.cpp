// nstd/execution/operation_state.t.cpp                               -*-C++-*-
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

#include "nstd/execution/operation_state.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace EX = ::nstd::execution;
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    struct non_state {};

    struct non_destructible_state {
        ~non_destructible_state() = delete;
        auto start() & noexcept -> void {}
    };

    template <bool Noexcept>
    struct state {
        auto start() & noexcept(Noexcept) -> void {}
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("random object isn't a state", []{
           return !EX::operation_state<TD::non_state>;
        }),
    KT::expect_success("non-destructible object isn't a state", []{
           return !EX::operation_state<TD::non_destructible_state>;
        }),
    KT::expect_success("object with throwing start isn't a state", []{
           return !EX::operation_state<TD::state<false>>;
        }),
    KT::expect_success("reference to a state", []{
           return !EX::operation_state<TD::state<true>&>;
        }),
    KT::expect_success("destructible object with non-throwing start() is a state", []{
           return EX::operation_state<TD::state<true>>;
        }),
};

static KT::add_tests suite("operation_state", ::tests);
