// algorithm/equal.hpp                                                -*-C++-*-
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

#ifndef INCLUDED_ALGORITHM_EQUAL
#define INCLUDED_ALGORITHM_EQUAL

#include "nstd/algorithm/mismatch.hpp"
#include "nstd/iterator/unreachable.hpp"
#include "nstd/projection/identity.hpp"
#include "nstd/utility/pair.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace algorithm {
        template <typename InIt1, typename EP1, typename InIt2>
        bool equal(InIt1 begin1, EP1 end1, InIt2 begin2);
        template <typename InIt1, typename EP1, typename InIt2, typename EP2>
        bool equal(InIt1 begin1, EP1 end1, InIt2 begin2, EP2 end2);
    }
}

// ----------------------------------------------------------------------------

template <typename InIt1, typename EP1, typename InIt2>
bool
nstd::algorithm::equal(InIt1 begin1, EP1 end1, InIt2 begin2) {
    return end1
        == ::nstd::algorithm::mismatch(::nstd::projection::identity, begin1, end1,
                                       ::nstd::projection::identity, begin2, ::nstd::iterator::unreachable).first;
}

template <typename InIt1, typename EP1, typename InIt2, typename EP2>
bool
nstd::algorithm::equal(InIt1 begin1, EP1 end1, InIt2 begin2, EP2 end2) {
    return ::nstd::utility::make_pair(end1, end2)
        == ::nstd::algorithm::mismatch(::nstd::projection::identity, begin1, end1,
                                       ::nstd::projection::identity, begin2, end2);
}

// ----------------------------------------------------------------------------

#endif
