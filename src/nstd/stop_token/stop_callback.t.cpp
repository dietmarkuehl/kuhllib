// nstd/stop_token/stop_callback.t.cpp                                -*-C++-*-
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

#include "nstd/stop_token/stop_callback.hpp"
#include "nstd/concepts/invocable.hpp"
#include "nstd/concepts/destructible.hpp"
#include "nstd/concepts/copy_constructible.hpp"
#include "nstd/stop_token/stop_source.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"
#include <concepts>
#include <type_traits>

namespace ST = ::nstd::stop_token_ns;
namespace UT = ::nstd::utility;
namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        struct arg { bool* value; };
        template <bool Noexcept>
        struct callback {
            bool* value;
            callback(TD::arg a) noexcept(Noexcept): value(a.value) {}
            auto operator()() -> void { *this->value = true; }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("test callback", []{
            return ::nstd::concepts::invocable<TD::callback<true>>
                && ::nstd::concepts::destructible<TD::callback<true>>
                && ::nstd::concepts::invocable<TD::callback<false>>
                && ::nstd::concepts::destructible<TD::callback<false>>
                ;
        }),
    KT::expect_success("test callback ctor", []{
            bool t{false};
            bool f{false};
            TD::callback<true>(TD::arg{&t})();
            TD::callback<false>(TD::arg{&f})();

            return ::std::is_nothrow_constructible_v<TD::callback<true>, TD::arg>
                && not ::std::is_nothrow_constructible_v<TD::callback<false>, TD::arg>
                && t
                && f
                ;
        }),
    KT::expect_success("copy ctor", []{
            return not ::nstd::concepts::copy_constructible<ST::stop_callback<TD::callback<true>>>;
        }),
    KT::expect_success("move ctor", []{
            return not ::nstd::concepts::move_constructible<ST::stop_callback<TD::callback<true>>>;
        }),
    KT::expect_success("copy assign", []{
            return not ::std::is_copy_assignable_v<ST::stop_callback<TD::callback<true>>>;
        }),
    KT::expect_success("move assign", []{
            return not ::std::is_move_assignable_v<ST::stop_callback<TD::callback<true>>>;
        }),
    KT::expect_success("callback_type assign", []{
            return KT::type<TD::callback<true>> == KT::type<ST::stop_callback<TD::callback<true>>::callback_type>;
        }),
    KT::expect_success("stop_token const& ctor", []{
            ST::stop_source      source;
            ST::stop_token const token{source.get_token()};
            bool                 value{false};

            source.request_stop();
            ST::stop_callback<TD::callback<true>>(token, TD::arg{&value});

            return ::std::is_constructible_v<ST::stop_callback<TD::callback<true>>, ST::stop_token const&, TD::arg>
                && ::std::is_constructible_v<ST::stop_callback<TD::callback<false>>, ST::stop_token const&, TD::arg>
                && noexcept(ST::stop_callback<TD::callback<true>>(token, TD::arg()))
                && not noexcept(ST::stop_callback<TD::callback<false>>(token, TD::arg()))
                && value
                ;
        }),
    KT::expect_success("stop_token&& ctor", []{
            ST::stop_source      source;
            bool                 value{false};
            ST::stop_token       token;

            source.request_stop();
            ST::stop_callback<TD::callback<true>>(source.get_token(), TD::arg{&value});

            return ::std::is_constructible_v<ST::stop_callback<TD::callback<true>>, ST::stop_token&&, TD::arg>
                && ::std::is_constructible_v<ST::stop_callback<TD::callback<false>>, ST::stop_token&&, TD::arg>
                && noexcept(ST::stop_callback<TD::callback<true>>(UT::move(token), TD::arg{}))
                && not noexcept(ST::stop_callback<TD::callback<false>>(UT::move(token), TD::arg{}))
                && value;
                ;
        }),
    KT::expect_success("register callbacks", []{
            ST::stop_source      source;
            ST::stop_token const token{source.get_token()};
            bool                 v0{false};
            bool                 v1{false};
            bool                 v2{false};

            ST::stop_callback<TD::callback<true>> c0(source.get_token(), TD::arg{&v0});
            ST::stop_callback<TD::callback<true>> c1(token, TD::arg{&v1});
            {
                ST::stop_callback<TD::callback<true>> c2(token, TD::arg{&v2});
            }
            source.request_stop();

            return ::std::is_constructible_v<ST::stop_callback<TD::callback<true>>, ST::stop_token&&, TD::arg>
                && ::std::is_constructible_v<ST::stop_callback<TD::callback<false>>, ST::stop_token&&, TD::arg>
                && noexcept(ST::stop_callback<TD::callback<true>>(UT::move(token), TD::arg{}))
                && not noexcept(ST::stop_callback<TD::callback<false>>(UT::move(token), TD::arg{}))
                && v0
                && v1
                && not v2
                ;
        }),
    KT::expect_success("CTAD", []{
            ST::stop_token token;
            ST::stop_callback callback(token, TD::callback<true>(TD::arg{nullptr}));
            return KT::type<ST::stop_callback<TD::callback<true>>> == KT::type<decltype(callback)>
                ;
        }),

};

static KT::add_tests suite("nstd/stop_token/stop_callback", ::tests);
