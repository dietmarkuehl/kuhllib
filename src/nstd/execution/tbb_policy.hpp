// nstd/execution/tbb_policy.hpp                                      -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_TBB_POLICY
#define INCLUDED_NSTD_EXECUTION_TBB_POLICY

// ----------------------------------------------------------------------------

#include "nstd/execution/is_execution_policy.hpp"
#include "nstd/type_traits/integral_constant.hpp"
#include "nstd/base/for_each.hpp"
#include "nstd/algorithm/distance.hpp"
#include "tbb/parallel_for.h"
#include "tbb/parallel_reduce.h"
#include "tbb/parallel_sort.h"
#include "tbb/blocked_range.h"
#include <numeric>

// ----------------------------------------------------------------------------

namespace nstd {
    namespace execution {
        struct tbb_policy {
        };
        constexpr tbb_policy tbb;

        template <typename MultiPass, typename EndPoint, typename Callable>
        void map(::nstd::execution::tbb_policy const&,
                 MultiPass begin, EndPoint end, Callable fun);

        template <typename FwdIt, typename EndPoint, typename Init, typename Reduce>
        auto reduce(::nstd::execution::tbb_policy const&,
                    FwdIt it, EndPoint end, Init init, Reduce op)
            -> decltype(op(*it, *it));

        template <typename RndIt, typename EndPoint, typename Compare>
        auto sort(::nstd::execution::tbb_policy const&,
                    RndIt it, EndPoint end, Compare compare)
            -> void;
    }

    template <>
    struct is_execution_policy<::nstd::execution::tbb_policy>
        : public ::nstd::type_traits::true_type {
    };
}

// ----------------------------------------------------------------------------

template <typename MultiPass, typename EndPoint, typename Callable>
void
nstd::execution::map(::nstd::execution::tbb_policy const&,
                     MultiPass begin, EndPoint end, Callable fun) {
    auto size(::nstd::algorithm::distance(begin, end));
    ::tbb::parallel_for(::tbb::blocked_range<int>(0, size),
                        [&](::tbb::blocked_range<int> r){
                            ::nstd::base::for_each(begin + r.begin(), begin + r.end(), fun);
                        });
}

// ----------------------------------------------------------------------------

template <typename FwdIt, typename EndPoint, typename Init, typename Reduce>
auto
nstd::execution::reduce(::nstd::execution::tbb_policy const&,
                       FwdIt it, EndPoint end, Init init, Reduce op)
    -> decltype(op(*it, *it)) {
    return tbb::parallel_reduce(tbb::blocked_range<int>(0, end - it),
                                init,
                                [=](tbb::blocked_range<int> const& range,
                                    auto const& value) {
                                    return ::std::accumulate(it + range.begin(),
                                                           it + range.end(),
                                                           value, op);
                                }, op);
}

// ----------------------------------------------------------------------------

template <typename RndIt, typename EndPoint, typename Compare>
auto nstd::execution::sort(::nstd::execution::tbb_policy const&,
                           RndIt it, EndPoint end, Compare compare)
    -> void {
    tbb::parallel_sort(it, end, compare);
}

// ----------------------------------------------------------------------------

#endif
