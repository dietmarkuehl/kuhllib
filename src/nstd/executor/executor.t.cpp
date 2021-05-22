// nstd/executor/executor.t.cpp                                       -*-C++-*-
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

#include "nstd/executor/executor.hpp"
#include "nstd/type_traits/declval.hpp"
#include "kuhl/test.hpp"

namespace NET = ::nstd::net;
namespace TT = ::nstd::type_traits;
namespace KT = ::kuhl::test;
namespace test_declarations {}
namespace TD = ::test_declarations;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    struct completion_token;
    struct example_type;
    struct service;
    struct executor {};
    struct execution_context { using executor_type = executor; };
}
namespace nstd::net
{
    template <typename ProtoAllocator>
    class associated_allocator<TD::example_type, ProtoAllocator>
    {
    public:
        using type = ::std::allocator<TD::example_type>;
    };

    template <typename Executor>
    struct associated_executor<TD::example_type, Executor>
    {
        using type = TD::executor;
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("executor.hpp declaration are present", [](KT::context& )->bool{
           return KT::assert_template_exists<NET::async_result, TD::completion_token, int(int)>
               && KT::assert_template_exists<NET::async_completion, TD::completion_token, int(int)>
               && KT::assert_template_exists<NET::associated_allocator, TD::example_type>
               && KT::assert_template_exists<NET::associated_allocator, TD::example_type, ::std::allocator<void>>
               && KT::assert_template_exists<NET::associated_allocator_t, TD::example_type>
               && KT::assert_template_exists<NET::associated_allocator_t, TD::example_type, ::std::allocator<void>>
               && KT::type<NET::associated_allocator_t<TD::example_type>>
                   == KT::type<decltype(NET::get_associated_allocator(TT::declval<TD::example_type const&>()))>
               && noexcept(NET::get_associated_allocator(TT::declval<TD::example_type const&>()))
               && KT::type<NET::associated_allocator_t<TD::example_type>>
                   == KT::type<decltype(NET::get_associated_allocator(TT::declval<TD::example_type const&>(),
                                                                     TT::declval<::std::allocator<void> const&>()))>
               && noexcept(NET::get_associated_allocator(TT::declval<TD::example_type const&>(),
                                                        TT::declval<::std::allocator<void> const&>()))
               && KT::assert_type_exists<NET::fork_event>
               && KT::type<NET::fork_event> == KT::type<decltype(NET::fork_event::prepare)>
               && KT::type<NET::fork_event> == KT::type<decltype(NET::fork_event::parent)>
               && KT::type<NET::fork_event> == KT::type<decltype(NET::fork_event::child)>
               && KT::assert_type_exists<NET::execution_context>
               && KT::assert_type_exists<NET::service_already_exists>
               && KT::type<TD::service&> == KT::type<decltype(NET::use_service<TD::service>(TT::declval<NET::execution_context&>()))>
               && KT::type<TD::service&> == KT::type<decltype(NET::make_service<TD::service>(TT::declval<NET::execution_context&>()))>
               && KT::type<bool> == KT::type<decltype(NET::has_service<TD::service>(TT::declval<NET::execution_context&>()))>
               && KT::assert_template_exists<NET::is_executor, int>
               //-dk:TODO && KT::type<bool const> == KT::type<decltype(NET::is_executor_v<int>)>
               && KT::assert_type_exists<NET::executor_arg_t>
               && KT::type<NET::executor_arg_t> == KT::type<decltype(NET::executor_arg_t())>
               && KT::type<NET::executor_arg_t const> == KT::type<decltype(NET::executor_arg)>
               && KT::assert_template_exists<NET::uses_executor, int, TD::executor>
                //-dk:TODO && KT::type bool const == KT::type<decltype(NET::uses_executor, int, TD::executor)
               && KT::assert_template_exists<NET::associated_executor, int, TD::executor>
                //-dk:TODO verify the default argument
                //-dk:TODO && KT::assert_template_exists<NET::associated_executor_t, int, TD::executor>
               && noexcept(NET::get_associated_executor(TT::declval<TD::example_type const&>()))
               && KT::type<TD::executor> == KT::type<decltype(NET::get_associated_executor(TT::declval<TD::example_type const&>()))>
               && noexcept(NET::get_associated_executor(TT::declval<TD::example_type const&>(),
                                                       TT::declval<TD::executor const&>()))
               && KT::type<TD::executor> == KT::type<decltype(NET::get_associated_executor(TT::declval<TD::example_type const&>(),
                                                              TT::declval<TD::executor const&>()))>
               && noexcept(NET::get_associated_executor(TT::declval<TD::example_type const&>(),
                                                       TT::declval<TD::execution_context&>()))
               && KT::type<TD::executor> == KT::type<decltype(NET::get_associated_executor(TT::declval<TD::example_type const&>(),
                                                              TT::declval<TD::execution_context&>()))>
               && KT::assert_template_exists<NET::executor_binder, TD::example_type, TD::executor>
               && KT::type<NET::executor_binder<TD::example_type, TD::executor>>
                   == KT::type<decltype(NET::bind_executor(TT::declval<TD::executor const&>(), TT::declval<TD::example_type>()))>
               && KT::assert_template_exists<NET::executor_work_guard, TD::executor>
               && KT::type<NET::executor_work_guard<TD::executor>>
                   == KT::type<decltype(NET::make_work_guard(TT::declval<TD::executor const&>()))>
               && KT::assert_type_exists<NET::system_executor>
               && KT::type<bool> == KT::type<decltype(TT::declval<NET::system_executor const&>() == TT::declval<NET::system_executor const&>())>
               && KT::assert_type_exists<NET::system_context>
               && KT::assert_type_exists<NET::bad_executor>
               && KT::assert_type_exists<NET::executor>
               && KT::type<bool> == KT::type<decltype(TT::declval<NET::executor const&>() == TT::declval<NET::executor const&>())>
               && KT::type<bool> == KT::type<decltype(TT::declval<NET::executor const&>() == nullptr)>
               //-dk:TODO dispatch
               //-dk:TODO post
               //-dk:TODO defer
               && KT::assert_template_exists<NET::strand, TD::executor>
               && KT::type<bool> == KT::type<decltype(TT::declval<NET::strand<TD::executor> const&>() == TT::declval<NET::strand<TD::executor> const&>())>
               && KT::assert_template_exists<NET::use_future_t>
               && KT::assert_template_exists<NET::use_future_t, std::allocator<void>>
               && KT::type<NET::use_future_t<> const> == KT::type<decltype(NET::use_future)>
               && true
               ;
        }),
};

static KT::add_tests suite("executor", ::tests);
