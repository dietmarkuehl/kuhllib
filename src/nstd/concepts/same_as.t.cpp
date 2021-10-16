// nstd/concepts/same_as.t.cpp                                        -*-C++-*-
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

#include "nstd/concepts/same_as.hpp"
#include "nstd/type_traits/integral_constant.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace NC = ::nstd::concepts;
namespace TT = ::nstd::type_traits;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        template <int> struct type {};
        struct derived: type<0>{};

        template <typename T0, typename T1>
            requires NC::same_as<T0, T1>
        auto f() -> TT::true_type { return {}; }

        template <typename T0, typename T1>
            requires (not NC::same_as<T0, T1>)
        auto f() -> TT::false_type { return {}; }

        template <typename T0, typename T1>
            requires NC::same_as<T1, T0>
                && NC::same_as<int, T0>
        auto f() -> int { return 17; }

        template <typename T0, typename T1>
        auto f() -> void {}
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("basic operation", []{
            return NC::same_as<int, int>
                && NC::same_as<TD::type<0>, TD::type<0>>
                && not NC::same_as<TD::type<0>, TD::type<1>>
                && not NC::same_as<TD::type<0>, TD::derived>
                ;
        }),
    KT::expect_success("use in constraints", []{
            return true
                && TD::f<int, int>()
                && TD::f<TD::type<0>, TD::type<0>>()
                && not TD::f<TD::type<0>, TD::type<1>>()
                && not TD::f<int, TD::type<1>>()
                && 17 == TD::f<int, int>()
                ;
        }),
};

static KT::add_tests suite("same_as", ::tests);
