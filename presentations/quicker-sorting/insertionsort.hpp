// insertionsort.hpp                                                  -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2015 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_INSERTIONSORT
#define INCLUDED_INSERTIONSORT

#include <algorithm>
#include <iterator>
#include <utility>

// ----------------------------------------------------------------------------

namespace sort {
    struct insertion_sort;
    struct insertion_sort_sentinel;
    struct insertion_sort_rotate;
}

// ----------------------------------------------------------------------------

struct sort::insertion_sort
{
    template <typename BiDirIt>
    void operator()(BiDirIt begin, BiDirIt end) {
        for (BiDirIt current = begin; current != end; ++current) {
            BiDirIt it = current;
            while (begin != it && it[0] < it[-1]) {
                std::iter_swap(it, it - 1);
                --it;
            }
        }
    }
};

// ----------------------------------------------------------------------------

struct sort::insertion_sort_sentinel
{
    template <typename BiDirIt>
    void operator()(BiDirIt begin, BiDirIt end) {
        for (BiDirIt current = begin; current != end; ++current) {
            BiDirIt it = current;
            while (it[0] < it[-1]) {
                std::iter_swap(it, it - 1);
                --it;
            }
        }
    }
};

// ----------------------------------------------------------------------------

struct sort::insertion_sort_rotate
{
    template <typename BiDirIt>
    void operator()(BiDirIt begin, BiDirIt end) {
        typedef typename std::iterator_traits<BiDirIt>::value_type value_type;
        for (BiDirIt current = begin; current != end; ++current) {
            BiDirIt it = current;
            if (*it < it[-1]) {
                value_type tmp(std::move(*it));
                do {
                    *it = std::move(it[-1]);
                    --it;
                }
                while (tmp < it[-1]);
                *it = std::move(tmp);
            }
        }
    }
};

// ----------------------------------------------------------------------------

#endif
