// nstd/algorithm/reduce.hpp                                          -*-C++-*-
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

#ifndef INCLUDED_NSTD_ALGORITHM_REDUCE
#define INCLUDED_NSTD_ALGORITHM_REDUCE

#include "nstd/execution/execution.hpp"
#include "nstd/functional/plus.hpp"
#include "nstd/type_traits/enable_if.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/decay.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace algorithm {
        namespace detail {
            struct reduce {
                constexpr reduce() noexcept(true) {}
                template <typename FwdIt, typename EndPoint,
                          typename Init = ::nstd::type_traits::decay_t<decltype(*::nstd::type_traits::declval<FwdIt>())>,
                          typename Reduce = nstd::functional::plus<>>
                    auto operator()(FwdIt it, EndPoint,
                                    Init init = Init(), Reduce op = Reduce()) const
                    -> nstd::type_traits::enable_if_t<!nstd::is_execution_policy<FwdIt>::value,
                                                      decltype(op(*it, *it))>;
                template <typename ExecutionPolicy,
                          typename FwdIt, typename EndPoint,
                          typename Init = ::nstd::type_traits::decay_t<decltype(*::nstd::type_traits::declval<FwdIt>())>,
                          typename Reduce = nstd::functional::plus<>>
                    auto operator()(ExecutionPolicy const&, FwdIt it, EndPoint,
                                    Init init = Init(), Reduce op = Reduce()) const
                    -> nstd::type_traits::enable_if_t<nstd::is_execution_policy<ExecutionPolicy>::value,
                                                      decltype(op(*it, *it))>;
            };
        }
        constexpr nstd::algorithm::detail::reduce reduce{};
    }
}

// ----------------------------------------------------------------------------

template <typename FwdIt, typename EndPoint,
          typename Init,
          typename Reduce>
auto nstd::algorithm::detail::reduce::operator()(FwdIt it, EndPoint end,
                                                 Init init, Reduce op) const
    -> nstd::type_traits::enable_if_t<!nstd::is_execution_policy<FwdIt>::value,
                                      decltype(op(*it, *it))>
{
    return (*this)(::nstd::execution::seq, it, end, init, op);
}

// ----------------------------------------------------------------------------

template <typename ExecutionPolicy,
          typename FwdIt, typename EndPoint,
          typename Init,
          typename Reduce>
auto nstd::algorithm::detail::reduce::operator()(ExecutionPolicy const& ep,
                                                 FwdIt it, EndPoint end,
                                                 Init init, Reduce op) const
    -> nstd::type_traits::enable_if_t<nstd::is_execution_policy<ExecutionPolicy>::value,
                                      decltype(op(*it, *it))>
{
    return ::nstd::execution::reduce(ep, it, end, init, op);
}

// ----------------------------------------------------------------------------

#endif
