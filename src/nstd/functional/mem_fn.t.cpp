// nstd/functional/mem_fn.t.cpp                                       -*-C++-*-
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

#include "nstd/functional/mem_fn.hpp"
#include "kuhl/test.hpp"
#include <tuple>

namespace NF = nstd::functional;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct foo {
        int mem;
        foo(int mem): mem(mem) {}
        std::tuple<int> mem0() { return std::make_tuple(this->mem); }
        std::tuple<int> omem0() { return std::make_tuple(this->mem); }
        std::tuple<int, int> mem1(int v0) { return std::make_tuple(this->mem, v0); }
        std::tuple<int, int> omem1(int v0) { return std::make_tuple(this->mem, v0); }
        std::tuple<int, int, int> mem2(int v0, int v1) { return std::make_tuple(this->mem, v0, v1); }
        std::tuple<int, int, int> omem2(int v0, int v1) { return std::make_tuple(this->mem, v0, v1); }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("mem_fn(mem0)", [](KT::context& c)->bool{
            foo f(18);
            return KT::assert_true(c, "call()", std::make_tuple(18) == NF::mem_fn(&foo::mem0)(f))
                && KT::assert_true(c, "equal", NF::mem_fn(&foo::mem0) == NF::mem_fn(&foo::mem0))
                && KT::assert_false(c, "not equal", NF::mem_fn(&foo::mem0) == NF::mem_fn(&foo::omem0))
                ;
        }),
    KT::expect_success("mem_fn(mem1)", [](KT::context& c)->bool{
            foo f(18);
            return KT::assert_true(c, "call()", std::make_tuple(18, 43) == NF::mem_fn(&foo::mem1)(f, 43))
                && KT::assert_true(c, "equal", NF::mem_fn(&foo::mem1) == NF::mem_fn(&foo::mem1))
                && KT::assert_false(c, "not equal", NF::mem_fn(&foo::mem1) == NF::mem_fn(&foo::omem1))
                ;
        }),
    KT::expect_success("mem_fn(mem2)", [](KT::context& c)->bool{
            foo f(18);
            return KT::assert_true(c, "call()", std::make_tuple(18, 43, 11) == NF::mem_fn(&foo::mem2)(f, 43, 11))
                && KT::assert_true(c, "equal", NF::mem_fn(&foo::mem2) == NF::mem_fn(&foo::mem2))
                && KT::assert_false(c, "not equal", NF::mem_fn(&foo::mem2) == NF::mem_fn(&foo::omem2))
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("functional::mem_fn", ac, av, ::tests);
}
