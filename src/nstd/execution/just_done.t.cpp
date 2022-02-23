// nstd/execution/just_done.t.cpp                                     -*-C++-*-
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

#include "nstd/execution/just.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "nstd/utility/move.hpp"
#include <optional>
#include <tuple>
#include <variant>
#include "kuhl/test.hpp"

namespace EX = ::nstd::execution;
namespace TT = ::nstd::this_thread;
namespace UT = ::nstd::utility;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("breathing", []{
            auto sender{EX::just_done()};
            auto rc{TT::sync_wait(UT::move(sender))};
            return KT::type<decltype(rc)> == KT::type<::std::optional<::std::variant<::std::monostate>>>
                && !rc
                ;
        }),
    KT::expect_success("just_done + then", []{
            auto called{false};
            auto sender{EX::just_done() | EX::then([&called]{ called = true; })};
            auto rc{TT::sync_wait(UT::move(sender))};
            return KT::type<decltype(rc)> == KT::type<::std::optional<::std::variant<::std::monostate>>>
                && !rc
                && !called
                ;
        }),
};

static KT::add_tests suite("just_done", ::tests);
