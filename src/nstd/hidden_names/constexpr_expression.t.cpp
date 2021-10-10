// nstd/hidden_names/constexpr_expression.t.cpp                       -*-C++-*-
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

#include "nstd/hidden_names/constexpr_expression.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace HN = ::nstd::hidden_names;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        template <typename T>
        auto non_constexpr_fun() -> T { return T(); }
        template <typename T>
        constexpr auto constexpr_fun() -> T { return T(); }

        template <typename T>
        concept is_non_constexpr = requires { typename HN::constexpr_expression<TD::non_constexpr_fun<T>()>; };
        template <typename T>
        concept is_constexpr = requires { typename HN::constexpr_expression<TD::constexpr_fun<T>()>; };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("integer literals are constant expressions", []{
            return requires { typename HN::constexpr_expression<true>; }
                && requires { typename HN::constexpr_expression<17>; }
                ;
        }),
    KT::expect_success("non-constexpr functions are not constant expressions", []{
            return not TD::is_non_constexpr<bool>
                && not TD::is_non_constexpr<int>
                ;
        }),
    KT::expect_success("constexpr functions are constant expressions", []{
            return TD::is_constexpr<bool>
                && TD::is_constexpr<int>
                ;
        }),
};

static KT::add_tests suite("constexpr_expression", ::tests);
