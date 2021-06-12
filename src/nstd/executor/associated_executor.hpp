// nstd/executor/associated_executor.hpp                              -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTOR_ASSOCIATED_EXECUTOR
#define INCLUDED_NSTD_EXECUTOR_ASSOCIATED_EXECUTOR

#include "nstd/executor/system_executor.hpp"

// ----------------------------------------------------------------------------

namespace nstd::net {
    template <typename, class = ::nstd::net::system_executor>
    struct associated_executor;
    template <typename T, class Executor = ::nstd::net::system_executor>
    using associated_executor_t = typename ::nstd::net::associated_executor<T, Executor>::type;

    template <typename T>
        auto get_associated_executor(T const&) noexcept
            ->::nstd::net::associated_executor_t<T>;
    template <typename T, typename Executor>
        //-dk:TODO constrain this overload?
        auto get_associated_executor(T const&, Executor const&) noexcept
            ->::nstd::net::associated_executor_t<T, Executor>;
    template <typename T, typename ExecutionContext>
        auto get_associated_executor(T const&, ExecutionContext&) noexcept
            ->::nstd::net::associated_executor_t<T, typename ExecutionContext::executor_type>;
}

// ----------------------------------------------------------------------------

template <typename T, class Executor>
struct nstd::net::associated_executor
{
    using type = Executor;

    static auto get(T const&, Executor const& e = {}) noexcept -> type {
        return e;
    }
};

template <typename T, class Executor>
    requires requires{ typename T::executor_type; }
struct nstd::net::associated_executor<T, Executor>
{
    using type = typename T::executor_type;
    static auto get(T const& t, Executor const& = {}) noexcept -> type {
            return t.get_executor();
    }
};

// ----------------------------------------------------------------------------

template <typename T>
    auto nstd::net::get_associated_executor(T const& t) noexcept
        ->::nstd::net::associated_executor_t<T>
{
    return ::nstd::net::associated_executor<T>::get(t);
}

template <typename T, typename Executor>
    auto nstd::net::get_associated_executor(T const&t, Executor const& executor) noexcept
        ->::nstd::net::associated_executor_t<T, Executor>
{
    return ::nstd::net::associated_executor<T, Executor>::get(t, executor);
}

// ----------------------------------------------------------------------------

#endif
