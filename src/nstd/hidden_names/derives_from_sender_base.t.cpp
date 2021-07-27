// nstd/hidden_names/derives_from_sender_base.t.cpp                   -*-C++-*-
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

#include "nstd/hidden_names/derives_from_sender_base.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace HN = ::nstd::hidden_names;
namespace EX = ::nstd::execution;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct non_sender {};

        struct sender
            : EX::sender_base
        {
        };

        struct typed_sender
            : EX::sender_base
        {
            template <template <typename...> class V, template <typename...> class T>
            using value_types = V<T<>>;
            template <template <typename...> class V>
            using error_types = V<int>;
            static constexpr bool sends_done = false;
        };

    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("class not deriving from sender_base ", []{
            return !HN::derives_from_sender_base<TD::non_sender>;
        }),
    KT::expect_success("sender deriving from sender_base ", []{
            return HN::derives_from_sender_base<TD::sender>;
        }),
    KT::expect_success("typed_sender deriving from sender_base ", []{
            return !HN::derives_from_sender_base<TD::typed_sender>;
        }),
};

static KT::add_tests suite("derives_from_sender_base", ::tests);
