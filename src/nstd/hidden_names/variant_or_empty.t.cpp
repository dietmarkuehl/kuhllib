// src/nstd/hidden_names/variant_or_empty.t.cpp                       -*-C++-*-
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

#include "nstd/hidden_names/variant_or_empty.hpp"
#include "kuhl/test.hpp"
#include <type_traits>

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace HN = ::nstd::hidden_names;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct type {};
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("empty type can't be default constructed", []{
            return not ::std::is_constructible_v<HN::variant_or_empty_impl::empty_variant>
                ;
        }),
    KT::expect_success("variant_or_empty<> yields empty type", []{
            return KT::type<HN::variant_or_empty<>> == KT::type<HN::variant_or_empty_impl::empty_variant>
                ;
        }),
    KT::expect_success("variant_or_empty<T, ...> yields variant type", []{
            return KT::type<HN::variant_or_empty<TD::type>> == KT::type<::std::variant<TD::type>>
                && KT::type<HN::variant_or_empty<TD::type, int>> == KT::type<::std::variant<TD::type, int>>
                && KT::type<HN::variant_or_empty<TD::type, int, bool>> == KT::type<::std::variant<TD::type, int, bool>>
                ;
        }),
};

static KT::add_tests suite("variant_or_empty", ::tests);