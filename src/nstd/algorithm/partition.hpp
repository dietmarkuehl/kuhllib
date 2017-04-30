// nstd/algorithm/partition.hpp                                       -*-C++-*-
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

#ifndef INCLUDED_NSTD_ALGORITHM_PARTITION
#define INCLUDED_NSTD_ALGORITHM_PARTITION

#include "nstd/utility/swap.hpp"
#include "nstd/projection/identity.hpp"
#include "nstd/execution/is_execution_policy.hpp"
#include "nstd/type_traits/enable_if.hpp"

// ----------------------------------------------------------------------------
// Move values with pred(value) == true in front of those with pred(value) == false.

namespace nstd {
    namespace algorithm {
        template <typename Readable, typename ForwardCursor, typename EndPoint,
                  typename Predicate>
        auto partition(Readable readable, ForwardCursor begin, EndPoint end, Predicate pred)
            -> ::nstd::type_traits::enable_if_t<!::nstd::is_execution_policy<Readable>::value, ForwardCursor>;
        template <typename ForwardCursor, typename EndPoint, typename Predicate>
        auto partition(ForwardCursor begin, EndPoint end, Predicate pred)
            -> ForwardCursor;
        template <typename Policy,
                  typename Readable, typename ForwardCursor, typename EndPoint,
                  typename Predicate>
        auto partition(Policy&& policy, Readable readable, ForwardCursor begin, EndPoint end,
                       Predicate pred)
            -> ForwardCursor;
        template <typename Policy,
                  typename ForwardCursor, typename EndPoint, typename Predicate>
        auto partition(Policy&& policy, ForwardCursor begin, EndPoint end, Predicate pred)
            -> ::nstd::type_traits::enable_if_t<::nstd::is_execution_policy<Policy>::value, ForwardCursor>;
    }
}

// ----------------------------------------------------------------------------

template <typename Readable, typename ForwardCursor, typename EndPoint,
          typename Predicate>
auto ::nstd::algorithm::partition(Readable readable, ForwardCursor begin, EndPoint end, Predicate pred)
    -> ::nstd::type_traits::enable_if_t<!::nstd::is_execution_policy<Readable>::value, ForwardCursor> {
    //-dk:TODO needs to delegate to algorithm with swap-projection
    ForwardCursor to(begin);
    while (to != end && pred(readable(*to))) {
        ++to;
    }
    if (to != end) {
        ForwardCursor from(to);
        while (++from != end) {
            if (pred(readable(*from))) {
                using ::nstd::utility::swap;
                swap(readable(*to), readable(*from));
                ++to;
            }
        }
    }
    return to;
}

template <typename ForwardCursor, typename EndPoint, typename Predicate>
auto ::nstd::algorithm::partition(ForwardCursor begin, EndPoint end, Predicate pred)
    -> ForwardCursor {
    return ::nstd::algorithm::partition(::nstd::projection::identity,
                                        begin, end, pred);
}

// ----------------------------------------------------------------------------

#endif
