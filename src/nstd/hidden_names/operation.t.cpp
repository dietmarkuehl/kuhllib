// nstd/hidden_names/operation.t.cpp                                  -*-C++-*-
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

#include "nstd/hidden_names/operation.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/sync_wait.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace EX = ::nstd::execution;
namespace TD = ::test_declarations;
namespace HN = ::nstd::hidden_names;
namespace UT = ::nstd::utility;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        template <typename...> struct variant;
        template <typename...> struct tuple;
        struct action_t {};

        template <typename R>
        auto tag_invoke(action_t, int arg, R&& receiver) {
            EX::set_value(UT::forward<R>(receiver), arg);
        } 

        inline constexpr HN::operation<action_t, TD::variant<TD::tuple<int>>, TD::tuple<std::exception_ptr>>
            action;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("factory creation of a sender", []{
            auto sender = TD::action(17);
            /*auto res =*/ EX::sync_wait(UT::move(sender));
            return KT::use(sender)
                && EX::sender<decltype(sender)>
                ;
        }),
#if 0
    KT::expect_success("direct creation with a sender", []{
            auto sender = TD::action(EX::just(17));
            /*auto res =*/ EX::sync_wait(UT::move(sender));
            return KT::use(sender)
                && EX::sender<decltype(sender)>
                ;
        }),
#endif
};

static KT::add_tests suite("operation", ::tests);
