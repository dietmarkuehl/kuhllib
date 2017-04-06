// nstd/algorithm/for_each.hpp                                        -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2014 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_ALGORITHM_FOR_EACH
#define INCLUDED_NSTD_ALGORITHM_FOR_EACH

#include "nstd/cursor/single_pass.hpp"
#include "nstd/execution/execution.hpp"
#include "nstd/projection/identity.hpp"
#include "nstd/type_traits/enable_if.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/utility/pair.hpp"
#include <exception>

// ----------------------------------------------------------------------------

namespace nstd {
    namespace algorithm {
        namespace detail {
            struct for_each {
                constexpr for_each() noexcept(true) {}
                template <typename Readable, typename SinglePass, typename EndPoint, typename Callable>
                auto operator()(Readable, SinglePass, EndPoint, Callable) const
                    -> nstd::type_traits::enable_if_t<!nstd::is_execution_policy_v<Readable>,
                                                      nstd::utility::pair<SinglePass, Callable>>;
                template <typename ForwardIterator, typename EndPoint, typename Callable>
                auto operator()(ForwardIterator, EndPoint, Callable) const
                    -> nstd::type_traits::enable_if_t<!nstd::is_execution_policy_v<ForwardIterator>,
                                                      nstd::utility::pair<ForwardIterator, Callable>>;

                template <typename ExecutionPolicy, typename Readable, typename MultiPass, typename EndPoint, typename Callable>
                auto operator()(ExecutionPolicy&&, Readable, MultiPass, EndPoint, Callable) const
                    -> nstd::type_traits::enable_if_t<nstd::is_execution_policy_v<ExecutionPolicy>>;

                template <typename ExecutionPolicy, typename ForwardIterator, typename EndPoint, typename Callable>
                auto operator()(ExecutionPolicy&&, ForwardIterator, EndPoint, Callable) const
                    -> nstd::type_traits::enable_if_t<nstd::is_execution_policy_v<ExecutionPolicy>>;
            };
        }
        constexpr nstd::algorithm::detail::for_each for_each{};
    }

}

// ----------------------------------------------------------------------------

template <typename Readable, typename SinglePass, typename EndPoint, typename Callable>
auto nstd::algorithm::detail::for_each::operator()(Readable readable,
                                                   SinglePass cur, EndPoint end,
                                                   Callable fun) const
    -> nstd::type_traits::enable_if_t<!nstd::is_execution_policy_v<Readable>,
                                      nstd::utility::pair<SinglePass, Callable>>
{
    // The actual implementation of for_each()
    namespace NC = nstd::cursor;
    for (; !NC::at_same_pos(cur, end); NC::step(cur)) {
        fun(readable(NC::key(cur)));
    }
    return nstd::utility::make_pair(cur, std::move(fun));
}

template <typename ForwardIterator, typename EndPoint, typename Callable>
auto nstd::algorithm::detail::for_each::operator()(ForwardIterator begin,
                                                   EndPoint        end,
                                                   Callable        fun) const
    -> nstd::type_traits::enable_if_t<!nstd::is_execution_policy_v<ForwardIterator>,
                                      nstd::utility::pair<ForwardIterator, Callable>> {
    // Got an iterator => forward to projection/cursor version with identity projection
    return (*this)(nstd::projection::identity, begin, end, nstd::utility::move(fun));
}

// ----------------------------------------------------------------------------

template <typename ExecutionPolicy, typename Readable, typename MultiPass, typename EndPoint, typename Callable>
auto nstd::algorithm::detail::for_each::operator()(ExecutionPolicy&& ep,
                                                   Readable          readable,
                                                   MultiPass         begin,
                                                   EndPoint          end,
                                                   Callable          fun) const
    -> nstd::type_traits::enable_if_t<nstd::is_execution_policy_v<ExecutionPolicy>> {
    try {
        execute(ep, begin, end, [readable, fun = std::move(fun)](auto&& begin, auto&& end) {
                ::nstd::algorithm::for_each(readable, begin, end, fun);
            });
    }
    catch (...) {
        std::terminate();
    }
}

template <typename ExecutionPolicy, typename ForwardIterator, typename EndPoint, typename Callable>
auto nstd::algorithm::detail::for_each::operator()(ExecutionPolicy&& ep,
                                                   ForwardIterator   begin,
                                                   EndPoint          end,
                                                   Callable          fun) const
    -> nstd::type_traits::enable_if_t<nstd::is_execution_policy_v<ExecutionPolicy>> {
    (*this)(nstd::utility::forward<ExecutionPolicy>(ep),
           nstd::projection::identity, begin, end, nstd::utility::move(fun));
}

// ----------------------------------------------------------------------------

#endif
