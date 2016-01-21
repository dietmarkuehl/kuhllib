// quicksort.hpp                                                      -*-C++-*-
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

#ifndef INCLUDED_QUICKSORT
#define INCLUDED_QUICKSORT

#include "insertionsort.hpp"
#include <algorithm>
#include <iterator>
#include <string>

// ----------------------------------------------------------------------------

namespace quicksort {
    template <int Size, typename Partition> struct simple_quicksort;
    template <int Size, typename Partition> struct fat_quicksort;
    template <int Size, typename Sort> struct insertion_quicksort;
    template <int Size, typename Sort> struct insertion_sentinel_quicksort;
    template <int Size, typename Sort> struct insertion_rotate_quicksort;
}

// ----------------------------------------------------------------------------

template <int Size, typename Partition>
struct quicksort::simple_quicksort {
    static std::string name() { return "simple-quick sort/" + Partition::name(); }
    template <typename RndIt>
    static void run(RndIt begin, RndIt end) {
        typedef typename std::iterator_traits<RndIt>::value_type value_type;
        if (Size < std::distance(begin, end)) {
            RndIt pivot(end - 1);
            RndIt mid = Partition::run(begin, end - 1,
                                        [=](value_type const& arg){ return arg < *pivot; });
            run(begin, mid);
            std::iter_swap(mid, pivot);
            run(mid + 1, end);
        }
    }
};

// ----------------------------------------------------------------------------

template <int Size, typename Partition>
struct quicksort::fat_quicksort {
    Partition partition;
    template <typename RndIt>
    void sort(RndIt begin, RndIt end) {
        typedef typename std::iterator_traits<RndIt>::value_type value_type;
        if (Size < std::distance(begin, end)) {
            RndIt pivot(end - 1);
            RndIt mid = this->partition(begin, end - 1,
                                        [=](value_type const& arg){ return arg < *pivot; });
            if (begin != mid) {
                this->sort(begin, mid);
            }
            else {
                mid = this->partition(begin, end - 1,
                                      [=](value_type const& arg){ return !(*pivot < arg); });
            }
            std::iter_swap(mid, pivot);
            this->sort(mid + 1, end);
        }
    }
};

// ----------------------------------------------------------------------------

template <int Size, typename Sort>
struct quicksort::insertion_quicksort {
    template <typename RndIt>
    void sort(RndIt begin, RndIt end) {
        Sort().sort(begin, end);
        sort::insertion_sort()(begin, end);
    }
};

// ----------------------------------------------------------------------------

template <int Size, typename Sort>
struct quicksort::insertion_sentinel_quicksort {
    template <typename RndIt>
    void sort(RndIt begin, RndIt end) {
        if (std::distance(begin, end) < Size) {
            sort::insertion_sort()(begin, end);
        }
        else {
            Sort().sort(begin, end);
            sort::insertion_sort()(begin, begin + Size);
            sort::insertion_sort_sentinel()(begin + Size, end);
        }
    }
};

// ----------------------------------------------------------------------------

template <int Size, typename Sort>
struct quicksort::insertion_rotate_quicksort {
    template <typename RndIt>
    void sort(RndIt begin, RndIt end) {
        if (std::distance(begin, end) < Size) {
            sort::insertion_sort()(begin, end);
        }
        else {
            Sort().sort(begin, end);
            sort::insertion_sort()(begin, begin + Size);
            sort::insertion_sort_rotate()(begin + Size, end);
        }
    }
};

// ----------------------------------------------------------------------------

#endif
