// small.hpp                                                          -*-C++-*-
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

#ifndef INCLUDED_SMALL
#define INCLUDED_SMALL

#include <algorithm>

// ----------------------------------------------------------------------------

namespace small {
    template <typename I>
    static inline unsigned sort(I p0, I p1, I p2);
    template <typename I>
    static inline unsigned sort(I p0, I p1, I p2, I p3);
    template <typename I>
    static inline unsigned sort(I p0, I p1, I p2, I p3, I p4);
}

// ----------------------------------------------------------------------------

template <typename I>
static inline unsigned small::sort(I p0, I p1, I p2) {
    using std::swap;
    if (*p1 < *p0) {
        if (*p2 < *p1) { // p2 < p1 && p1 < p0
            swap(*p0, *p2);
            return 1;
        }
        else if (*p2 < *p0) { // p1 <= p2 && p2 < p0
            auto tmp = std::move(*p0);
            *p0 = std::move(*p1);
            *p1 = std::move(*p2);
            *p2 = std::move(tmp);
            return 2;
        }
        else { // p1 < p0 && p0 <= p2
            swap(*p0, *p1);
            return 1;
        }
    }
    else {
        if (*p2 < *p0) { // p2 < p0 &&  < p0 <= p1
            auto tmp = std::move(*p2);
            *p2 = std::move(*p1);
            *p1 = std::move(*p0);
            *p0 = std::move(tmp);
            return 2;
        }
        else if (*p2 < *p1) { // p0 <= p2 && p2 < p1
            swap(*p1, *p2);
            return 1;
        }
        else { // else: p0 <= p1 <= p2
            return 0;
        }
    }
}

// ----------------------------------------------------------------------------

template <typename I>
static inline unsigned small::sort(I p0, I p1, I p2, I p3) {
    unsigned result = sort(p0, p1, p2);
    if (*p3 < *p2) {
        if (*p3 < *p1) {
            auto tmp = std::move(*p3);
            *p3 = std::move(*p2);
            *p2 = std::move(*p1);
            if (tmp < *p0) {
                *p1 = std::move(*p0);
                *p0 = std::move(tmp);
                result += 3;
            }
            else {
                *p1 = std::move(tmp);
                result += 2;
            }
        }
        else {
            std::iter_swap(p2, p3);
            ++result;
        }
    }
    return result;
}

// ----------------------------------------------------------------------------

template <typename I>
static inline unsigned small::sort(I p0, I p1, I p2, I p3, I p4) {
    unsigned result = sort(p0, p1, p2, p3);
    if (*p4 < *p3) {
        if (*p4 < *p2) {
            auto tmp = std::move(*p4);
            *p4 = std::move(*p3);
            *p3 = std::move(*p2);
            if (tmp < *p1) {
                *p2 = std::move(*p1);
                if (tmp < *p0) {
                    *p1 = std::move(*p0);
                    *p0 = std::move(tmp);
                    result += 4;
                }
                else {
                    *p1 = std::move(tmp);
                    result += 3;
                }
            }
            else {
                *p2 = std::move(tmp);
                result += 2;
            }
        }
        else {
            std::iter_swap(p3, p4);
            ++result;
        }
    }
    return result;
}

// ----------------------------------------------------------------------------

#endif
