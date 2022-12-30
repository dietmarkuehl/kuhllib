// nstd/buffer/sequence.t.cpp                                         -*-C++-*-
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

#include "nstd/buffer/sequence.hpp"
#include "nstd/buffer/const_buffer.hpp"
#include "kuhl/test.hpp"
#include <array>
#include <vector>

namespace test_declaration {}
namespace NN = ::nstd::net;
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("array", []{
            char data[] = "hello";
            ::std::array<NN::const_buffer, 3> bs{ NN::buffer(data), NN::buffer(data), NN::buffer(data) };
            auto const& cbs(bs);

            return NN::buffer_sequence_begin(bs)  == bs.begin()
                && NN::buffer_sequence_end(bs)    == bs.end()
                && NN::buffer_sequence_begin(cbs) == cbs.begin()
                && NN::buffer_sequence_end(cbs)   == cbs.end()
                ;
        }),
    KT::expect_success("vector", []{
            char data[] = "hello";
            ::std::vector<NN::const_buffer> bs{ NN::buffer(data), NN::buffer(data), NN::buffer(data) };
            auto const& cbs(bs);

            return NN::buffer_sequence_begin(bs)  == bs.begin()
                && NN::buffer_sequence_end(bs)    == bs.end()
                && NN::buffer_sequence_begin(cbs) == cbs.begin()
                && NN::buffer_sequence_end(cbs)   == cbs.end()
                ;
        }),
};

static KT::add_tests suite("nstd/buffer/sequence", ::tests);
