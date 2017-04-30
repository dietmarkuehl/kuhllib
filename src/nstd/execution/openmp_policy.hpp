// nstd/execution/openmp_policy.hpp                                   -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_OPENMP_POLICY
#define INCLUDED_NSTD_EXECUTION_OPENMP_POLICY

#include "nstd/execution/is_execution_policy.hpp"
#include "nstd/type_traits/integral_constant.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace execution {
        struct openmp_policy {
        };
        constexpr openmp_policy omp;

        template <typename MultiPass, typename EndPoint, typename Callable>
        void map(::nstd::execution::openmp_policy const&,
                 MultiPass begin, EndPoint end, Callable fun);
    }

    template <>
    struct is_execution_policy<::nstd::execution::openmp_policy>
        : public ::nstd::type_traits::true_type {
    };
}

// ----------------------------------------------------------------------------

template <typename MultiPass, typename EndPoint, typename Callable>
void
nstd::execution::map(::nstd::execution::openmp_policy const&,
                     MultiPass begin, EndPoint end, Callable fun) {
    #pragma omp parallel for
    for (auto cur = begin; cur < end; ++cur) {
        fun(*cur);
    }
}

// ----------------------------------------------------------------------------

#endif
