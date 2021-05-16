// nstd/utility/equality_comparable.t.cpp                             -*-C++-*-
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

#include "nstd/utility/equality_comparable.hpp"
#include "kuhl/test.hpp"

namespace NU = nstd::utility;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    template <typename>
    struct foo {
        int value;
        explicit foo(int value): value(value) {}
    };

    template <typename T>
    int compare(foo<T> const& v0, foo<T> const& v1) {
        return v0.value - v1.value;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("foo<equality_comparable> can be compared with ==", [](KT::context& c)->bool{
            using foo_eq = foo<NU::equality_comparable>;
            return KT::assert_false(c, "less",    foo_eq(10) == foo_eq(17))
                && KT::assert_true (c, "equal",   foo_eq(10) == foo_eq(10))
                && KT::assert_false(c, "greater", foo_eq(13) == foo_eq(10))
                ;
        }),
    KT::expect_success("foo<equality_comparable> can be compared with !=", [](KT::context& c)->bool{
            using foo_eq = foo<NU::equality_comparable>;
            return KT::assert_true (c, "less",    foo_eq(10) != foo_eq(17))
                && KT::assert_false(c, "equal",   foo_eq(10) != foo_eq(10))
                && KT::assert_true (c, "greater", foo_eq(13) != foo_eq(10))
                ;
        }),
};

static KT::add_tests suite("utility::equality_comparable", ::tests);
