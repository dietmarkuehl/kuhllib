// nstd/file/io_object.t.cpp                                          -*-C++-*-
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

#include "nstd/file/io_object.hpp"
#include "kuhl/test.hpp"

namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;
namespace NF = ::nstd::file;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        struct non_io {};
        struct type_only {
            using native_handle_type = int;
        };
        struct handle_only {
            auto native_handle() const -> int { return {}; }
        };
        struct non_const_handle {
            using native_handle_type = int;
            auto native_handle() noexcept -> native_handle_type { return {}; }
        };
        struct throwing_handle {
            using native_handle_type = int;
            auto native_handle() const -> native_handle_type { return {}; }
        };
        struct wrong_type {
            using native_handle_type = int;
            auto native_handle() const noexcept -> bool { return {}; }
        };
        struct io_object {
            using native_handle_type = int;
            auto native_handle() const noexcept -> native_handle_type { return {}; }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("non-io_object", []{ return not NF::io_object<TD::non_io>; }),
    KT::expect_success("type only", []{ return not NF::io_object<TD::type_only>; }),
    KT::expect_success("handle only", []{ return not NF::io_object<TD::handle_only>; }),
    KT::expect_success("non-const handle ", []{ return not NF::io_object<TD::non_const_handle>; }),
    KT::expect_success("throwing handle", []{ return not NF::io_object<TD::throwing_handle>; }),
    KT::expect_success("wrong_type", []{ return not NF::io_object<TD::wrong_type>; }),
    KT::expect_success("io_object", []{ return NF::io_object<TD::io_object>; }),
};

static KT::add_tests suite("nstd/file/io_object", ::tests);
