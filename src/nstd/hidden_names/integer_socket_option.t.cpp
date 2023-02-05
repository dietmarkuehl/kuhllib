// nstd/hidden_names/integer_socket_option.t.cpp                      -*-C++-*-
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

#include "nstd/hidden_names/integer_socket_option.hpp"
#include "kuhl/test.hpp"

namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;
namespace HN = ::nstd::hidden_names;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        struct option {
            option() noexcept = default;
            option(int) noexcept {}
            auto value() const noexcept -> int { return {}; }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("concept", []{
            return not HN::integer_socket_option<int>
                && HN::integer_socket_option<TD::option>
                && HN::integer_socket_option<HN::integer_socket_option_t<3>>
                ;
        }),
    KT::expect_success("breathing", []{
            return HN::integer_socket_option_t<0>().value() == int()
                ;
        }),
    KT::expect_success("ctor", []{
            return HN::integer_socket_option_t<0>(5).value() == 5
                ;
        }),
};

static KT::add_tests suite("nstd/hidden_names/integer_socket_option", ::tests);
