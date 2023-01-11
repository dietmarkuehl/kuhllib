// nstd/execution/get_attrs.t.cpp                                     -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2023 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#include "nstd/execution/get_attrs.hpp"
#include "kuhl/test.hpp"

namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;
namespace EX = ::nstd::execution;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        struct env {};

        template <bool Noexcept>
        struct sender {
            friend auto tag_invoke(EX::get_attrs_t, sender const&) noexcept(Noexcept)
                -> TD::env {
                return {};
            }
        };

        template <typename T>
        concept has_get_attrs = requires(T const& obj) { EX::get_attrs(obj); };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("breathing", []{
            TD::sender<true> sender{};
            return KT::type<TD::env> == KT::type<decltype(EX::get_attrs(sender))>
                && TD::has_get_attrs<TD::sender<true>>
                ;
        }),
    KT::expect_success("throwing get_attrs isn't considered", []{
            return not TD::has_get_attrs<TD::sender<false>>
                ;
        }),
};

static KT::add_tests suite("nstd/execution/get_attrs", ::tests);
