// src/nstd/hidden_names/add_signatures.t.cpp                         -*-C++-*-
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

#include "nstd/hidden_names/add_signatures.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "kuhl/test.hpp"

namespace EX = ::nstd::execution;
namespace HN = ::nstd::hidden_names;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("empty list without addition stays empty", []{
            return KT::type<HN::add_signatures_t<EX::completion_signatures<>>>
                    == KT::type<EX::completion_signatures<>>
            ;
        }),
    KT::expect_success("adding one element to an empty list", []{
            return KT::type<HN::add_signatures_t<EX::completion_signatures<>, void()>>
                    == KT::type<EX::completion_signatures<void()>>
            ;
        }),
    KT::expect_success("adding multiple elements to an empty list", []{
            return KT::type<HN::add_signatures_t<EX::completion_signatures<>, void(), bool(), int()>>
                    == KT::type<EX::completion_signatures<void(), bool(), int()>>
            ;
        }),
    KT::expect_success("adding one element to a non-empty list", []{
            return KT::type<HN::add_signatures_t<EX::completion_signatures<long(), float()>, void()>>
                    == KT::type<EX::completion_signatures<long(), float(), void()>>
            ;
        }),
    KT::expect_success("adding multiple elements to a non-empty list", []{
            return KT::type<HN::add_signatures_t<EX::completion_signatures<long(), float()>, void(), bool(), int()>>
                    == KT::type<EX::completion_signatures<long(), float(), void(), bool(), int()>>
            ;
        }),
    KT::expect_success("adding duplicate first element to a non-empty list", []{
            return KT::type<HN::add_signatures_t<EX::completion_signatures<long(), float()>, void(), bool(), long(), int()>>
                    == KT::type<EX::completion_signatures<long(), float(), void(), bool(), int()>>
            ;
        }),
    KT::expect_success("adding duplicate last element to a non-empty list", []{
            return KT::type<HN::add_signatures_t<EX::completion_signatures<long(), float()>, float(), void(), bool(), int()>>
                    == KT::type<EX::completion_signatures<long(), float(), void(), bool(), int()>>
            ;
        }),
    KT::expect_success("adding duplicate middle element to a non-empty list", []{
            return KT::type<HN::add_signatures_t<EX::completion_signatures<long(), bool(), float()>, void(), bool(), float(), int()>>
                    == KT::type<EX::completion_signatures<long(), bool(), float(), void(), int()>>
            ;
        }),
};

static KT::add_tests suite("add_signatures", ::tests);
