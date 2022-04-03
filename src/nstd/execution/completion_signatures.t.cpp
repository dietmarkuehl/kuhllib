// src/nstd/execution/completion_signatures.t.cpp                     -*-C++-*-
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

#include "nstd/execution/completion_signatures.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {};
namespace TD = test_declarations;
namespace KT = ::kuhl::test;
namespace EX = ::nstd::execution;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct type {};
        template <typename... T>
        struct list:
            EX::completion_signatures<T...>
        {
            int value;
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("breathing", []{
            return sizeof(TD::list<int, int, int>) == sizeof(int)
                && 0u != sizeof(EX::completion_signatures<TD::type, TD::type>)
                ;
        }),
};

static KT::add_tests suite("completion_signatures", ::tests);