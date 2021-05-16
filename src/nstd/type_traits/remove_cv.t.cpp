// nstd/type_traits/remove_cv.t.cpp                                   -*-C++-*-
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

#include "nstd/type_traits/remove_cv.hpp"
#include "nstd/type_traits/is_same.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct foo;
}

// ----------------------------------------------------------------------------

static kuhl::test::testcase const tests[] = {
    kuhl::test::expect_success("non-volatile, non-volatile stays the same", []()->bool{
            return NT::is_same<foo, typename NT::remove_cv<foo>::type>::value
                && NT::is_same<foo, NT::remove_cv_t<foo>>::value
                ;
        }),
    kuhl::test::expect_success("const is removed", []()->bool{
            return NT::is_same<foo, typename NT::remove_cv<foo const>::type>::value
                && NT::is_same<foo, NT::remove_cv_t<foo const>>::value
                ;
        }),
    kuhl::test::expect_success("volatile is removed", []()->bool{
            return NT::is_same<foo, typename NT::remove_cv<foo volatile>::type>::value
                && NT::is_same<foo, NT::remove_cv_t<foo volatile>>::value
                ;
        }),
    kuhl::test::expect_success("const volatile are removed", []()->bool{
            return NT::is_same<foo, typename NT::remove_cv<foo const volatile>::type>::value
                && NT::is_same<foo, NT::remove_cv_t<foo const volatile>>::value
                ;
        }),
    kuhl::test::expect_success("inner const is not removed", []()->bool{
            return NT::is_same<foo const*, typename NT::remove_cv<foo const*>::type>::value
                && NT::is_same<foo const*, NT::remove_cv_t<foo const*>>::value
                ;
        }),
    kuhl::test::expect_success("inner volatile is not removed", []()->bool{
            return NT::is_same<foo volatile*, typename NT::remove_cv<foo volatile*>::type>::value
                && NT::is_same<foo volatile*, NT::remove_cv_t<foo volatile*>>::value
                ;
        }),
    kuhl::test::expect_success("inner const volatile is not removed", []()->bool{
            return NT::is_same<foo const volatile*, typename NT::remove_cv<foo const volatile*>::type>::value
                && NT::is_same<foo const volatile*, NT::remove_cv_t<foo const volatile*>>::value
                ;
        }),
    kuhl::test::expect_success("outer const volatile is, inner const volatile is not removed", []()->bool{
            return NT::is_same<foo const volatile*, typename NT::remove_cv<foo const volatile* const volatile>::type>::value
                && NT::is_same<foo const volatile*, NT::remove_cv_t<foo const volatile* const volatile>>::value
                ;
        }),
};

static KT::add_tests suite("type_traits::remove_cv", ::tests);
