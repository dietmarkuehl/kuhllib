// nstd/type_traits/condition.t.cpp                                   -*-C++-*-
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

#include "nstd/type_traits/condition.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct foo;
    struct bar;
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("condition<true, foo, bar>", [](KT::context& c)->bool{
            return KT::assert_type<foo, NT::condition<true, foo, bar>::type>(c, "expect foo")
                && KT::assert_type<foo, NT::condition_t<true, foo, bar>>(c, "expect foo")
                ;
        }),
    KT::expect_success("condition<false, foo, bar>", [](KT::context& c)->bool{
            return KT::assert_type<bar, NT::condition<false, foo, bar>::type>(c, "expect bar")
                && KT::assert_type<bar, NT::condition_t<false, foo, bar>>(c, "expect bar")
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("type_traits::condition", ac, av, ::tests);
}
