// nstd/utility/forward.t.cpp                                         -*-C++-*-
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

#include "nstd/utility/forward.hpp"
#include "nstd/type_traits/declval.hpp"
#include "kuhl/test.hpp"

namespace NU = nstd::utility;
namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct foo {};
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("forward(rvalue)", [](KT::context& c)->bool{
            foo&& reference(NU::forward<foo&&>(foo()));
            KT::use(reference);
            return KT::assert_type<foo&&, decltype(NU::forward<foo&&>(foo()))>(c, "foo()");
                ;
        }),
    KT::expect_success("forward(lvalue)", [](KT::context& c)->bool{
            foo  lvalue{};
            foo& reference(NU::forward<foo&>(lvalue));
            return assert_equal(c, "forward lvalue", &lvalue, &reference)
                && KT::assert_type<foo&, decltype(NU::forward<foo&>(lvalue))>(c, "foo&");
                ;
        }),
    KT::expect_success("forward(lvalue const)", [](KT::context& c)->bool{
            foo const  lvalue{};
            foo const& reference(NU::forward<foo const&>(lvalue));
            return assert_equal(c, "forward lvalue const", &lvalue, &reference)
                && KT::assert_type<foo const&, decltype(NU::forward<foo const&>(lvalue))>(c, "foo const&");
                ;
        }),
    KT::expect_success("forward(lvalue volatile)", [](KT::context& c)->bool{
            foo volatile  lvalue{};
            foo volatile& reference(NU::forward<foo volatile&>(lvalue));
            return assert_equal(c, "forward lvalue volatile", &lvalue, &reference)
                && KT::assert_type<foo volatile&, decltype(NU::forward<foo volatile&>(lvalue))>(c, "foo volatile&");
                ;
        }),
    KT::expect_success("forward(lvalue const volatile)", [](KT::context& c)->bool{
            foo const volatile  lvalue{};
            foo const volatile& reference(NU::forward<foo const volatile&>(lvalue));
            return assert_equal(c, "forward lvalue const volatile", &lvalue, &reference)
                && KT::assert_type<foo const volatile&, decltype(NU::forward<foo const volatile&>(lvalue))>(c, "foo const volatile&");
                ;
        }),
    KT::expect_success("forward() doesn't throw", [](KT::context& c)->bool{
            return assert_true(c, "rvlaue", noexcept(NU::forward<foo&&>(NT::declval<foo>())))
                && assert_true(c, "lvlaue", noexcept(NU::forward<foo&>(NT::declval<foo&>())))
                && assert_true(c, "lvlaue const", noexcept(NU::forward<foo const&>(NT::declval<foo const&>())))
                && assert_true(c, "lvlaue volatile", noexcept(NU::forward<foo volatile&>(NT::declval<foo volatile&>())))
                && assert_true(c, "lvlaue const volatile", noexcept(NU::forward<foo const volatile&>(NT::declval<foo const volatile&>())))
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("utility::forward", ac, av, ::tests);
}
