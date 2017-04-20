// nstd/execution/parallel_policy.hpp                                 -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2017 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_EXECUTION_PARALLEL_POLICY
#define INCLUDED_NSTD_EXECUTION_PARALLEL_POLICY

#include "nstd/execution/is_execution_policy.hpp"
#include "nstd/base/for_each.hpp"
#include "nstd/algorithm/distance.hpp"
#include "nstd/execution/thread_pool_executor.hpp"
#include "nstd/iterator/random_access.hpp"
#include "nstd/type_traits/integral_constant.hpp"
#include "nstd/type_traits/enable_if.hpp"
#include <iostream>

// ----------------------------------------------------------------------------

namespace nstd {
    namespace execution {
        struct parallel_policy {
            unsigned size;
            parallel_policy operator()(unsigned s) const;
        };
        constexpr parallel_policy par{65536u};

        template <typename MultiPass, typename EndPoint, typename Callable>
        ::nstd::type_traits::enable_if_t<!::nstd::iterator::is_random_access<EndPoint>::value>
        map(::nstd::execution::parallel_policy const&,
            MultiPass begin, EndPoint end, Callable fun);
        template <typename MultiPass, typename EndPoint, typename Callable>
        ::nstd::type_traits::enable_if_t<::nstd::iterator::is_random_access<EndPoint>::value>
        map(::nstd::execution::parallel_policy const&,
            MultiPass begin, EndPoint end, Callable fun);
    }

    template <>
    struct is_execution_policy<::nstd::execution::parallel_policy>
        : public ::nstd::type_traits::true_type {
    };
}

// ----------------------------------------------------------------------------

template <typename MultiPass, typename EndPoint, typename Callable>
::nstd::type_traits::enable_if_t<!::nstd::iterator::is_random_access<EndPoint>::value>
nstd::execution::map(::nstd::execution::parallel_policy const&,
                     MultiPass cur, EndPoint end, Callable fun) {
    //-dk:TODO support parallel version for non-random access
    ::nstd::base::for_each(cur, end, fun);
}

// ----------------------------------------------------------------------------

template <typename MultiPass, typename EndPoint, typename Callable>
::nstd::type_traits::enable_if_t<::nstd::iterator::is_random_access<EndPoint>::value>
nstd::execution::map(::nstd::execution::parallel_policy const& policy,
                     MultiPass cur, EndPoint end, Callable fun) {
    ::nstd::execution::thread_pool_executor executor; //-dk:TODO use the argument?
    auto size(policy.size);
    for (size = ::nstd::algorithm::distance(cur, end);
         policy.size <= size; size -= policy.size) {
        auto tmp(cur + policy.size);
        executor.add([=](){ ::nstd::base::for_each(cur, tmp, fun); });
        cur = tmp;
    }
    if (cur != end) {
        executor.add([=](){ ::nstd::base::for_each(cur, end, fun); });
    }
    executor.process();
}

// ----------------------------------------------------------------------------

inline
::nstd::execution::parallel_policy
nstd::execution::parallel_policy::operator()(unsigned s) const {
    return ::nstd::execution::parallel_policy{s};
}

// ----------------------------------------------------------------------------

#endif
