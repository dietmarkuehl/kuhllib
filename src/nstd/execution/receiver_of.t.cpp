// nstd/execution/receiver_of.t.cpp                                   -*-C++-*-
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

#include "nstd/execution/receiver_of.hpp"
#include <exception>
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace EX = ::nstd::execution;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    struct env {};
    struct error {};
    struct non_error {};
    struct type {};

    template <typename Error, typename Env, bool SetErrorNoexcept, bool SetStoppedNoexcept, bool SetValueNoexcept>
    struct receiver
    {
        receiver(receiver&);
        receiver(receiver&&);
        friend auto tag_invoke(EX::get_env_t, receiver const&) -> Env { return {}; }
        friend auto tag_invoke(EX::set_value_t, receiver&&, int, double) noexcept(SetValueNoexcept) -> void {}
        friend auto tag_invoke(EX::set_error_t, receiver&&, Error) noexcept(SetErrorNoexcept) -> void {}
        friend auto tag_invoke(EX::set_stopped_t, receiver&&) noexcept(SetStoppedNoexcept) -> void {}
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("receiver_of", [](KT::context& )->bool{
            return EX::receiver_of<TD::receiver<TD::error, TD::env, true, true, true>,
                                   EX::completion_signatures<EX::set_stopped_t()>>
                && EX::receiver_of<TD::receiver<TD::error, TD::env, true, true, true>,
                                   EX::completion_signatures<EX::set_error_t(TD::error const&)>>
                && EX::receiver_of<TD::receiver<TD::error, TD::env, true, true, true>,
                                   EX::completion_signatures<EX::set_value_t(int, double)>>
                && not EX::receiver_of<TD::receiver<TD::error, TD::env, true, true, true>,
                                   EX::completion_signatures<EX::set_error_t(TD::non_error const&)>>
                && not EX::receiver_of<TD::receiver<TD::error, TD::env, true, true, true>,
                                   EX::completion_signatures<EX::set_value_t(TD::type, double)>>
                ;
        }),
    KT::expect_success("not receiver", [](KT::context& )->bool{
            using no_env = ::nstd::hidden_names::exec_envs::no_env;
            return not EX::receiver_of<TD::receiver<TD::error, no_env, true, true, true>,
                                   EX::completion_signatures<EX::set_stopped_t()>>
                && not EX::receiver_of<TD::receiver<TD::error, no_env, true, true, true>,
                                   EX::completion_signatures<EX::set_error_t(TD::error const&)>>
                && not EX::receiver_of<TD::receiver<TD::error, no_env, true, true, true>,
                                   EX::completion_signatures<EX::set_value_t(int, double)>>
                ;
        }),
    KT::expect_success("not receiver_of", [](KT::context& )->bool{
            using no_env = ::nstd::hidden_names::exec_envs::no_env;
            return not EX::receiver_of<TD::receiver<TD::error, no_env, true, false, true>,
                                   EX::completion_signatures<EX::set_stopped_t()>>
                && not EX::receiver_of<TD::receiver<TD::error, no_env, false, true, true>,
                                   EX::completion_signatures<EX::set_error_t(TD::error const&)>>
                && not EX::receiver_of<TD::receiver<TD::error, no_env, true, true, false>,
                                   EX::completion_signatures<EX::set_value_t(int, double)>>
                ;
        }),
};

static KT::add_tests suite("receiver_of", ::tests);
