// nstd/type_traits/is_void.t.cpp                                     -*-C++-*-
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

#include "nstd/type_traits/is_void.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    class foo;
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("foo", [](KT::context& c)->bool{
            constexpr bool value = NT::is_void<foo>::value;
            KT::use(value);
            return KT::assert_false(c, "foo", NT::is_void<foo>::value)
                ;
        }),
    KT::expect_success("void", [](KT::context& c)->bool{
            constexpr bool value = NT::is_void<void>::value;
            KT::use(value);
            return KT::assert_true(c, "void", NT::is_void<void>::value)
                ;
        }),
    KT::expect_success("void cv", [](KT::context& c)->bool{
            return KT::assert_false(c, "void const", NT::is_void<void const>::value)
                && KT::assert_false(c, "void volatile", NT::is_void<void volatile>::value)
                && KT::assert_false(c, "void const volatile", NT::is_void<void const volatile>::value)
                ;
        }),
};

static KT::add_tests suite("type_traits::is_void", ::tests);
