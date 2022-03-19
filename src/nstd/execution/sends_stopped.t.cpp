// nstd/execution/sends_stopped.t.cpp                                 -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/execution/sends_stopped.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_value.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace EX = ::nstd::execution;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct env {};
        template <typename... S>
        struct sender {
            using completion_signatures = EX::completion_signatures<S...>;
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("TD::sender is a sender", []{
            using sender = TD::sender<EX::set_value_t(), EX::set_error_t(int), EX::set_stopped_t()>;
            static_assert(EX::sender<TD::sender<>>);
            return EX::sender<TD::sender<>>
                && EX::sender<sender>
                && KT::type<sender::completion_signatures>
                    == KT::type<EX::completion_signatures<EX::set_value_t(), EX::set_error_t(int), EX::set_stopped_t()>>
                ;
        }),
    KT::expect_success("TD::sender<EX::set_value_t()> doesn't send stopped", []{
            return not EX::sends_stopped<TD::sender<EX::set_value_t()>, TD::env>
                ;
        }),
    KT::expect_success("TD::sender<EX::set_stopped_t()> does send stopped", []{
            return EX::sends_stopped<TD::sender<EX::set_stopped_t()>, TD::env>
                ;
        }),
};

static KT::add_tests suite("sends_stopped", ::tests);
