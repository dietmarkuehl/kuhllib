// nstd/algorithm/find_if.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_ALGORITHM_FIND_IF
#define INCLUDED_NSTD_ALGORITHM_FIND_IF

#include "nstd/cursor/single_pass.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace algorithm {
        namespace detail {
            struct find_if {
                constexpr find_if() noexcept(true) {}
                template <typename Readable,
                          typename SinglePass, typename EndPoint,
                          typename UnaryPredicate>
                auto operator()(Readable read, SinglePass it, EndPoint end, UnaryPredicate predicate) const -> SinglePass;
            };
        }
        constexpr nstd::algorithm::detail::find_if find_if{};
    }

}

// ----------------------------------------------------------------------------

template <typename Readable,
          typename SinglePass, typename EndPoint,
          typename UnaryPredicate>
auto nstd::algorithm::detail::find_if::operator()(Readable read,
                                                  SinglePass it, EndPoint end,
                                                  UnaryPredicate predicate) const
    -> SinglePass {
    while (!nstd::cursor::at_same_pos(it, end)
           && !predicate(read(nstd::cursor::key(it)))) {
        nstd::cursor::step(it);
    }
    return it;
}

// ----------------------------------------------------------------------------

#endif
