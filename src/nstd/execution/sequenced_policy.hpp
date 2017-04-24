// nstd/execution/sequenced_policy.h                                  -*-C++-*-
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
//  The above copyright not ice and this permission notice shall be       
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

#ifndef INCLUDED_NSTD_EXECUTION_SEQUENCED_POLICY
#define INCLUDED_NSTD_EXECUTION_SEQUENCED_POLICY

#include "nstd/execution/is_execution_policy.hpp"
#include "nstd/base/for_each.hpp"
#include "nstd/type_traits/integral_constant.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace execution {
        class sequenced_policy {
        };
        constexpr sequenced_policy seq{};

        template <typename MultiPass, typename EndPoint, typename Callable>
        auto map(::nstd::execution::sequenced_policy const&,
                 MultiPass begin, EndPoint end, Callable fun)
            -> void;

        template <typename FwdIt, typename EndPoint, typename Init, typename Reduce>
        auto reduce(::nstd::execution::sequenced_policy const&,
                    FwdIt it, EndPoint end, Init init, Reduce op)
            -> decltype(op(*it, *it));
    }

    template <>
    struct is_execution_policy<::nstd::execution::sequenced_policy>
        : public ::nstd::type_traits::true_type {
    };
}

// ----------------------------------------------------------------------------

template <typename MultiPass, typename EndPoint, typename Callable>
void nstd::execution::map(::nstd::execution::sequenced_policy const&,
                          MultiPass begin, EndPoint end, Callable fun) {
    ::nstd::base::for_each(begin, end, fun);
}

// ----------------------------------------------------------------------------

template <typename FwdIt, typename EndPoint, typename Init, typename Reduce>
auto nstd::execution::reduce(::nstd::execution::sequenced_policy const&,
            FwdIt it, EndPoint end, Init init, Reduce op)
    -> decltype(op(*it, *it)) {
    auto value(init);
    for (; it != end; ++it) {
        value = op(value, *it);
    }
    return value;
}

// ----------------------------------------------------------------------------

#endif
