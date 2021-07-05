// nstd/functional/tag_invoke.t.cpp                                   -*-C++-*-
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

#include "nstd/functional/tag_invoke.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {};
namespace TD = test_declarations;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    struct non_overloaded_tag_t {};

    template <bool Nothrow> struct overloaded_tag_t {};
    template <bool Nothrow> constexpr overloaded_tag_t<Nothrow> overloaded_tag;

    template <bool Nothrow>
    auto tag_invoke(overloaded_tag_t<Nothrow>, int v) noexcept(Nothrow) -> int{
        return v;
    }

    template <bool Nothrow>
    auto tag_invoke(overloaded_tag_t<Nothrow>, int& v, bool) noexcept(Nothrow) -> int&{
        return v;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("non-overloaded tag_invoke isn't tag_invocable", []{
        return !::nstd::tag_invocable<TD::non_overloaded_tag_t>
            && !::nstd::tag_invocable<TD::non_overloaded_tag_t, int>
            ;
    }),
    KT::expect_success("overloaded tag_invoke is tag_invocable", []{
        return !::nstd::tag_invocable<TD::overloaded_tag_t<false>>
            && ::nstd::tag_invocable<TD::overloaded_tag_t<false>, int>
            && !::nstd::nothrow_tag_invocable<TD::overloaded_tag_t<false>, int>
            && 17 == ::nstd::tag_invoke(TD::overloaded_tag<false>, 17)
            ;
    }),
    KT::expect_success("tag_invoke reference result", []{
        int value(17);
        return ::nstd::tag_invocable<TD::overloaded_tag_t<false>, int&, bool>
            && !::nstd::nothrow_tag_invocable<TD::overloaded_tag_t<false>, int&, bool>
            && &value == &::nstd::tag_invoke(TD::overloaded_tag<false>, value, true)
            ;
    }),
    KT::expect_success("noexcept overloaded tag_invoke is tag_invocable", []{
        return !::nstd::tag_invocable<TD::overloaded_tag_t<true>>
            && ::nstd::tag_invocable<TD::overloaded_tag_t<true>, int>
            && ::nstd::nothrow_tag_invocable<TD::overloaded_tag_t<true>, int>
            && 17 == ::nstd::tag_invoke(TD::overloaded_tag<true>, 17)
            ;
    }),
    KT::expect_success("noexcept tag_invoke reference result", []{
        int value(17);
        return ::nstd::tag_invocable<TD::overloaded_tag_t<true>, int&, bool>
            && ::nstd::nothrow_tag_invocable<TD::overloaded_tag_t<true>, int&, bool>
            && &value == &::nstd::tag_invoke(TD::overloaded_tag<true>, value, true)
            ;
    }),
};

static KT::add_tests suite("tag_invoke", ::tests);
