// nstd/execution/task.t.cpp                                          -*-C++-*-
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

#include "nstd/execution/task.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "kuhl/test.hpp"

namespace test_declaration {}
namespace TD = ::test_declaration;
namespace EX = ::nstd::execution;
namespace TT = ::nstd::this_thread;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("task<int>", []{
	    auto t = []()->EX::task<int> {
	            auto[x] = co_await EX::just(11);
		        co_return x;
            }();
			#if 0
			//-dk:TODO
	        auto v = TT::sync_wait(t | EX::then([](auto&& value){
	            return value;
		    }));
            return v && std::get<0>(*v) == 11
	        ;
			#else
			(void)t;
			return true;
			#endif
        }),
    KT::expect_success("task<>", []{
            bool called{false};
	    auto t = []()->EX::task<> {
	        co_await EX::just();
            }();
	    TT::sync_wait(t | EX::then([&](){
	        called = true;
		return 0;
		}));
            return called
	        ;
        }),
};

static KT::add_tests suite("nstd/execution/task", ::tests);
