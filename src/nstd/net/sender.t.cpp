// nstd/net/sender.t.cpp                                              -*-C++-*-
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

#include "nstd/net/sender.hpp"
#include "kuhl/test.hpp"

namespace NN = ::nstd::net;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("async_accept", []{
            return KT::use(NN::async_accept_adapter);
        }),
    KT::expect_success("async_connect", []{
            return KT::use(NN::async_connect_adapter);
        }),
    KT::expect_success("async_read_some", []{
            return KT::use(NN::async_read_some_adapter);
        }),
    KT::expect_success("async_receive", []{
            return KT::use(NN::async_receive_adapter);
        }),
    KT::expect_success("async_receive_from", []{
            return KT::use(NN::async_receive_from_adapter);
        }),
    KT::expect_success("async_send", []{
            return KT::use(NN::async_send_adapter);
        }),
    KT::expect_success("async_send_to", []{
            return KT::use(NN::async_send_to_adapter);
        }),
    KT::expect_success("async_write", []{
            return KT::use(NN::async_write); //-dk:TODO
        }),
    KT::expect_success("async_write_some", []{
            return KT::use(NN::async_write_some_adapter);
        }),
    KT::expect_success("async_write_some", []{
            return 0 != sizeof(NN::scope);
        }),
};

static KT::add_tests suite("nstd/net/sender", ::tests);
