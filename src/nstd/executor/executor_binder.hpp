// nstd/executor/executor_binder.hpp                                  -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTOR_EXECUTOR_BINDER
#define INCLUDED_NSTD_EXECUTOR_EXECUTOR_BINDER

#include "nstd/executor/async_result.hpp"
#include "nstd/executor/associated_allocator.hpp"
#include "nstd/executor/associated_executor.hpp"
#include "nstd/type_traits/decay.hpp"

// ----------------------------------------------------------------------------

namespace nstd::inline net {
    template <typename, typename> class executor_binder;

    template <typename T, typename Executor, typename Signature>
    class async_result< ::nstd::net::executor_binder<T, Executor>, Signature>;

    template <typename T, typename Executor, typename ProtoAllocator>
    class associated_allocator< ::nstd::net::executor_binder<T, Executor>, ProtoAllocator>;

    template <typename T, typename Executor, typename ExecutorArg>
    class associated_executor< ::nstd::net::executor_binder<T, Executor>, ExecutorArg>;

    template <typename Executor, typename T>
        auto bind_executor(Executor const&, T&&)
            -> ::nstd::net::executor_binder<::nstd::type_traits::decay_t<T>, Executor>;
    template <typename ExecutionContext, typename T>
        //-dk:TODO constraint the overload?
        auto bind_executor(ExecutionContext&, T&&)
            -> ::nstd::net::executor_binder<::nstd::type_traits::decay_t<T>, typename ExecutionContext::executor_type>;
}

// ----------------------------------------------------------------------------

#endif
