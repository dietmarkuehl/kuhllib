// nstd/stop_token/nostopstate.t.cpp                                  -*-C++-*-
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

#include "nstd/stop_token/nostopstate.hpp"
#include "kuhl/test.hpp"
#include <type_traits>

namespace ST = ::nstd::stop_token_ns;
namespace TT = ::std;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("nostopstate_t ctors", []{
            return TT::is_default_constructible_v<ST::nostopstate_t>
                && TT::is_copy_constructible_v<ST::nostopstate_t>
                && TT::is_move_constructible_v<ST::nostopstate_t>
                ;
        }),
    KT::expect_success("nostopstate", []{
            return KT::type<ST::nostopstate_t const> == KT::type<decltype(ST::nostopstate)>
                ;
        }),
};

static KT::add_tests suite("nstd/stop_token/nostopstate", ::tests);
