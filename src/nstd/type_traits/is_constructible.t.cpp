// nstd/type_traits/is_constructible.t.cpp                            -*-C++-*-
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

#include "nstd/type_traits/is_constructible.hpp"
#include "kuhl/test.hpp"

namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct movable {
        movable(movable&&) = default;
        movable(movable const&) = delete;
    };
    struct copyable {
        copyable(copyable&&) = delete;
        copyable(copyable const&) = default;
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("int", [](KT::context& c)->bool{
            return KT::assert_true(c, "int()", NT::is_constructible<int>::value)
                && KT::assert_true(c, "int(int)", NT::is_constructible<int, int>::value)
                && KT::assert_true(c, "int(int&)", NT::is_constructible<int, int&>::value)
                && KT::assert_true(c, "int(int&&)", NT::is_constructible<int, int&&>::value)
                && KT::assert_false(c, "int(nullptr_t)", NT::is_constructible<int, decltype(nullptr)>::value)
                && KT::assert_false(c, "int(int, int)", NT::is_constructible<int, int, int>::value)
                ;
        }),
    KT::expect_success("movable", [](KT::context& c)->bool{
            return KT::assert_false(c, "movable()", NT::is_constructible<movable>::value)
                && KT::assert_true(c, "movable(movable)", NT::is_constructible<movable, movable>::value)
                && KT::assert_false(c, "movable(movable const&)", NT::is_constructible<movable, movable const&>::value)
                && KT::assert_false(c, "movable(movable&)", NT::is_constructible<movable, movable&>::value)
                && KT::assert_true(c, "movable(movable&&)", NT::is_constructible<movable, movable&&>::value)
                && KT::assert_false(c, "movable(nullptr_t)", NT::is_constructible<int, decltype(nullptr)>::value)
                && KT::assert_false(c, "movable(movable, movable)", NT::is_constructible<movable, movable, movable>::value)
                ;
        }),
    KT::expect_success("copyable", [](KT::context& c)->bool{
            return KT::assert_false(c, "copyable()", NT::is_constructible<copyable>::value)
                && KT::assert_false(c, "copyable(copyable)", NT::is_constructible<copyable, copyable>::value)
                && KT::assert_true(c, "copyable(copyable const&)", NT::is_constructible<copyable, copyable const&>::value)
                && KT::assert_true(c, "copyable(copyable&)", NT::is_constructible<copyable, copyable&>::value)
                && KT::assert_false(c, "copyable(copyable&&)", NT::is_constructible<copyable, copyable&&>::value)
                && KT::assert_false(c, "copyable(nullptr_t)", NT::is_constructible<int, decltype(nullptr)>::value)
                && KT::assert_false(c, "copyable(copyable, copyable)", NT::is_constructible<copyable, copyable, copyable>::value)
                ;
        }),
};

static KT::add_tests suite("type_traits::is_constructible", ::tests);
