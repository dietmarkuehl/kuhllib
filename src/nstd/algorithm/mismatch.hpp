// nstd/algorithm/mismatch.hpp                                        -*-C++-*-
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

#ifndef INCLUDED_NSTD_ALGORITHM_MISMATCH
#define INCLUDED_NSTD_ALGORITHM_MISMATCH

#include "nstd/cursor/single_pass.hpp"
#include "nstd/functional/equal_to.hpp"
#include "nstd/utility/pair.hpp"

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace algorithm {
        namespace detail {
            struct mismatch {
                constexpr mismatch() noexcept(true) {}
                template <typename Readable1, typename SinglePass1, typename EndPoint1,
                          typename Readable2, typename SinglePass2, typename EndPoint2,
                          typename Predicate>
                auto operator()(Readable1, SinglePass1, EndPoint1,
                                Readable2, SinglePass2, EndPoint2,
                                Predicate) const
                    -> nstd::utility::pair<SinglePass1, SinglePass2>;
                template <typename Readable1, typename SinglePass1, typename EndPoint1,
                          typename Readable2, typename SinglePass2, typename EndPoint2>
                auto operator()(Readable1, SinglePass1, EndPoint1,
                                Readable2, SinglePass2, EndPoint2) const
                    -> nstd::utility::pair<SinglePass1, SinglePass2>;
            };
        }
        constexpr nstd::algorithm::detail::mismatch mismatch{};
    }

}

// ----------------------------------------------------------------------------

template <typename Readable1, typename SinglePass1, typename EndPoint1,
          typename Readable2, typename SinglePass2, typename EndPoint2,
          typename Predicate>
auto nstd::algorithm::detail::mismatch::operator()(Readable1 readable1, SinglePass1 cursor1, EndPoint1 end1,
                                                   Readable2 readable2, SinglePass2 cursor2, EndPoint2 end2,
                                                   Predicate predicate) const
    -> nstd::utility::pair<SinglePass1, SinglePass2>
{
    while (!nstd::cursor::at_same_pos(cursor1, end1)
           && !nstd::cursor::at_same_pos(cursor2, end2)
           && predicate(readable1(nstd::cursor::key(cursor1)), readable2(nstd::cursor::key(cursor2)))) {
        nstd::cursor::step(cursor1);
        nstd::cursor::step(cursor2);
    }
    return nstd::utility::pair<SinglePass1, SinglePass2>(cursor1, cursor2);
}

template <typename Readable1, typename SinglePass1, typename EndPoint1,
          typename Readable2, typename SinglePass2, typename EndPoint2>
auto nstd::algorithm::detail::mismatch::operator()(Readable1 readable1, SinglePass1 begin1, EndPoint1 end1,
                                                   Readable2 readable2, SinglePass2 begin2, EndPoint2 end2) const
    -> nstd::utility::pair<SinglePass1, SinglePass2>
{
    return this->operator()(readable1, begin1, end1, readable2, begin2, end2, nstd::functional::equal_to<>());
}

// ----------------------------------------------------------------------------

#endif
