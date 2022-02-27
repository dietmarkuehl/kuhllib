// nstd/execution/sender_to.t.cpp                                     -*-C++-*-
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

#include "nstd/execution/sender_to.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/operation_state.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/sender_base.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include <exception>
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace EX = ::nstd::execution;
namespace TT = ::nstd::type_traits;
namespace UT = ::nstd::utility;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct result {};

        template <typename T>
        struct receiver
        {
            friend auto tag_invoke(EX::set_value_t, receiver&&, T) noexcept -> void {}
            friend auto tag_invoke(EX::set_error_t, receiver&&, ::std::exception_ptr) noexcept -> void {}
            friend auto tag_invoke(EX::set_stopped_t, receiver&&) noexcept -> void {}
        };

        template <typename Receiver>
        struct state
        {
            TT::remove_cvref_t<Receiver> receiver;
            friend auto tag_invoke(EX::start_t, state& s)
                noexcept
                requires requires(Receiver&& r) {
                    EX::set_value(UT::move(receiver), TD::result());
                    UT::move(receiver);
                }
            {
                EX::set_value(UT::move(s.receiver), TD::result());
            }
        };

        struct sender
            : EX::sender_base
        {
            template <typename Receiver>
            friend auto tag_invoke(EX::connect_t, sender&&, Receiver&& receiver) {
                return TD::state<Receiver>{UT::forward<Receiver>(receiver)};
            }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("test classes receiver<int>", []{
            return EX::receiver<TD::receiver<int>>;
        }),
    KT::expect_success("test classes receiver<TD::result>", []{
            return EX::receiver<TD::receiver<TD::result>>;
        }),
    KT::expect_success("test classes sender<TD::sender>", []{
            return EX::sender<TD::sender>;
        }),
    KT::expect_success("test classes operation_state<state<receiver<result>>>", []{
            return EX::operation_state<TD::state<TD::receiver<TD::result>>>;
        }),
    KT::expect_success("sender_to<int, int> is false", []{
            return !EX::sender_to<int, int>;
        }),
    KT::expect_success("sender_to<TD::sender, TD::receiver<int>> is false", []{
            return !EX::sender_to<TD::sender, TD::receiver<int>>;
        }),
    KT::expect_success("sender_to<TD::sender, TD::receiver<TD::result>> is true", []{
            return EX::sender_to<TD::sender, TD::receiver<TD::result>>;
        }),
};

static KT::add_tests suite("sender_to", ::tests);
