// nstd/hidden_names/filter_completions.t.cpp                         -*-C++-*-
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

#include "nstd/hidden_names/filter_completions.hpp"
#include "nstd/execution/completion_signatures.hpp"
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
        struct type {};
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("empty", []{
            return KT::type<EX::completion_signatures<>>
                    == 
                    KT::type<typename HN::filter_completions<
                        EX::set_value_t,
                        EX::completion_signatures<>,
                        EX::completion_signatures<>
                        >::type>
                && KT::type<EX::completion_signatures<>>
                    == 
                    KT::type<typename HN::filter_completions<
                        EX::set_error_t,
                        EX::completion_signatures<>,
                        EX::completion_signatures<>
                        >::type>
                && KT::type<EX::completion_signatures<>>
                    == 
                    KT::type<typename HN::filter_completions<
                        EX::set_stopped_t,
                        EX::completion_signatures<>,
                        EX::completion_signatures<>
                        >::type>
                        ;
        }),
    KT::expect_success("set_value", []{
            return KT::type<EX::completion_signatures<
                    EX::set_value_t(TD::type),
                    EX::set_value_t(TD::type, int),
                    EX::set_value_t()
                    >>
                    == 
                    KT::type<typename HN::filter_completions<
                        EX::set_value_t,
                        EX::completion_signatures<>,
                        EX::completion_signatures<
                            EX::set_value_t(TD::type),
                            EX::set_value_t(TD::type, int),
                            EX::set_value_t(),
                            EX::set_error_t(TD::error),
                            EX::set_error_t(int),
                            EX::set_stopped_t()
                            >
                        >::type>
                        ;
        }),
    KT::expect_success("set_error", []{
            return KT::type<EX::completion_signatures<
                    EX::set_error_t(TD::error),
                    EX::set_error_t(int)
                    >>
                    == 
                    KT::type<typename HN::filter_completions<
                        EX::set_error_t,
                        EX::completion_signatures<>,
                        EX::completion_signatures<
                            EX::set_value_t(TD::type),
                            EX::set_value_t(TD::type, int),
                            EX::set_value_t(),
                            EX::set_error_t(TD::error),
                            EX::set_error_t(int),
                            EX::set_stopped_t()
                            >
                        >::type>
                        ;
        }),
    KT::expect_success("set_stopped", []{
            return KT::type<EX::completion_signatures<
                    EX::set_stopped_t()
                    >>
                    == 
                    KT::type<typename HN::filter_completions<
                        EX::set_stopped_t,
                        EX::completion_signatures<>,
                        EX::completion_signatures<
                            EX::set_value_t(TD::type),
                            EX::set_value_t(TD::type, int),
                            EX::set_value_t(),
                            EX::set_error_t(TD::error),
                            EX::set_error_t(int),
                            EX::set_stopped_t()
                            >
                        >::type>
                        ;
        }),
};

static KT::add_tests suite("nstd/hidden_names/filter_completions", ::tests);
