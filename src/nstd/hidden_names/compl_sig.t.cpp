// nstd/hidden_names/compl_sig.t.cpp                                  -*-C++-*-
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

#include "nstd/hidden_names/compl_sig.hpp"
#include "nstd/execution/set_value.hpp"
#include "kuhl/test.hpp"

namespace EX = ::nstd::execution;
namespace HN = ::nstd::hidden_names;
namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        struct type;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("basic use", []{
            return KT::type<EX::set_value_t()> == KT::type<HN::compl_sig_t<EX::set_value_t>>
                && KT::type<EX::set_value_t(TD::type)> == KT::type<HN::compl_sig_t<EX::set_value_t, TD::type>>
                && KT::type<EX::set_value_t(TD::type, bool&)> == KT::type<HN::compl_sig_t<EX::set_value_t, TD::type, bool&>>
                ;
        }),
    KT::expect_success("void use", []{
            return KT::type<EX::set_value_t()> == KT::type<HN::compl_sig_t<EX::set_value_t, void>>
                ;
        }),
};

static KT::add_tests suite("hidden_names::compl_sig_t", ::tests);
