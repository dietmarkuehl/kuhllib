// nstd/algorithm/distance.hpp                                        -*-C++-*-
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

#ifndef INCLUDED_NSTD_ALGORITHM_DISTANCE
#define INCLUDED_NSTD_ALGORITHM_DISTANCE

#include "nstd/cursor/single_pass.hpp"

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace algorithm
    {
        namespace detail
        {
            struct distance
            {
                constexpr distance() noexcept(true) {}

                template <typename SinglePass, typename EndPoint>
                auto operator()(SinglePass it, EndPoint end) const -> nstd::cursor::difference_type_t<SinglePass>;
                //-dk:TODO random access version
                //-dk:TODO segmented version
            };
        }
        constexpr nstd::algorithm::detail::distance distance{};
    }
}

// ----------------------------------------------------------------------------

template <typename SinglePass, typename EndPoint>
auto nstd::algorithm::detail::distance::operator()(SinglePass it, EndPoint end) const -> nstd::cursor::difference_type_t<SinglePass> {
    namespace NC = nstd::cursor;
    nstd::cursor::difference_type_t<SinglePass> rc{};
    while (!NC::at_same_pos(it, end)) {
        ++rc;
        NC::step(it);
    }
    return rc;
}

// ----------------------------------------------------------------------------

#endif
