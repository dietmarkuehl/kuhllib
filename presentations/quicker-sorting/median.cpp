// median.cpp                                                         -*-C++-*-
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

#include "median.h"
#include <vector>
#include <cassert>

// ----------------------------------------------------------------------------

bool test_median(int expect, int i0, int i1, int i2) {
    std::vector<int> v{ i0, i1, i2 };
    return v.begin() + expect == quicksort::find_median(v.begin() + 0, v.begin() + 1, v.begin() + 2);
}

// ----------------------------------------------------------------------------

int main() {
    assert(test_median(1, 1, 2, 3));
    assert(test_median(0, 2, 1, 3));
    assert(test_median(0, 2, 3, 1));
    assert(test_median(2, 1, 3, 2));
    assert(test_median(2, 3, 1, 2));
    assert(test_median(1, 3, 2, 1));
}
