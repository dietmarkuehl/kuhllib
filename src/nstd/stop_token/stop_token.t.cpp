// nstd/stop_token/stop_token.t.cpp                                   -*-C++-*-
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

#include "nstd/stop_token/stop_token.hpp"
#include "nstd/stop_token/stop_source.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"

namespace test_declaration {}
namespace ST = ::nstd::stop_token_ns;
namespace UT = ::nstd::utility;
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("default ctor", []{
            ST::stop_source source;
            ST::stop_token  token;
            return noexcept(ST::stop_token())
                && token.stop_possible() == false
                && token.stop_requested() == false
                && token != source.get_token()
                ;
        }),
    KT::expect_success("copy ctor", []{
            ST::stop_source      source;
            ST::stop_token const t0;
            ST::stop_token const t1 = source.get_token();

            return noexcept(ST::stop_token(t1))
                && t0 == ST::stop_token(t0)
                && t1 == ST::stop_token(t1)
                ;
        }),
    KT::expect_success("move ctor", []{
            ST::stop_source source;
            ST::stop_token  t0;
            ST::stop_token  c0(t0);
            ST::stop_token  t1 = source.get_token();
            ST::stop_token  c1(t1);

            return noexcept(ST::stop_token(UT::move(t1)))
                && c0 == ST::stop_token(UT::move(t0))
                && c1 == ST::stop_token(UT::move(t1))
                ;
        }),
    KT::expect_success("copy assign", []{
            ST::stop_source      source;
            ST::stop_token const t0;
            ST::stop_token       a0;
            ST::stop_token const t1 = source.get_token();
            ST::stop_token       a1;

            return noexcept(a0 = t0)
                && &a0 == &(a0 = t0)
                && a0 == t0
                && &a1 == &(a1 = t1)
                && a1 == t1
                ;
        }),
    KT::expect_success("move assign", []{
            ST::stop_source      source;
            ST::stop_token const t0;
            ST::stop_token       c0(t0);
            ST::stop_token       a0;
            ST::stop_token const t1 = source.get_token();
            ST::stop_token       c1(t1);
            ST::stop_token       a1;

            return noexcept(a0 = UT::move(c0))
                && &a0 == &(a0 = UT::move(c0))
                && a0 == t0
                && &a1 == &(a1 = UT::move(c1))
                && a1 == t1
                ;
        }),
    KT::expect_success("member swap", []{
            ST::stop_source      source;
            ST::stop_token const t0;
            ST::stop_token       s0(t0);
            ST::stop_token const t1 = source.get_token();
            ST::stop_token       s1(t1);

            s0.swap(s1);

            return noexcept(s0.swap(s1))
                && KT::type<void> == KT::type<decltype(s0.swap(s1))>
                && s0 == t1
                && s1 == t0
                ;
        }),
    KT::expect_success("non-member swap", []{
            ST::stop_source      source;
            ST::stop_token const t0;
            ST::stop_token       s0(t0);
            ST::stop_token const t1 = source.get_token();
            ST::stop_token       s1(t1);

            swap(s0, s1);

            return noexcept(swap(s0, s1))
                && KT::type<void> == KT::type<decltype(swap(s0, s1))>
                && s0 == t1
                && s1 == t0
                ;
        }),
    KT::expect_success("stop_requested", []{
            ST::stop_source      s1;
            ST::stop_source      s2;
            ST::stop_token const t0;
            ST::stop_token const t1 = s1.get_token();
            ST::stop_token const t2 = s2.get_token();

            s2.request_stop();

            return noexcept(t0.stop_requested())
                && KT::type<bool> == KT::type<decltype(t0.stop_requested())>
                && not t0.stop_requested()
                && not t1.stop_requested()
                && t2.stop_requested()
                ;
        }),
    KT::expect_success("stop_possible", []{
            ST::stop_source      source;
            ST::stop_token const t0;
            ST::stop_token const t1 = source.get_token();

            return noexcept(t0.stop_possible())
                && KT::type<bool> == KT::type<decltype(t0.stop_possible())>
                && not t0.stop_possible()
                && t1.stop_possible()
                ;
        }),
    KT::expect_success("operator==", []{
            ST::stop_source      source;
            ST::stop_token const t0;
            ST::stop_token const c0(t0);
            ST::stop_token const t1 = source.get_token();
            ST::stop_token const c1(t1);

            return noexcept(t0 == t1)
                && noexcept(t0 != t1)
                && KT::type<bool> == KT::type<decltype(t0 == t1)>
                && KT::type<bool> == KT::type<decltype(t0 != t1)>
                && c0 == t0
                && c1 == t1
                && t0 != t1
                ;
        }),
};

static KT::add_tests suite("nstd::stop_token", ::tests);
