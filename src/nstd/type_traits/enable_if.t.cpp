// nstd/utility/enable_if.t.cpp                                       -*-C++-*-
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

#include "nstd/type_traits/enable_if.hpp"
#include "nstd/type_traits/is_same.hpp"
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

namespace
{
    template <typename T>
    bool test(T*, typename NT::enable_if<!NT::is_same<foo, T>::value>::type* = 0) {
        return true;
    }
    char test(void*) {
        return 'n';
    }

    template <typename T, typename = NT::enable_if_t<!NT::is_same<foo, T>::value>>
    bool test_t(T*) {
        return true;
    }
    char test_t(void*) {
        return 'n';
    }
}

// ----------------------------------------------------------------------------

static kuhl::test::testcase const tests[] = {
    kuhl::test::expect_success("default type when true", []()->bool{
            return NT::is_same<void, typename NT::enable_if<true>::type>::value
                && NT::is_same<void, NT::enable_if_t<true>>::value;
        }),
    kuhl::test::expect_success("custom type when true", []()->bool{
            return NT::is_same<foo, typename NT::enable_if<true, foo>::type>::value
                && NT::is_same<foo, NT::enable_if_t<true, foo>>::value;
        }),
    kuhl::test::expect_success("no type when false (enable_if", []()->bool{
            return NT::is_same<char, decltype(test(static_cast<foo*>(0)))>::value
                && char(test(static_cast<foo*>(0)) == 'n')
                && NT::is_same<bool, decltype(test(static_cast<bar*>(0)))>::value
                && test(static_cast<bar*>(0));
        }),
    kuhl::test::expect_success("no type when false (enable_if_t)", []()->bool{
            return NT::is_same<char, decltype(test_t(static_cast<foo*>(0)))>::value
                && char(test_t(static_cast<foo*>(0)) == 'n')
                && NT::is_same<bool, decltype(test_t(static_cast<bar*>(0)))>::value
                && test_t(static_cast<bar*>(0));
        }),
};

static KT::add_tests suite("type_traits::enable_if", ::tests);
