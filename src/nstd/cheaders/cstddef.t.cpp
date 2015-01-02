// nstd/cheaders/cstddef.t.cpp                                        -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2015 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/cheaders/cstddef.hpp"
#include "kuhl/test.hpp"

namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct foo {
        int value0;
        int value1;
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("NULL", [](KT::context& c)->bool{
            foo* ptr = NULL;
            return KT::assert_true(c, "NULL", ptr == NULL)
                && KT::assert_true(c, "NULL", ptr == nullptr)
                ;
        }),
#if 0
    //-dk:TODO offsetof() vs. EDG
    KT::expect_success("offsetof()", [](KT::context& c)->bool{
            return KT::assert_equal(c, "offsetof(foo, value0)", offsetof(foo, value0), 0u)
                && KT::assert_equal(c, "offsetof(foo, value1)", offsetof(foo, value1), sizeof(int))
                ;
        }),
#endif
    KT::expect_success("ptrdiff_t", [](KT::context& c)->bool{
            foo a[1];
            return KT::assert_type< ::nstd::ptrdiff_t, ::ptrdiff_t>(c, "nstd::ptrdiff_t vs. ptrdiff_t")
                && KT::assert_type< ::nstd::ptrdiff_t, decltype((a + 1) - a)>(c, "nstd::ptrdiff_t vs. a - a")
                ;
        }),
    KT::expect_success("size_t", [](KT::context& c)->bool{
            return KT::assert_type< ::nstd::size_t, ::size_t>(c, "nstd::size_t vs. size_t")
                && KT::assert_type< ::nstd::size_t, decltype(sizeof(0))>(c, "nstd::size_t vs. sizeof(0)")
                ;
        }),
    KT::expect_success("max_align_t", [](KT::context& c)->bool{
            ::nstd::max_align_t value{};
            KT::use(value);
            return true
                ;
        }),
    KT::expect_success("nullptr_t", [](KT::context& c)->bool{
            return KT::assert_type< ::nstd::nullptr_t, decltype(nullptr)>(c, "nstd::nullptr_t vs. decltype(nullptr)")
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("cheaders/cstddef", ac, av, ::tests);
}
