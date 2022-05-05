// nstd/execution/run_loop.t.cpp                                      -*-C++-*-
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

#include "nstd/execution/run_loop.hpp"
#include "nstd/execution/scheduler.hpp"
#include "kuhl/test.hpp"
#include <exception>
#include <type_traits>

namespace EX = ::nstd::execution;
namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("exec.run_loop.ctor", []{
            EX::run_loop loop;
            KT::use(loop);
            return noexcept(EX::run_loop())
                && not ::std::is_copy_constructible_v<EX::run_loop>
                && not ::std::is_move_constructible_v<EX::run_loop>
                && not ::std::is_copy_assignable_v<EX::run_loop>
                && not ::std::is_move_assignable_v<EX::run_loop>
                && KT::test_terminate([]{
                    // if count == 0 or state is running
                    ::std::terminate(); //-dk:TODO
                })
                ;
        }),
    KT::expect_success("exec.run_loop.members get_scheduler", []{
            using scheduler_type = decltype(EX::run_loop().get_scheduler());
            EX::run_loop l1;
            EX::run_loop l2;

            return KT::type<EX::run_loop::scheduler> == KT::type<scheduler_type>
                && EX::scheduler<scheduler_type>
                && l1.get_scheduler() == l1.get_scheduler()
                && l2.get_scheduler() == l2.get_scheduler()
                && l1.get_scheduler() != l2.get_scheduler()
                ;
        }),
};

static KT::add_tests suite("nstd/execution/run_loop", ::tests);
