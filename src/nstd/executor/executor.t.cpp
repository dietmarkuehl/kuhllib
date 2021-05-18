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

namespace EX = ::nstd::executor;
namespace TT = ::nstd::type_traits;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct completion_token;
    struct example_type;
    struct service;
}
namespace nstd::executor
{
    template <typename ProtoAllocator>
    class associated_allocator<example_type, ProtoAllocator>
    {
    public:
        using type = ::std::allocator<example_type>;
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("executor.hpp declaration are present", [](KT::context& )->bool{
           return KT::assert_template_exists<EX::async_result, completion_token, int(int)>
               && KT::assert_template_exists<EX::async_completion, completion_token, int(int)>
               && KT::assert_template_exists<EX::associated_allocator, example_type>
               && KT::assert_template_exists<EX::associated_allocator, example_type, ::std::allocator<void>>
               && KT::assert_template_exists<EX::associated_allocator_t, example_type>
               && KT::assert_template_exists<EX::associated_allocator_t, example_type, ::std::allocator<void>>
               && KT::type<EX::associated_allocator_t<example_type>>
                   == KT::type<decltype(EX::get_associated_allocator(TT::declval<example_type const&>()))>
               && noexcept(EX::get_associated_allocator(TT::declval<example_type const&>()))
               && KT::type<EX::associated_allocator_t<example_type>>
                   == KT::type<decltype(EX::get_associated_allocator(TT::declval<example_type const&>(),
                                                                     TT::declval<::std::allocator<void> const&>()))>
               && noexcept(EX::get_associated_allocator(TT::declval<example_type const&>(),
                                                        TT::declval<::std::allocator<void> const&>()))
               && KT::assert_type_exists<EX::fork_event>
               && KT::type<EX::fork_event> == KT::type<decltype(EX::fork_event::prepare)>
               && KT::type<EX::fork_event> == KT::type<decltype(EX::fork_event::parent)>
               && KT::type<EX::fork_event> == KT::type<decltype(EX::fork_event::child)>
               && KT::assert_type_exists<EX::execution_context>
               && KT::assert_type_exists<EX::service_already_exists>
               && KT::type<service&> == KT::type<decltype(EX::use_service<service>(TT::declval<EX::execution_context&>()))>
               && KT::type<service&> == KT::type<decltype(EX::make_service<service>(TT::declval<EX::execution_context&>()))>
               && KT::type<bool> == KT::type<decltype(EX::has_service<service>(TT::declval<EX::execution_context&>()))>
               && true
               ;
        }),
};

static KT::add_tests suite("executor", ::tests);
