// nstd/stop_token.t.cpp                                              -*-C++-*-
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

#include "nstd/stop_token.hpp"
#include "kuhl/test.hpp"

namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        struct callback {
            auto operator()() {}
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("stop_source", []{
            return KT::assert_type_exists<nstd::stop_source>;
        }),
    KT::expect_success("stop_token", []{
            return KT::assert_type_exists<nstd::stop_token>;
        }),
    KT::expect_success("nostopstate_t", []{
            return KT::assert_type_exists<nstd::nostopstate_t>;
        }),
    KT::expect_success("nostopstate", []{
            return KT::type<nstd::nostopstate_t const> == KT::type<decltype(nstd::nostopstate)>;
        }),
    KT::expect_success("stop_callback", []{
            return KT::assert_template_exists<nstd::stop_callback, TD::callback>;
        }),
};

static KT::add_tests suite("nstd/stop_token", ::tests);
