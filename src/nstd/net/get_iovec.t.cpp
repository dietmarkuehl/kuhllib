// nstd/net/get_iovec.t.cpp                                           -*-C++-*-
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

#include "nstd/net/get_iovec.hpp"
#include "nstd/buffer/const_buffer.hpp"
#include "nstd/buffer/mutable_buffer.hpp"
#include "kuhl/test.hpp"
#include <array>
#include <vector>
#include <concepts>

namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;
namespace NN = ::nstd::net;
namespace HN = ::nstd::hidden_names;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        struct env {};
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("mutable buffer", []{
            TD::env env{};
            char    buf[4]{};

            auto iov = NN::get_iovec(env, NN::buffer(buf));
            return iov.size() == 1
                && iov.data()[0].iov_base == buf
                && iov.data()[0].iov_len  == sizeof(buf)
                && not ::std::copy_constructible<decltype(iov)>
                && not ::std::move_constructible<decltype(iov)>
                && ::std::same_as<HN::get_iovec::array_iovec<1u>, decltype(iov)>
                ;
        }),
    KT::expect_success("const buffer", []{
            TD::env    env{};
            char const buf[4]{};

            auto iov = NN::get_iovec(env, NN::buffer(buf));
            return iov.size() == 1
                && iov.data()[0].iov_base == buf
                && iov.data()[0].iov_len  == sizeof(buf)
                && not ::std::copy_constructible<decltype(iov)>
                && not ::std::move_constructible<decltype(iov)>
                && ::std::same_as<HN::get_iovec::array_iovec<1u>, decltype(iov)>
                ;
        }),
    KT::expect_success("mutable buffer built-in array", []{
            TD::env env{};
            char    buf0[4]{};
            char    buf1[5]{};
            char    buf2[3]{};
            char    buf3[7]{};
            NN::mutable_buffer bseq[]{
                NN::buffer(buf0),
                NN::buffer(buf1),
                NN::buffer(buf2),
                NN::buffer(buf3),
            };

            auto iov = NN::get_iovec(env, bseq);
            return iov.size() == 4
                && iov.data()[0].iov_base == buf0
                && iov.data()[0].iov_len  == sizeof(buf0)
                && iov.data()[1].iov_base == buf1
                && iov.data()[1].iov_len  == sizeof(buf1)
                && iov.data()[2].iov_base == buf2
                && iov.data()[2].iov_len  == sizeof(buf2)
                && iov.data()[3].iov_base == buf3
                && iov.data()[3].iov_len  == sizeof(buf3)
                && not ::std::copy_constructible<decltype(iov)>
                && not ::std::move_constructible<decltype(iov)>
                && ::std::same_as<HN::get_iovec::array_iovec<4u>, decltype(iov)>
                ;
        }),
    KT::expect_success("const buffer built-in array", []{
            TD::env    env{};
            char const buf0[4]{};
            char const buf1[5]{};
            char const buf2[3]{};
            char const buf3[7]{};
            NN::const_buffer bseq[]{
                NN::buffer(buf0),
                NN::buffer(buf1),
                NN::buffer(buf2),
                NN::buffer(buf3),
            };

            auto iov = NN::get_iovec(env, bseq);
            return iov.size() == 4
                && iov.data()[0].iov_base == buf0
                && iov.data()[0].iov_len  == sizeof(buf0)
                && iov.data()[1].iov_base == buf1
                && iov.data()[1].iov_len  == sizeof(buf1)
                && iov.data()[2].iov_base == buf2
                && iov.data()[2].iov_len  == sizeof(buf2)
                && iov.data()[3].iov_base == buf3
                && iov.data()[3].iov_len  == sizeof(buf3)
                && not ::std::copy_constructible<decltype(iov)>
                && not ::std::move_constructible<decltype(iov)>
                && ::std::same_as<HN::get_iovec::array_iovec<4u>, decltype(iov)>
                ;
        }),
    KT::expect_success("mutable buffer std::array", []{
            TD::env env{};
            char    buf0[4]{};
            char    buf1[5]{};
            char    buf2[3]{};
            char    buf3[7]{};
            ::std::array<NN::mutable_buffer, 4> bseq{
                NN::buffer(buf0),
                NN::buffer(buf1),
                NN::buffer(buf2),
                NN::buffer(buf3),
            };

            auto iov = NN::get_iovec(env, bseq);
            return iov.size() == 4
                && iov.data()[0].iov_base == buf0
                && iov.data()[0].iov_len  == sizeof(buf0)
                && iov.data()[1].iov_base == buf1
                && iov.data()[1].iov_len  == sizeof(buf1)
                && iov.data()[2].iov_base == buf2
                && iov.data()[2].iov_len  == sizeof(buf2)
                && iov.data()[3].iov_base == buf3
                && iov.data()[3].iov_len  == sizeof(buf3)
                && not ::std::copy_constructible<decltype(iov)>
                && not ::std::move_constructible<decltype(iov)>
                && ::std::same_as<HN::get_iovec::array_iovec<4u>, decltype(iov)>
                ;
        }),
    KT::expect_success("const buffer std::array", []{
            TD::env    env{};
            char const buf0[4]{};
            char const buf1[5]{};
            char const buf2[3]{};
            char const buf3[7]{};
            ::std::array<NN::const_buffer, 4> bseq{
                NN::buffer(buf0),
                NN::buffer(buf1),
                NN::buffer(buf2),
                NN::buffer(buf3),
            };

            auto iov = NN::get_iovec(env, bseq);
            return iov.size() == 4
                && iov.data()[0].iov_base == buf0
                && iov.data()[0].iov_len  == sizeof(buf0)
                && iov.data()[1].iov_base == buf1
                && iov.data()[1].iov_len  == sizeof(buf1)
                && iov.data()[2].iov_base == buf2
                && iov.data()[2].iov_len  == sizeof(buf2)
                && iov.data()[3].iov_base == buf3
                && iov.data()[3].iov_len  == sizeof(buf3)
                && not ::std::copy_constructible<decltype(iov)>
                && not ::std::move_constructible<decltype(iov)>
                && ::std::same_as<HN::get_iovec::array_iovec<4u>, decltype(iov)>
                ;
        }),
    KT::expect_success("mutable buffer std::vector", []{
            TD::env env{};
            char    buf0[4]{};
            char    buf1[5]{};
            char    buf2[3]{};
            char    buf3[7]{};
            ::std::vector<NN::mutable_buffer> bseq{
                NN::buffer(buf0),
                NN::buffer(buf1),
                NN::buffer(buf2),
                NN::buffer(buf3),
            };

            auto iov = NN::get_iovec(env, bseq);
            return iov.size() == 4
                && iov.data()[0].iov_base == buf0
                && iov.data()[0].iov_len  == sizeof(buf0)
                && iov.data()[1].iov_base == buf1
                && iov.data()[1].iov_len  == sizeof(buf1)
                && iov.data()[2].iov_base == buf2
                && iov.data()[2].iov_len  == sizeof(buf2)
                && iov.data()[3].iov_base == buf3
                && iov.data()[3].iov_len  == sizeof(buf3)
                && not ::std::copy_constructible<decltype(iov)>
                && not ::std::move_constructible<decltype(iov)>
                ;
        }),
    KT::expect_success("const buffer std::vector", []{
            TD::env    env{};
            char const buf0[4]{};
            char const buf1[5]{};
            char const buf2[3]{};
            char const buf3[7]{};
            ::std::vector<NN::const_buffer> bseq{
                NN::buffer(buf0),
                NN::buffer(buf1),
                NN::buffer(buf2),
                NN::buffer(buf3),
            };

            auto iov = NN::get_iovec(env, bseq);
            return iov.size() == 4
                && iov.data()[0].iov_base == buf0
                && iov.data()[0].iov_len  == sizeof(buf0)
                && iov.data()[1].iov_base == buf1
                && iov.data()[1].iov_len  == sizeof(buf1)
                && iov.data()[2].iov_base == buf2
                && iov.data()[2].iov_len  == sizeof(buf2)
                && iov.data()[3].iov_base == buf3
                && iov.data()[3].iov_len  == sizeof(buf3)
                && not ::std::copy_constructible<decltype(iov)>
                && not ::std::move_constructible<decltype(iov)>
                ;
        }),
};

static KT::add_tests suite("nstd/net/get_iovec", ::tests);
