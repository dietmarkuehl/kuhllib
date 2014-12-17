// nstd/functional/not_.t.cpp                                         -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2014 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/functional/not_.hpp"
#include "kuhl/test.hpp"

namespace NF = nstd::functional;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct bar {
        bar() = default;
        bar(bar const&) = default;
        bar(bar&&) = delete;
        auto operator()(int value) -> int { return value; }
    };
    struct foo {
        bool flip;
        foo(bool flip): flip(flip) {}
        auto operator()(bool value) const -> bool {
            return flip? !value: value;
        }
    };

    struct foobar {
        auto operator()(bool b, int i, char c) -> bool {
            return b? i: c;
        }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("unary negate", [](KT::context& c)->bool{
            bar b;
            NF::n_ary_negate<bar> neg(b);
            return KT::assert_false(c, "17", neg(17))
                && KT::assert_true(c, "0", neg(0))
                && KT::assert_type<bool, decltype(neg(0))>(c, "neg(0) yields bool")
                ;
        }),
    KT::expect_success("ternary negate", [](KT::context& c)->bool{
            NF::n_ary_negate<foobar> neg{foobar()};
            return KT::assert_true (c, "neg(true,  0, 0)", neg(true,  0, 0))
                && KT::assert_true (c, "neg(false, 0, 0)", neg(false, 0, 0))
                && KT::assert_false(c, "neg(true,  1, 0)", neg(true,  1, 0))
                && KT::assert_true (c, "neg(false, 1, 0)", neg(false, 1, 0))
                && KT::assert_true (c, "neg(true,  0, 1)", neg(true,  0, 1))
                && KT::assert_false(c, "neg(false, 0, 1)", neg(false, 0, 1))
                && KT::assert_false(c, "neg(true,  1, 1)", neg(true , 1, 1))
                && KT::assert_false(c, "neg(false, 1, 1)", neg(false, 1, 1))
                && KT::assert_type<bool, decltype(neg(true, 0, 0))>(c, "neg(0, 0, 0) yields bool")
                ;
        }),
    KT::expect_success("negate input", [](KT::context& c)->bool{
            return KT::assert_false(c, "true input", NF::not_(foo(false))(true))
                && KT::assert_true(c, "false input", NF::not_(foo(false))(false))
                && KT::assert_type<bool, decltype(NF::not_(foo(false))(true))>(c, "result type is bool")
                ;
        }),
    KT::expect_success("negate input (using data)", [](KT::context& c)->bool{
            return KT::assert_true(c, "true input", NF::not_(foo(true))(true))
                && KT::assert_false(c, "false input", NF::not_(foo(true))(false))
                && KT::assert_type<bool, decltype(NF::not_(foo(true))(true))>(c, "result type is bool")
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("functional::not_", ac, av, ::tests);
}
