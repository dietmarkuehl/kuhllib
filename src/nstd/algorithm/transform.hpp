// nstd/algorithm/transform.hpp                                       -*-C++-*-
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

#ifndef INCLUDED_NSTD_ALGORITHM_TRANSFORM
#define INCLUDED_NSTD_ALGORITHM_TRANSFORM

#include "nstd/type_traits/enable_if.hpp"
#include "nstd/execution/is_execution_policy.hpp"
#include "nstd/utility/pair.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace algorithm {
        namespace detail {
            struct transform {
                constexpr transform() noexcept(true) {}
                template <typename Readable, typename SinglePass, typename EndPoint, typename Callable>
                auto operator()(Readable, SinglePass, EndPoint, Callable) const
                    -> nstd::type_traits::enable_if_t<!nstd::is_execution_policy<Readable>::value,
                                                      nstd::utility::pair<SinglePass, Callable>>;
                template <typename ForwardIterator, typename EndPoint, typename Callable>
                auto operator()(ForwardIterator, EndPoint, Callable) const
                    -> nstd::type_traits::enable_if_t<!nstd::is_execution_policy<ForwardIterator>::value,
                                                      nstd::utility::pair<ForwardIterator, Callable>>;

                template <typename ExecutionPolicy, typename Readable, typename MultiPass, typename EndPoint, typename Callable>
                auto operator()(ExecutionPolicy&&, Readable, MultiPass, EndPoint, Callable) const
                    -> nstd::type_traits::enable_if_t<nstd::is_execution_policy<ExecutionPolicy>::value>;

                template <typename ExecutionPolicy, typename ForwardIterator, typename EndPoint, typename Callable>
                auto operator()(ExecutionPolicy&&, ForwardIterator, EndPoint, Callable) const
                    -> nstd::type_traits::enable_if_t<nstd::is_execution_policy<ExecutionPolicy>::value>;
            };
        }
        constexpr nstd::algorithm::detail::transform transform{};
    }

}

// ----------------------------------------------------------------------------

#endif
