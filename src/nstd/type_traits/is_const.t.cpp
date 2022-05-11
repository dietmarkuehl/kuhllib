// nstd/type_traits/is_const.t.cpp                                    -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#include "nstd/type_traits/is_const.hpp"
#include "kuhl/test.hpp"

namespace TT = ::nstd::type_traits;
namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        struct type;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("basic types", []{
            return not TT::is_const<TD::type>::value
                && not TT::is_const_v<TD::type>
                && TT::is_const<TD::type const>::value
                && TT::is_const_v<TD::type const>
                && not TT::is_const<TD::type volatile>::value
                && not TT::is_const_v<TD::type volatile>
                && TT::is_const<TD::type const volatile>::value
                && TT::is_const_v<TD::type const volatile>
                ;
        }),
    KT::expect_success("reference types", []{
            return not TT::is_const<TD::type&>::value
                && not TT::is_const_v<TD::type&>
                && not TT::is_const<TD::type const&>::value
                && not TT::is_const_v<TD::type const&>
                && not TT::is_const<TD::type volatile&>::value
                && not TT::is_const_v<TD::type volatile&>
                && not TT::is_const<TD::type const volatile&>::value
                && not TT::is_const_v<TD::type const volatile&>
                ;
        }),
    KT::expect_success("array types", []{
            return not TT::is_const<TD::type[10]>::value
                && not TT::is_const_v<TD::type[10]>
                && TT::is_const<TD::type const[10]>::value
                && TT::is_const_v<TD::type const[10]>
                && not TT::is_const<TD::type volatile[10]>::value
                && not TT::is_const_v<TD::type volatile[10]>
                && TT::is_const<TD::type const volatile[10]>::value
                && TT::is_const_v<TD::type const volatile[10]>
                ;
        }),
};

static KT::add_tests suite("[meta.unary.prop]::is_const", ::tests);
