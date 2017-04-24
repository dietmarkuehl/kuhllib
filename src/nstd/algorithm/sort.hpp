// nstd/algorithm/sort.hpp                                            -*-C++-*-
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

#ifndef INCLUDED_NSTD_ALGORITHM_SORT
#define INCLUDED_NSTD_ALGORITHM_SORT

#include "nstd/execution/execution.hpp"
#include "nstd/functional/less.hpp"
#include "nstd/type_traits/enable_if.hpp"
#include <algorithm>

// ----------------------------------------------------------------------------

namespace nstd {
    namespace algorithm {
        namespace detail {
            struct sort {
                constexpr sort() noexcept(true) {}
                template <typename RndIt, typename EndPoint,
                          typename Compare = ::nstd::functional::less<>>
                    auto operator()(RndIt it, EndPoint,
                                    Compare compare = Compare()) const
                    -> nstd::type_traits::enable_if_t<!nstd::is_execution_policy<RndIt>::value>;
                template <typename ExecutionPolicy,
                          typename RndIt, typename EndPoint,
                          typename Compare = ::nstd::functional::less<>>
                    auto operator()(ExecutionPolicy const&,
                                    RndIt it, EndPoint,
                                    Compare compare = Compare()) const
                    -> nstd::type_traits::enable_if_t<nstd::is_execution_policy<ExecutionPolicy>::value>;
            };
        }
        constexpr nstd::algorithm::detail::sort sort{};
    }
}

// ----------------------------------------------------------------------------

template <typename RndIt, typename EndPoint,
          typename Compare>
auto ::nstd::algorithm::detail::sort::operator()(RndIt it, EndPoint end,
                                                 Compare compare) const
    -> nstd::type_traits::enable_if_t<!nstd::is_execution_policy<RndIt>::value> {
    ::std::sort(it, end, compare);
}

// ----------------------------------------------------------------------------

template <typename ExecutionPolicy,
          typename RndIt, typename EndPoint,
          typename Compare>
auto ::nstd::algorithm::detail::sort::operator()(ExecutionPolicy const& ep,
                                                 RndIt it, EndPoint end,
                                                 Compare compare) const
    -> ::nstd::type_traits::enable_if_t<::nstd::is_execution_policy<ExecutionPolicy>::value> {
    ::nstd::execution::sort(ep, it, end, compare);
}

// ----------------------------------------------------------------------------

#endif
