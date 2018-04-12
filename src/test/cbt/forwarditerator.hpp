// cbt/forwarditeartor.hpp                                            -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2018 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_CBT_FORWARDITEARTOR
#define INCLUDED_CBT_FORWARDITEARTOR

// ----------------------------------------------------------------------------

#include "cbt/inputiterator.hpp"
#include "cbt/incrementable.hpp"
#include "cbt/detection.hpp"
#include "cbt/level.hpp"
#include "cbt/test.hpp"
#include <iterator>
#include <ostream>
#include <string>
#include <utility>

// ----------------------------------------------------------------------------

namespace cbt {
    // template <typename FwdIt>
    // concept ForwardIterator =
    //     InputIterator<FwdIt>
    //     && DerivedFrom<iterator_category_t<FwdIt>, forward_iterator_tag>
    //     && Incrementable<FwdIt>
    //     && Sentinel<FwdIt, FwdIt>;

    // — a == b implies ++a == ++b and
    // — The expression ([](X x){++x;}(a), *a) is equivalent to the expression *a. 

    template <typename FwdIt>
    bool ForwardIterator(std::ostream& out, std::string_view name,
                         FwdIt&& begin, FwdIt&& end) {
        (void)begin;
        (void)end;
        using T = std::decay_t<FwdIt>;
        return cbt::test({ cbt::InputIterator(out, name, begin, end)
                         , cbt::NestedRequirement<cbt::iterator_category_t, T>(out, "iterator_category_t<" + std::string(name) + ">",
                             [](std::ostream& out, auto name) {
                                 if (!std::is_base_of_v<std::forward_iterator_tag, cbt::iterator_category_t<T>>) {
                                     out << cbt::level::error << " iterator_category of " << name << " is not derived from forward_iterator_tag\n";
                                     return false;
                                 }
                                 return true;
                             })
                         , cbt::Incrementable(out, name, begin, end)
                         //     && Sentinel<FwdIt, FwdIt>;
                         //-dk:TODO , [&]{ out << cbt::level::warning << " while testing "
                         //-dk:TODO            << name << ": forward iterator test is incomplete\n";
                         //-dk:TODO     return true; }()
                         });
    }
}

// ----------------------------------------------------------------------------

#endif
