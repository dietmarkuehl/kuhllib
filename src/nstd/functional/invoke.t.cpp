// nstd/functional/invoke.t.cpp                                       -*-C++-*-
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

#include "nstd/functional/invoke.hpp"
#include "kuhl/test.hpp"

namespace NF = nstd::functional;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    int fun(char, double) { return 41; }
    struct function {
        int operator()(char, double) { return 42; }
    };
    struct type {
        int member(char, double) { return 43; }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("invoke(fun)", [](KT::context& c)->bool{
            return KT::assert_type<int, decltype(NF::invoke(fun, 'c', 3.14))>(c, "type")
                && KT::assert_equal(c, "call", 41, NF::invoke(fun, 'c', 3.14))
               ;
        }),
    KT::expect_success("invoke(function())", [](KT::context& c)->bool{
            return KT::assert_type<int, decltype(NF::invoke(function(), 'c', 3.14))>(c, "type")
                && KT::assert_equal(c, "call", 42, NF::invoke(function(), 'c', 3.14))
               ;
        }),
#if 0
    KT::expect_success("invoke(&type::member)", [](KT::context& c)->bool{
            type object;
            return KT::assert_type<int, decltype(NF::invoke(&type::member, object, 'c', 3.14))>(c, "type")
                && KT::assert_equal(c, "call", 42, NF::invoke(&type::member, object, 'c', 3.14))
               ;
        }),
#endif
};

int main(int ac, char* av[])
{
    return KT::run_tests("functional::invoke", ac, av, ::tests);
}
