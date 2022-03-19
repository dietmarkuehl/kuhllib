// nstd/execution/make_completion_signatures.t.cpp                    -*-C++-*-
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

#include "nstd/execution/make_completion_signatures.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/type_traits/add_lvalue_reference.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace EX = ::nstd::execution;
namespace HN = ::nstd::execution::hidden_names;
namespace TT = ::nstd::type_traits;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct env {};
        struct type {};
        struct error {};

        template <typename... S>
        struct sender {
            using completion_signatures = EX::completion_signatures<S...>;
        };

        template <typename... T>
        using set_value_ref = EX::set_value_t(TT::add_lvalue_reference_t<T>...);
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("TD::sender is a sender", []{
            using sender = TD::sender<EX::set_value_t(), EX::set_error_t(int), EX::set_stopped_t()>;
            static_assert(EX::sender<TD::sender<>>);
            return EX::sender<TD::sender<>>
                && EX::sender<sender>
                && KT::type<sender::completion_signatures>
                    == KT::type<EX::completion_signatures<EX::set_value_t(), EX::set_error_t(int), EX::set_stopped_t()>>
                ;
        }),
    KT::expect_success("default-set-value", []{
           return KT::type<HN::default_set_value<>>
                   == KT::type<EX::completion_signatures<EX::set_value_t()>>
               && KT::type<HN::default_set_value<TD::type>>
                   == KT::type<EX::completion_signatures<EX::set_value_t(TD::type)>>
               && KT::type<HN::default_set_value<TD::type&>>
                   == KT::type<EX::completion_signatures<EX::set_value_t(TD::type&)>>
               && KT::type<HN::default_set_value<TD::type, TD::type>>
                   == KT::type<EX::completion_signatures<EX::set_value_t(TD::type, TD::type)>>
               && KT::type<HN::default_set_value<TD::type&, int const&>>
                   == KT::type<EX::completion_signatures<EX::set_value_t(TD::type&, int const&)>>
               ;
                                                                
       }),
    KT::expect_success("default-set-error", []{
           return KT::type<HN::default_set_error<TD::error>>
                   == KT::type<EX::completion_signatures<EX::set_error_t(TD::error)>>
               && KT::type<HN::default_set_error<TD::error&>>
                   == KT::type<EX::completion_signatures<EX::set_error_t(TD::error&)>>
               && KT::type<HN::default_set_error<TD::error const&>>
                   == KT::type<EX::completion_signatures<EX::set_error_t(TD::error const&)>>
               && KT::type<HN::default_set_error<TD::error&&>>
                   == KT::type<EX::completion_signatures<EX::set_error_t(TD::error&&)>>
               ;
        }),
    KT::expect_success("basic make_completionsignatures", []{
           return KT::type<EX::make_completion_signatures<TD::sender<EX::set_value_t(), EX::set_error_t(int), EX::set_stopped_t()>>>
                   == KT::type<EX::completion_signatures<EX::set_value_t(), EX::set_error_t(int), EX::set_stopped_t()>>
               && KT::type<EX::make_completion_signatures<TD::sender<EX::set_value_t(), EX::set_error_t(int), EX::set_stopped_t()>, TD::env>>
                   == KT::type<EX::completion_signatures<EX::set_value_t(), EX::set_error_t(int), EX::set_stopped_t()>>
               ;
        }),
    KT::expect_success("make_completionsignatures set_value_transform", []{
           return KT::type<EX::make_completion_signatures<TD::sender<EX::set_value_t(int, TD::type), EX::set_error_t(int), EX::set_stopped_t()>,
                                                          TD::env, EX::completion_signatures<>, TD::set_value_ref>>
               == KT::type<EX::completion_signatures<EX::set_value_t(int&, TD::type&), EX::set_error_t(int), EX::set_stopped_t()>>
               ;
        }),
};

static KT::add_tests suite("make_completion_signatures", ::tests);
