// algorithm/insertion_sort.hpp                                       -*-C++-*-
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

#ifndef INCLUDED_ALGORITHM_INSERTION_SORT
#define INCLUDED_ALGORITHM_INSERTION_SORT

#include "nstd/projection/identity.hpp"
#include "nstd/functional/less.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/utility/swap.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace algorithm {
        namespace detail {
            struct insertion_sort {
                constexpr insertion_sort() noexcept(true) {}

                template <typename BiDirIt,
                          typename Compare = ::nstd::functional::less<>>
                    void operator()(BiDirIt begin, BiDirIt end,
                                    Compare compare = Compare()) const;
                template <typename ReadWriteProjection,
                          typename BiDirIt,
                          typename Compare = ::nstd::functional::less<>>
                    void operator()(ReadWriteProjection projection,
                                    BiDirIt begin, BiDirIt end,
                                    Compare compare = Compare()) const;
                template <typename ReadProjection,
                          typename ReadWriteProjection,
                          typename BiDirIt,
                          typename Compare = ::nstd::functional::less<>>
                    void operator()(ReadProjection key,
                                    ReadWriteProjection projection,
                                    BiDirIt begin, BiDirIt end,
                                    Compare compare = Compare()) const;
            };
        }
        //-dk:TODO should be inline
        constexpr ::nstd::algorithm::detail::insertion_sort insertion_sort{};
    }
}

// ----------------------------------------------------------------------------

template <typename ReadProjection,
          typename ReadWriteProjection,
          typename BiDirIt, typename Compare>
void
nstd::algorithm::detail::insertion_sort::operator()(ReadProjection key,
                                                    ReadWriteProjection projection,
                                                    BiDirIt begin, BiDirIt end,
                                                    Compare compare) const {
    using nstd::utility::swap;
    if (begin != end) {
        for (auto it(begin); ++it != end; ) {
            auto tmp(projection(*it));
            auto pit(it), cit(it);
            for (;begin != pit && compare(tmp, key(*--pit)); --cit) {
                projection(*cit, projection(*pit));
            }
            projection(*cit, ::nstd::utility::move(tmp));
        }
    }
}

template <typename ReadWriteProjection,
          typename BiDirIt, typename Compare>
void
nstd::algorithm::detail::insertion_sort::operator()(ReadWriteProjection projection,
                                                    BiDirIt begin, BiDirIt end,
                                                    Compare compare) const {
    (*this)(::nstd::projection::identity, projection, begin, end, compare);
}

template <typename BiDirIt, typename Compare>
void
nstd::algorithm::detail::insertion_sort::operator()(BiDirIt begin, BiDirIt end,
                                                    Compare compare) const {
    (*this)(::nstd::projection::identity, begin, end, compare);
}

// ----------------------------------------------------------------------------

#endif
