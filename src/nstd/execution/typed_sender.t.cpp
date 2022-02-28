// nstd/execution/typed_sender.t.cpp                                  -*-C++-*-
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

#include "nstd/execution/typed_sender.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/sender_base.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace EX = ::nstd::execution;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct typed_sender {
            using completion_signatures = EX::completion_signatures<>;

            template <template <typename...> class T, template <typename...> class V>
            using value_types = V<T<int>, T<>>;
            template <template <typename...> class V>
            using error_types = V<int>;
            static constexpr bool sends_done = false;
        };

        struct sender
            : EX::sender_base
        {
            using completion_signatures = EX::completion_signatures<>;
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("typed_sender is a sender", []{
            return EX::sender<TD::typed_sender>;
        }),
    KT::expect_success("sender is a sender", []{
            return EX::sender<TD::sender>;
        }),
    KT::expect_success("typed_sender is a typed_sender", []{
            return EX::typed_sender<TD::typed_sender>;
        }),
    KT::expect_success("typed_sender lvalue reference is a typed_sender", []{
            return EX::typed_sender<TD::typed_sender&>;
        }),
    KT::expect_success("typed_sender rvalue reference is a typed_sender", []{
            return EX::typed_sender<TD::typed_sender&&>;
        }),
    KT::expect_success("typed_sender const is a typed_sender", []{
            return EX::typed_sender<TD::typed_sender const>;
        }),
    KT::expect_success("typed_sender volatile is a typed_sender", []{
            return EX::typed_sender<TD::typed_sender volatile>;
        }),
    KT::expect_success("typed_sender const volatile is a typed_sender", []{
            return EX::typed_sender<TD::typed_sender const volatile>;
        }),
    KT::expect_success("sender is not a typed_sender", []{
            return !EX::typed_sender<TD::sender>;
        }),
};

static KT::add_tests suite("typed_sender", ::tests);
