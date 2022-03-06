// nstd/hidden_names/completion_signature.t.cpp                       -*-C++-*-
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

#include "nstd/hidden_names/completion_signature.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_value.hpp"
#include "kuhl/test.hpp"

namespace test_declaration {}
namespace TD = ::test_declaration;
namespace EX = ::nstd::execution;
namespace HN = ::nstd::hidden_names;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        struct error {};
        template <int> struct type {};
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("non-completion signature", []{
            return not HN::completion_signature<void()>
                ;
        }),
    KT::expect_success("set_value_t() is a completion signature", []{
            return HN::completion_signature<EX::set_value_t()>
                ;
        }),
    KT::expect_success("set_value_t(T0) is a completion signature", []{
            return HN::completion_signature<EX::set_value_t(TD::type<0>)>
                && HN::completion_signature<EX::set_value_t(TD::type<0>&)>
                && HN::completion_signature<EX::set_value_t(TD::type<0> const&)>
                && HN::completion_signature<EX::set_value_t(TD::type<0>&&)>
                ;
        }),
    KT::expect_success("set_value_t(T0, T1) is a completion signature", []{
            return HN::completion_signature<EX::set_value_t(TD::type<0>, TD::type<1>)>
                && HN::completion_signature<EX::set_value_t(TD::type<0>, TD::type<1>&)>
                && HN::completion_signature<EX::set_value_t(TD::type<0>, TD::type<1> const&)>
                && HN::completion_signature<EX::set_value_t(TD::type<0>, TD::type<1>&&)>
                ;
        }),
    KT::expect_success("set_value_t(T0, T1, T2, T3) is a completion signature", []{
            return HN::completion_signature<EX::set_value_t(TD::type<0>, TD::type<1>, TD::type<2>, TD::type<3>)>
                && HN::completion_signature<EX::set_value_t(TD::type<0>, TD::type<1>, TD::type<2>, TD::type<3>&)>
                && HN::completion_signature<EX::set_value_t(TD::type<0>, TD::type<1>, TD::type<2>, TD::type<3> const&)>
                && HN::completion_signature<EX::set_value_t(TD::type<0>, TD::type<1>, TD::type<2>, TD::type<3>&&)>
                ;
        }),
    KT::expect_success("set_error_t(E) is a completion signature", []{
            return HN::completion_signature<EX::set_error_t(TD::error)>
                && HN::completion_signature<EX::set_error_t(TD::error&)>
                && HN::completion_signature<EX::set_error_t(TD::error const&)>
                && HN::completion_signature<EX::set_error_t(TD::error&&)>
                ;
        }),
    KT::expect_success("set_error_t() is not a completion signature", []{
            return not HN::completion_signature<EX::set_error_t()>
                ;
        }),
    KT::expect_success("set_error_t(E, int) is not a completion signature", []{
            return not HN::completion_signature<EX::set_error_t(TD::error, int)>
                ;
        }),
    KT::expect_success("set_stopped_t() is a completion signature", []{
            return HN::completion_signature<EX::set_stopped_t()>
                ;
        }),
    KT::expect_success("set_stopped_t(int) is not a completion signature", []{
            return not HN::completion_signature<EX::set_stopped_t(int)>
                ;
        }),
};

static KT::add_tests suite("nstd/hidden_names/completion_signature", ::tests);
