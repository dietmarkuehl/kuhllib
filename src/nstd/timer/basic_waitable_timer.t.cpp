// nstd/timer/basic_waitable_timer.t.cpp                              -*-C++-*-
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

#include "nstd/timer/basic_waitable_timer.hpp"
#include "kuhl/test.hpp"

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
    KT::expect_success("basic_waitable_timer", []{
            return KT::assert_template_exists<NN::basic_waitable_timer, ::std::chrono::system_clock>
                && KT::assert_template_exists<NN::basic_waitable_timer, ::std::chrono::system_clock, ::NN::wait_traits<::std::chrono::system_clock>>
                ;
        }),
    KT::expect_success("aliases", []{
            return KT::type<NN::basic_waitable_timer<::std::chrono::system_clock, ::NN::wait_traits<::std::chrono::system_clock>>>
                    == KT::type<NN::system_timer>
                && KT::type<NN::basic_waitable_timer<::std::chrono::steady_clock, ::NN::wait_traits<::std::chrono::steady_clock>>>
                    == KT::type<NN::steady_timer>
                && KT::type<NN::basic_waitable_timer<::std::chrono::high_resolution_clock, ::NN::wait_traits<::std::chrono::high_resolution_clock>>>
                    == KT::type<NN::high_resolution_timer>
                ;
        }),
};

static KT::add_tests suite("nstd/timer/basic_waitable_timer", ::tests);
