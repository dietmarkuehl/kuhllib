// nstd/algorithm/search.hpp                                          -*-C++-*-
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

#ifndef INCLUDED_NSTD_ALGORITHM_SEARCH
#define INCLUDED_NSTD_ALGORITHM_SEARCH

#include "nstd/algorithm/find_if.hpp"
#include "nstd/algorithm/mismatch.hpp"
#include "nstd/cursor/single_pass.hpp"
#include "nstd/functional/bind.hpp"
#include "nstd/functional/equal_to.hpp"
#include "nstd/functional/placeholders.hpp"
#include "nstd/functional/reference_wrapper.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace algorithm {
        namespace detail {
            struct search {
                template <typename Readable1,
                          typename MultiPass1, typename EndPoint1,
                          typename Readable2,
                          typename MultiPass2, typename EndPoint2,
                          typename BinaryPredicate>
                auto operator()(Readable1 readable1, MultiPass1 begin1,  EndPoint1 end1,
                                Readable2 readable2, MultiPass2 begin2, EndPoint2 end2,
                                BinaryPredicate predicate) const
                    -> MultiPass1 {
                    //-dk:TODO static assert for multi-pass cursor
                    namespace NA = nstd::algorithm;
                    namespace NC = nstd::cursor;
                    namespace NF = nstd::functional;
                    namespace NFP = NF::placeholders;
                    if (NC::at_same_pos(begin2, end2)) {
                        return begin1;
                    }
                    MultiPass1 cur1(begin1);
                    auto find_predicate(NF::bind(predicate, NFP::_1, readable2(NC::key(begin2))));
                    NC::step(begin2);
                    while (!NC::at_same_pos(cur1 = NA::find_if(readable1, cur1, end1, find_predicate), end1)) {
                        begin1 = cur1;
                        NC::step(cur1);
                        auto p = NA::mismatch(readable1, cur1, end1,
                                              readable2, begin2, end2,
                                              predicate);
                        if (NC::at_same_pos(p.second, end2)) {
                            return begin1;
                        }
                        else if (NC::at_same_pos(p.first, end1)) {
                            return p.first;
                        }
                    }
                    return cur1;
                }
                template <typename Readable1,
                          typename MultiPass1, typename EndPoint1,
                          typename Readable2,
                          typename MultiPass2, typename EndPoint2>
                auto operator()(Readable1 readable1, MultiPass1 begin1, EndPoint1 end1,
                                Readable2 readable2, MultiPass2 begin2, EndPoint2 end2) const
                    -> MultiPass1 {
                    return this->operator()(readable1, begin1, end1,
                                            readable2, begin2, end2,
                                            nstd::functional::equal_to<>());
                }
            };
        }
        constexpr nstd::algorithm::detail::search search{};
    }
}

// ----------------------------------------------------------------------------

#endif
