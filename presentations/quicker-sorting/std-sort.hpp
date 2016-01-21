// std-sort.hpp                                                       -*-C++-*-
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

#ifndef INCLUDED_STD_SORT
#define INCLUDED_STD_SORT

#include <algorithm>
#include <string>

// ----------------------------------------------------------------------------

namespace std_sort {
    struct sort;
    struct heap_sort;
    struct stable_sort;
}

// ----------------------------------------------------------------------------

struct std_sort::sort
{
    static std::string name() { return "std::sort"; }
    template <typename RndIt>
    static void run(RndIt begin, RndIt end) {
        std::sort(begin, end);
    }
};

// ----------------------------------------------------------------------------

struct std_sort::stable_sort
{
    static std::string name() { return "std::stable_sort"; }
    template <typename RndIt>
    static void run(RndIt begin, RndIt end) {
        std::stable_sort(begin, end);
    }
};

// ----------------------------------------------------------------------------

struct std_sort::heap_sort
{
    static std::string name() { return "std::heap_sort"; }
    template <typename RndIt>
    static void run(RndIt begin, RndIt end) {
        std::make_heap(begin, end);
        std::sort_heap(begin, end);
    }
};

// ----------------------------------------------------------------------------

#endif
