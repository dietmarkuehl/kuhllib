// nstd/utility/as_const.t.cpp                                        -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/utility/as_const.hpp"
#include "nstd/type_traits/declval.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace KT = ::kuhl::test;
namespace UT = ::nstd::utility;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    template <typename T>
    concept has_as_const
        = requires() { UT::as_const(::nstd::type_traits::declval<T>()); }
        ;

    struct type {};
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("as_const with non-const object", []{
            TD::type value{};
            return TD::has_as_const<TD::type&>
                && KT::type<TD::type const&> == KT::type<decltype(UT::as_const(value))>
                && noexcept(UT::as_const(value))
                && &value == &UT::as_const(value)
                ;
        }),
    KT::expect_success("as_const with const object", []{
            TD::type const value{};
            return TD::has_as_const<TD::type const&>
                && KT::type<TD::type const&> == KT::type<decltype(UT::as_const(value))>
                && noexcept(UT::as_const(value))
                && &value == &UT::as_const(value)
                ;
        }),
    KT::expect_success("as_const with volatile object", []{
            TD::type volatile value{};
            return TD::has_as_const<TD::type volatile&>
                && KT::type<TD::type const volatile&> == KT::type<decltype(UT::as_const(value))>
                && noexcept(UT::as_const(value))
                && &value == &UT::as_const(value)
                ;
        }),
    KT::expect_success("as_const with const volatile object", []{
            TD::type const volatile value{};
            return TD::has_as_const<TD::type const volatile&>
                && KT::type<TD::type const volatile&> == KT::type<decltype(UT::as_const(value))>
                && noexcept(UT::as_const(value))
                && &value == &UT::as_const(value)
                ;
        }),
    KT::expect_success("can't use as_const with temporaries", []{
            return !TD::has_as_const<TD::type>
                ;
        })
};

static KT::add_tests suite("as_const", ::tests);
