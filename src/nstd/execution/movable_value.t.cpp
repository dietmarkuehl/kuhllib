// nstd/execution/movable_value.t.cpp                                 -*-C++-*-
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

#include "nstd/execution/movable_value.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace EX = ::nstd::execution;
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    struct movable {
        movable(movable const&) = default;
        movable(movable&&) = default;
    };

    struct non_lvalue_constructible {
        non_lvalue_constructible(non_lvalue_constructible&) = delete;
        non_lvalue_constructible(non_lvalue_constructible const&) = default;
        non_lvalue_constructible(non_lvalue_constructible&&) = default;
    };

    struct non_const_lvalue_constructible {
        non_const_lvalue_constructible(non_const_lvalue_constructible const&) = delete;
        non_const_lvalue_constructible(non_const_lvalue_constructible&) = default;
        non_const_lvalue_constructible(non_const_lvalue_constructible&&) = default;
    };

    struct non_movable {
        non_movable(non_movable&&) = delete;
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("movable type", [](KT::context& ){
        return EX::movable_value<TD::movable>
            && EX::movable_value<TD::movable const>
            ;
    }),
    KT::expect_success("movable l-value reference", [](KT::context& ){
        return EX::movable_value<TD::movable&>
            && EX::movable_value<TD::movable const&>
            ;
    }),
    KT::expect_success("movable r-value reference", [](KT::context& ){
        return EX::movable_value<TD::movable&&>
            && EX::movable_value<TD::movable const&&>
            ;
    }),
    KT::expect_success("non-movable type", [](KT::context& ){
        return !EX::movable_value<TD::non_movable>
            && !EX::movable_value<TD::non_movable const>
            ;
    }),
    KT::expect_success("non-movable l-value reference", [](KT::context& ){
        return !EX::movable_value<TD::non_movable&>
            && !EX::movable_value<TD::non_movable const&>
            ;
    }),
    KT::expect_success("non-movable r-value reference", [](KT::context& ){
        return !EX::movable_value<TD::non_movable&&>
            && !EX::movable_value<TD::non_movable const&&>
            ;
    }),
    KT::expect_success("non-lvalue constructible type", [](KT::context& ){
        return EX::movable_value<TD::non_lvalue_constructible>
            && EX::movable_value<TD::non_lvalue_constructible const>
            ;
    }),
    KT::expect_success("non-lvalue constructible l-value reference", [](KT::context& ){
        return !EX::movable_value<TD::non_lvalue_constructible&>
            && EX::movable_value<TD::non_lvalue_constructible const&>
            ;
    }),
    KT::expect_success("non-lvalue constructible r-value reference", [](KT::context& ){
        return EX::movable_value<TD::non_lvalue_constructible&&>
            && EX::movable_value<TD::non_lvalue_constructible const&&>
            ;
    }),
    KT::expect_success("non-const-lvalue constructible type", [](KT::context& ){
        return EX::movable_value<TD::non_const_lvalue_constructible>
            && !EX::movable_value<TD::non_const_lvalue_constructible const>
            ;
    }),
    KT::expect_success("non-const-lvalue constructible l-value reference", [](KT::context& ){
        return EX::movable_value<TD::non_const_lvalue_constructible&>
            && !EX::movable_value<TD::non_const_lvalue_constructible const&>
            ;
    }),
    KT::expect_success("non-const-lvalue constructible r-value reference", [](KT::context& ){
        return EX::movable_value<TD::non_const_lvalue_constructible&&>
            && !EX::movable_value<TD::non_const_lvalue_constructible const&&>
            ;
    }),
};

static KT::add_tests suite("movable_value", ::tests);
