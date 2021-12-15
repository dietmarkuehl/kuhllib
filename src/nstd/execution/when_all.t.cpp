// nstd/execution/when_all.t.cpp                                      -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/execution/when_all.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace EX = ::nstd::execution;
namespace TR = ::nstd::this_thread;
namespace TT = ::nstd::type_traits;
namespace UT = ::nstd::utility;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        template <typename...> class variant;
        template <typename...> class tuple;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("when_all without any sender", []{
            auto sender = EX::when_all();
            using Sender = decltype(sender);
            bool called = false;
            TR::sync_wait(UT::move(sender)
                          | EX::then([&]{ called = true; }));

            return KT::use(sender)
                && EX::sender<Sender>
                && KT::type<Sender::value_types<TD::tuple, TD::variant>>
                   == KT::type<TD::variant<TD::tuple<>>>
                && called
                ;
        }),
    KT::expect_success("when_all with multiple senders", []{
            ::std::size_t sum(0);
            auto sender = EX::when_all(
                EX::just(1) | EX::then([&sum](int v){ sum += v; }),
                EX::just(2) | EX::then([&sum](int v){ sum += v; }),
                EX::just(3) | EX::then([&sum](int v){ sum += v; })
                );
            using Sender = TT::remove_cvref_t<decltype(sender)>;
            bool called = false;
            TR::sync_wait(UT::move(sender)
                          | EX::then([&](auto&&...){ called = true; }));

            return KT::use(sender)
                && EX::sender<Sender>
                && called
                && sum == 6u
                ;
        }),
};

static KT::add_tests suite("when_all", ::tests);
