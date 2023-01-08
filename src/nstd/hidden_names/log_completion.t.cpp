// nstd/hidden_names/log_completion.t.cpp                             -*-C++-*-
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

#include "nstd/hidden_names/log_completion.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "kuhl/test.hpp"
#include <sstream>
#include <iostream>

namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;
namespace HN = ::nstd::hidden_names;
namespace EX = ::nstd::execution;
namespace TR = ::nstd::this_thread;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("breathing", []{
            ::std::ostringstream out;
            TR::sync_wait(HN::log_completion(EX::just(), out));
            return out.str() == "set_value(...)\n"
                ;
        }),
    KT::expect_success("use with pipe", []{
            ::std::ostringstream out;
            TR::sync_wait(EX::just() | HN::log_completion(out));
            return out.str() == "set_value(...)\n"
                ;
        }),
    KT::expect_success("default stream", []{
            ::std::ostringstream out;
            ::std::streambuf*    cout_sbuf{::std::cout.rdbuf(out.rdbuf())};

            TR::sync_wait(HN::log_completion(EX::just()));
            ::std::cout.rdbuf(cout_sbuf);
            return out.str() == "set_value(...)\n"
                ;
        }),
    KT::expect_success("use with pipe and default stream", []{
            ::std::ostringstream out;
            ::std::streambuf*    cout_sbuf{::std::cout.rdbuf(out.rdbuf())};

            TR::sync_wait(EX::just() | HN::log_completion());
            ::std::cout.rdbuf(cout_sbuf);
            return out.str() == "set_value(...)\n"
                ;
        }),
    KT::expect_success("set_value with message", []{
            ::std::ostringstream out;
            TR::sync_wait(HN::log_completion(EX::just(), "foo", out));
            return out.str() == "foo set_value(...)\n"
                ;
        }),
    KT::expect_success("use with pipe and message", []{
            ::std::ostringstream out;
            TR::sync_wait(EX::just() | HN::log_completion("foo", out));
            return out.str() == "foo set_value(...)\n"
                ;
        }),
    KT::expect_success("use with pipe and message and efault stream", []{
            ::std::ostringstream out;
            ::std::streambuf*    cout_sbuf{::std::cout.rdbuf(out.rdbuf())};

            TR::sync_wait(EX::just() | HN::log_completion("foo"));
            ::std::cout.rdbuf(cout_sbuf);
            return out.str() == "foo set_value(...)\n"
                ;
        }),
    KT::expect_success("set_error without message", []{
            ::std::ostringstream out;
            TR::sync_wait(HN::log_completion(EX::just_error(0), out) | EX::upon_error([](auto&&){}));
            return out.str() == "set_error(E)\n"
                ;
        }),
    KT::expect_success("set_error with message", []{
            ::std::ostringstream out;
            TR::sync_wait(HN::log_completion(EX::just_error(0), "foo", out) | EX::upon_error([](auto&&){}));
            return out.str() == "foo set_error(E)\n"
                ;
        }),
    KT::expect_success("set_stopped without message", []{
            ::std::ostringstream out;
            TR::sync_wait(HN::log_completion(EX::just_stopped(), out));
            return out.str() == "set_stopped()\n"
                ;
        }),
    KT::expect_success("set_stopped with message", []{
            ::std::ostringstream out;
            TR::sync_wait(HN::log_completion(EX::just_stopped(), "foo", out));
            return out.str() == "foo set_stopped()\n"
                ;
        }),
};

static KT::add_tests suite("nstd/hidden_names/log_completion", ::tests);
