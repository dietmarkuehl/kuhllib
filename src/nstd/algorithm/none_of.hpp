// nstd/algorithm/none_of.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_ALGORITHM_NONE_OF
#define INCLUDED_NSTD_ALGORITHM_NONE_OF

#include "nstd/cursor/single_pass.hpp"
#include "nstd/algorithm/find_if.hpp"

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace algorithm {
        namespace detail {
            struct none_of {
                constexpr none_of() noexcept(true) {}
                template <typename Readable, typename SinglePass, typename EndPoint, typename Predicate>
                auto operator()(Readable, SinglePass, EndPoint, Predicate) const -> bool;
            };
        }
        constexpr nstd::algorithm::detail::none_of none_of{};
    }

}

// ----------------------------------------------------------------------------

template <typename Readable, typename SinglePass, typename EndPoint, typename Predicate>
auto nstd::algorithm::detail::none_of::operator()(Readable readable,
                                                  SinglePass begin, EndPoint end,
                                                  Predicate predicate) const -> bool
{
    return nstd::cursor::at_same_pos(nstd::algorithm::find_if(readable, begin, end, predicate), end);
}

#endif
