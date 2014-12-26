// nstd/functional/reference_wrapper.t.cpp                            -*-C++-*-
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

#include "nstd/functional/reference_wrapper.hpp"
#include "kuhl/test.hpp"

namespace NF = nstd::functional;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct foo {};
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("typedefs", [](KT::context& c)->bool{
            return KT::assert_type<foo, NF::reference_wrapper<foo>::type>(c, "type")
                ;
        }),
    KT::expect_success("ctors", [](KT::context& c)->bool{
            foo object;
            NF::reference_wrapper<foo> const ref0(object);
            NF::reference_wrapper<foo> const ref1(ref0);
            return KT::assert_true(c, "noexcept", noexcept(NF::reference_wrapper<foo>(object)))
                && KT::assert_equal(c, "init object (conversion)", &object, &static_cast<foo&>(ref0))
                && KT::assert_equal(c, "init object", &object, &ref0.get())
                && KT::assert_true(c, "noexcept", noexcept(NF::reference_wrapper<foo>(ref0)))
                && KT::assert_equal(c, "copy object (conversion)", &object, &static_cast<foo&>(ref1))
                && KT::assert_equal(c, "copy object", &object, &ref1.get())
                ;
        }),
    KT::expect_success("assignment", [](KT::context& c)->bool{
            foo object0;
            foo object1;
            NF::reference_wrapper<foo> const ref0(object0);
            NF::reference_wrapper<foo>       ref1(object1);
            ref1 = ref0;
            return KT::assert_type<NF::reference_wrapper<foo>&, decltype(ref1 = ref0)>(c, "assignment type")
                && KT::assert_true(c, "noexept", noexcept(ref1 = ref0))
                && KT::assert_equal(c, "copy object (conversion)", &object0, &static_cast<foo&>(ref1))
                && KT::assert_equal(c, "copy object", &object0, &ref1.get())
                ;
        }),
    KT::expect_success("accessor", [](KT::context& c)->bool{
            foo object0;
            NF::reference_wrapper<foo> const ref0(object0);
            return KT::assert_type<foo&, decltype(ref0.get())>(c, "get() type")
                && KT::assert_equal(c, "object (conversion)", &object0, &static_cast<foo&>(ref0))
                && KT::assert_equal(c, "object", &object0, &ref0.get())
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("functional::reference_wrapper", ac, av, ::tests);
}
