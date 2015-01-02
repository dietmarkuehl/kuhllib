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
#include "nstd/utility/pair.hpp"
#include "nstd/utility/move.hpp"

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace algorithm {
        namespace detail {
            struct for_each {
                constexpr for_each() noexcept(true) {}
                template <typename Readable, typename SinglePass, typename EndPoint, typename Callable>
                auto operator()(Readable, SinglePass, EndPoint, Callable) const
                    -> nstd::utility::pair<SinglePass, Callable>;
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
    -> nstd::utility::pair<SinglePass, Callable>
{
    namespace NC = nstd::cursor;
    for (; !NC::at_same_pos(cur, end); NC::step(cur)) {
        fun(readable(NC::key(cur)));
    }
    return nstd::utility::make_pair(cur, ::nstd::utility::move(fun));
}

// ----------------------------------------------------------------------------

#endif
