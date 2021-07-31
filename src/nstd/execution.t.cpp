// nstd/execution.t.cpp                                               -*-C++-*-
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

#include "nstd/execution.hpp"
#include <exception>
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace EX = ::nstd::execution;
namespace HN = ::nstd::hidden_names;
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    struct non_movable {
        non_movable(non_movable&&) = delete;
        non_movable(non_movable const&) = delete;
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("movable_value", []{
        return HN::movable_value<int>
            && not HN::movable_value<TD::non_movable>
            ;
    }),
    KT::expect_success("scheduler", []{
        return not EX::scheduler<int>
            ;
    }),
    KT::expect_success("receiver", []{
        return not EX::receiver<int>
            && not EX::receiver<int, ::std::exception_ptr>
            ;
    }),
    KT::expect_success("receiver_of", []{
        return not EX::receiver_of<int>
            && not EX::receiver_of<int, bool, char>
            ;
    }),
    KT::expect_success("schedule", []{
        return KT::assert_type_exists<EX::schedule_t>
            && KT::type<EX::schedule_t const> == KT::type<decltype(EX::schedule)>
            ;
    }),
    KT::expect_success("set_done", []{
        return KT::assert_type_exists<EX::set_done_t>
            && KT::type<EX::set_done_t const> == KT::type<decltype(EX::set_done)>
            ;
    }),
    KT::expect_success("get_scheduler", []{
        return KT::assert_type_exists<EX::get_scheduler_t>
            && KT::type<EX::get_scheduler_t const> == KT::type<decltype(EX::get_scheduler)>
            ;
    }),
    KT::expect_success("get_allocator", []{
        return KT::assert_type_exists<EX::get_allocator_t>
            && KT::type<EX::get_allocator_t const> == KT::type<decltype(EX::get_allocator)>
            ;
    }),
    KT::expect_success("get_stop_token", []{
        return KT::assert_type_exists<EX::get_stop_token_t>
            && KT::type<EX::get_stop_token_t const> == KT::type<decltype(EX::get_stop_token)>
            ;
    }),
    KT::expect_success("start", []{
        return KT::assert_type_exists<EX::start_t>
            && KT::type<EX::start_t const> == KT::type<decltype(EX::start)>
            ;
    }),
    KT::expect_success("operation_state", []{
        return not EX::operation_state<int>
            ;
    }),
    KT::expect_success("sender", []{
        return not EX::sender<int>
            ;
    }),
    KT::expect_success("sender_to", []{
        return not EX::sender_to<int, int>
            ;
    }),
    KT::expect_success("has_sender_types", []{
        return not HN::has_sender_types<int>;
    }),
    KT::expect_success("typed_sender", []{
        return not EX::typed_sender<int>;
    }),
    KT::expect_success("sender_base", []{
        return KT::assert_type_exists<EX::sender_base>;
    }),
    KT::expect_success("sender_traits", []{
        return KT::assert_type_exists<EX::sender_traits<int>>;
    }),
    KT::expect_success("connect", []{
        return KT::assert_type_exists<EX::connect_t>
            && KT::type<EX::connect_t const> == KT::type<decltype(EX::connect)>
            ;
    }),
};

static KT::add_tests suite("execution", ::tests);
