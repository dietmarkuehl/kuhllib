// nstd/stop_token/stop_source.t.cpp                                  -*-C++-*-
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

#include "nstd/stop_token/stop_source.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"

namespace ST = ::nstd::stop_token_ns;
namespace UT = ::nstd::utility;
namespace test_declaration {}
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
            return source.stop_possible() == true
                && source.stop_requested() == false
                ;
        }),
    KT::expect_success("no stop state ctor", []{
            ST::stop_source source(ST::nostopstate);
            return noexcept(ST::stop_source(ST::nostopstate))
                && source.stop_possible() == false
                && source.stop_requested() == false
                ;
        }),
    KT::expect_success("copy ctor", []{
            ST::stop_source s0(ST::nostopstate);
            ST::stop_source s1;
            return noexcept(ST::stop_source(s0))
                && s0 == ST::stop_source(s0)
                && ST::stop_source(ST::nostopstate) == ST::stop_source(s0) 
                && s1 == ST::stop_source(s1)
                && ST::stop_source(ST::nostopstate) != ST::stop_source(s1) 
                ;
        }),
    KT::expect_success("move ctor", []{
            ST::stop_source s0(ST::nostopstate);
            ST::stop_source c0(s0);
            ST::stop_source s1;
            ST::stop_source c1(s1);
            return noexcept(ST::stop_source(ST::stop_source(ST::nostopstate)))
                && c0 == ST::stop_source(UT::move(s0))
                && c1 == ST::stop_source(UT::move(s1))
                ;
        }),
    KT::expect_success("copy assign", []{
            ST::stop_source const s0(ST::nostopstate);
            ST::stop_source const s1;
            ST::stop_source       c0;
            ST::stop_source       c1;
            ST::stop_source       t0(c0);
            ST::stop_source       t1(c0);

            c0 = s0;
            c1 = s1;
            return KT::type<ST::stop_source&> == KT::type<decltype(c0 = s0)>
                && noexcept(c0 = s0)
                && c0 == s0
                && c0 != t0
                && c1 == s1
                && c1 != t1
                && &(c0 = s0) == &c0
                ;
        }),
    KT::expect_success("move assign", []{
            ST::stop_source s0(ST::nostopstate);
            ST::stop_source s1;
            ST::stop_source a0(s0);
            ST::stop_source a1(s1);
            ST::stop_source c0;
            ST::stop_source c1;
            ST::stop_source t0(c0);
            ST::stop_source t1(c1);

            c0 = UT::move(s0);
            c1 = UT::move(s1);
            return KT::type<ST::stop_source&> == KT::type<decltype(s0 = UT::move(s1))>
                && noexcept(s0 = UT::move(s1))
                && c0 == a0
                && c0 != t0
                && c1 == a1
                && c1 != t1
                && &(c0 = UT::move(s0)) == &c0
                ;
        }),
    KT::expect_success("member swap", []{
            ST::stop_source s0;
            ST::stop_source s1;
            ST::stop_source c0(s0);
            ST::stop_source c1(s1);
            s0.swap(s1);

            return KT::type<void> == KT::type<decltype(s0.swap(s1))>
                && noexcept(s0.swap(s1))
                && s0 == c1
                && s1 == c0
                && s0 != s1
                ;
        }),
    KT::expect_success("non-member swap", []{
            ST::stop_source s0;
            ST::stop_source s1;
            ST::stop_source c0(s0);
            ST::stop_source c1(s1);
            swap(s0, s1);

            return KT::type<void> == KT::type<decltype(swap(s0, s1))>
                && noexcept(swap(s0, s1))
                && s0 == c1
                && s1 == c0
                && s0 != s1
                ;
        }),
    KT::expect_success("operator==", []{
            ST::stop_source n0(ST::nostopstate);
            ST::stop_source n1(ST::nostopstate);
            ST::stop_source o;

            return noexcept(n0 == n1)
                && KT::type<bool> == KT::type<decltype(n0 == n1)>
                && KT::type<bool> == KT::type<decltype(n0 != n1)>
                && n0 == n1
                && n0 != o
                && o  == o
                ;
        }),
    KT::expect_success("get_token", []{
            ST::stop_source const s;

            (void)s.get_token();
            return KT::type<ST::stop_token> == KT::type<decltype(s.get_token())>
                && noexcept(s.get_token())
                ;
        }),
    KT::expect_success("stop_possible", []{
            ST::stop_source const s0(ST::nostopstate);
            ST::stop_source const s1;

            return KT::type<bool> == KT::type<decltype(s0.stop_possible())>
                && noexcept(s0.stop_possible())
                && not s0.stop_possible()
                && s1.stop_possible()
                ;
        }),
    KT::expect_success("stop_requested", []{
            ST::stop_source       s0(ST::nostopstate);
            ST::stop_source const s1;
            ST::stop_source       s2;

            s0.request_stop();
            s2.request_stop();

            return KT::type<bool> == KT::type<decltype(s1.stop_requested())>
                && noexcept(s1.stop_requested())
                && not s0.stop_requested()
                && not s1.stop_requested()
                && s2.stop_requested()
                ;
        }),
    KT::expect_success("request_stop", []{
            ST::stop_source s0(ST::nostopstate);
            ST::stop_source s1;
            //-dk:TODO check whether callbacks are called

            return KT::type<bool> == KT::type<decltype(s0.request_stop())>
                && noexcept(s0.request_stop())
                && not s0.request_stop()
                && not s0.stop_requested()
                && s1.request_stop()
                && s1.stop_requested()
                && not s1.request_stop()
                && s1.stop_requested()
                ;
        }),
};

static KT::add_tests suite("nstd/stop_token/stop_source", ::tests);
