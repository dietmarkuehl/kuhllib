// nstd/executor/executor.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTOR_EXECUTOR
#define INCLUDED_NSTD_EXECUTOR_EXECUTOR

// ----------------------------------------------------------------------------

#include "nstd/executor/async_result.hpp"
#include "nstd/executor/async_completion.hpp"
#include "nstd/executor/associated_allocator.hpp"
#include "nstd/executor/associated_executor.hpp"
#include "nstd/executor/fork_event.hpp"
#include "nstd/executor/execution_context.hpp"
#include "nstd/executor/executor_binder.hpp"
#include "nstd/executor/service_already_exists.hpp"
#include "nstd/executor/system_executor.hpp"
#include "nstd/type_traits/integral_constant.hpp"

// ----------------------------------------------------------------------------

namespace nstd::net {
    template <typename> struct is_executor: std::false_type {};
    template <typename T>
    inline constexpr bool is_executor_v = ::nstd::net::is_executor<T>::value;

    struct executor_arg_t { explicit executor_arg_t() = default; };
    inline constexpr executor_arg_t executor_arg{};

    template <typename, typename> struct uses_executor: std::false_type {};
    template <typename T, typename Executor>
    inline constexpr bool uses_executor_v = ::nstd::net::uses_executor<T, Executor>::value;

    template <typename> class executor_work_guard;
    template <typename Executor>
        auto make_work_guard(Executor const&)
            -> ::nstd::net::executor_work_guard<Executor>;
    template <typename ExecutionContext>
        auto make_work_guard(ExecutionContext&)
            -> ::nstd::net::executor_work_guard<typename ExecutionContext::executor_type>;
    //-dk:TODO template <typename T>
    //-dk:TODO     auto make_work_guard(T const&)
    //-dk:TODO         -> ::nstd::net::executor_work_guard<::nstd::net::associated_executor<T>>;

    class bad_executor;
    class executor;

    //-dk:TODO make hidden friends?
    auto operator== (::nstd::net::executor const&, ::nstd::net::executor const&) -> bool;
    auto operator== (::nstd::net::executor const&, decltype(nullptr)) -> bool;

    //-dk:TODO dispatch
    //-dk:TODO post
    //-dk:TODO defer

    template <typename> class strand;
    template <typename Executor>
    auto operator== (::nstd::net::strand<Executor> const&, ::nstd::net::strand<Executor> const&) -> bool;

    template <typename = ::std::allocator<void>> class use_future_t {};
    inline constexpr use_future_t<> use_future{};
    template <typename ProtoAllocator, typename Result, typename... Args>
    class async_result<::nstd::net::use_future_t<ProtoAllocator>, Result(Args...)>;
    //-dk:TODO template <typename Result, typename... Args, typename Signature>
    //-dk:TODO class async_result<::std::packaged_task<Result(Args...)>, Signature>;
}

// ----------------------------------------------------------------------------

#endif
