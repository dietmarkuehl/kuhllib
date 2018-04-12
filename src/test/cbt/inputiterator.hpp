// cbt/inputiterator.hpp                                              -*-C++-*-
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

#ifndef INCLUDED_CBT_INPUTITERATOR
#define INCLUDED_CBT_INPUTITERATOR

#include "cbt/iterator.hpp"
#include "cbt/readable.hpp"
#include "cbt/detection.hpp"
#include "cbt/level.hpp"
#include "cbt/test.hpp"
#include <iterator>
#include <ostream>
#include <string>
#include <utility>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace cbt {
                         
    // template <typename InIt>
    // concept InputIterator =
    //     Iterator<InIt>
    //     && Readable<InIt>
    //     && requires { typename iterator_category_t<InIt>; }
    //     && DerivedFrom<iterator_category_t<InIt>, input_iterator_tag>;

    template <typename InIt>
    bool InputIterator(std::ostream& out, std::string_view name, InIt&& begin, InIt&& end) {
        using T = std::decay_t<InIt>;
        return cbt::test({ cbt::Iterator(out, name, begin, end)
                         , cbt::Readable<T>(out, name)
                         , cbt::TypeRequirement<cbt::iterator_category_t, T>(out, name, "iterator_category")
                         , cbt::NestedRequirement<cbt::iterator_category_t, T>(out, "iterator_category_t<" + std::string(name) + ">",
                             [](std::ostream& out, auto name) {
                                 if (!std::is_base_of_v<std::input_iterator_tag, cbt::iterator_category_t<T>>) {
                                     out << cbt::level::error << " iterator_category of " << name << " is not derived from input_iterator_tag\n";
                                     return false;
                                 }
                                 return true;
                             })
                         });
    }
}

// ----------------------------------------------------------------------------

#endif
