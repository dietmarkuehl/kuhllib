// median.h                                                           -*-C++-*-
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

#ifndef INCLUDED_MEDIAN
#define INCLUDED_MEDIAN

#include <algorithm>

// ----------------------------------------------------------------------------

namespace quicksort {
    template <typename Iterator>
    Iterator find_median(Iterator it1, Iterator it2, Iterator it3) {
        if (*it1 < *it2) {
            if (*it2 < *it3) {
                return it2;
            }
            else if (*it1 < *it3) {
                return it3;
            }
            else {
                return it1;
            }
        }
        else {
            if (*it1 < *it3) {
                return it1;
            }
            else if (*it3 < *it2) {
                return it2;
            }
            else {
                return it3;
            }
        }
    }
    template <typename Iterator>
    void get_median(Iterator it1, Iterator it2, Iterator it3) {
        // puts the median into the iterator pointed to by it3
        if (*it1 < *it2) {
            if (*it2 < *it3) {
                std::iter_swap(it2, it3);
            }
            else if (*it1 < *it3) {
            }
            else {
                std::iter_swap(it1, it3);
            }
        }
        else {
            if (*it1 < *it3) {
                std::iter_swap(it1, it3);
            }
            else if (*it3 < *it2) {
                std::iter_swap(it2, it3);
            }
            else {
            }
        }
    }
}

// ----------------------------------------------------------------------------

#endif
