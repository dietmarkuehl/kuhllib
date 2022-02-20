// src/nstd/hidden_names/is_instance_of.t.cpp                         -*-C++-*-
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

#include "nstd/hidden_names/is_instance_of.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {};
namespace KT = ::kuhl::test;
namespace HN = ::nstd::hidden_names;
namespace TD = test_declarations;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    namespace {
        struct type {};

        template <typename...> struct tmpl1;
        template <typename...> struct tmpl2;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_failure("breathing", []{
            static_assert(HN::is_instance_of<TD::tmpl1<TD::type, int>, TD::tmpl1>);
            static_assert(not HN::is_instance_of<TD::tmpl1<TD::type, int>, TD::tmpl2>);

            return HN::is_instance_of<TD::tmpl1<TD::type, int>, TD::tmpl1>
                && not HN::is_instance_of<TD::tmpl1<TD::type, int>, TD::tmpl2>
                ;
        }),
};

KT::add_tests suite("is_instance_of", ::tests);
