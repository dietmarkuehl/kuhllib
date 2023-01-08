// nstd/net/socket.t.cpp                                              -*-C++-*-
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

#include "nstd/net/socket.hpp"
#include "kuhl/test.hpp"

namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;
namespace NN = ::nstd::net;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        struct socket {
            using native_handle_type = int;
            using protocol_type = int;

            auto native_handle() const -> native_handle_type { return {}; }
            auto protocol() const -> protocol_type { return {}; }
        };

        template <typename NativeT, typename ProtocolT>
        struct non_socket {
            using native_handle_type = int;
            using protocol_type = int;

            auto native_handle() const -> NativeT { return {}; }
            auto protocol() const -> ProtocolT { return {}; }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("type with proper native_handle and protocol is a socket", []{
            return NN::socket<TD::socket>
                ;
        }),
    KT::expect_success("type without proper protocol and native_handle isn't a socket", []{
            return not NN::socket<TD::non_socket<int, bool>>
                && not NN::socket<TD::non_socket<bool, int>>
                && NN::socket<TD::non_socket<int, int>>
                ;
        }),
};

static KT::add_tests suite("nstd/net/socket", ::tests);
