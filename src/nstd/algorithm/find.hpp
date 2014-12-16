// nstd/algorithm/find.hpp                                            -*-C++-*-
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

#ifndef INCLUDED_NSTD_ALGORITHM_FIND
#define INCLUDED_NSTD_ALGORITHM_FIND

#include "nstd/algorithm/find_if.hpp"
#include "nstd/utility/move.hpp"

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace algorithm {
        namespace detail {
            struct find {
                template <typename T>
                struct predicate {
                    T value;
                    predicate(T value): value(value) {}
                    template <typename S>
                    auto operator()(S&& value) const -> bool {
                        return this->value == value;
                    }
                };
            public:
                template <typename Projection,
                          typename SinglePass, typename EndPoint,
                          typename Value>
                auto operator()(Projection projection, SinglePass begin, EndPoint end, Value value) const -> SinglePass {
                    // return nstd::algorithm::find_if(projection, begin, end, predicate<Value>(nstd::utility::move(value)));
                    return nstd::algorithm::find_if(projection, begin, end, predicate<Value>(value));
                }
            };
        }
        constexpr nstd::algorithm::detail::find find{};
    }

}

// ----------------------------------------------------------------------------

#endif
