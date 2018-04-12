// cbt/bidirectionaliterator.t.cpp                                    -*-C++-*-
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

#include "cbt/bidirectionaliterator.hpp"
#include "cbt/test.hpp"
#include <iterator>
#include <iostream>

// ----------------------------------------------------------------------------

namespace {
    struct BiDirIt {
        using value_type        = int;
        using difference_type   = signed int;
        using iterator_category = std::bidirectional_iterator_tag;

        int* d_it = nullptr;
        BiDirIt() = default;
        BiDirIt(BiDirIt const&) = default;
        BiDirIt(BiDirIt&&) = default;
        BiDirIt& operator= (BiDirIt const&) = default;

        friend bool operator== (BiDirIt it0, BiDirIt it1) { return it0.d_it == it1.d_it; }
        friend bool operator!= (BiDirIt it0, BiDirIt it1) { return it0.d_it != it1.d_it; }

        int operator*() { return *d_it; }
        BiDirIt& operator++() { ++this->d_it; return *(this); }
        BiDirIt  operator++(int) { int* tmp(this->d_it); ++this->d_it; return BiDirIt{tmp}; }
        BiDirIt& operator--() { --this->d_it; return *(this); }
        BiDirIt  operator--(int) { int* tmp(this->d_it); --this->d_it; return BiDirIt{tmp}; }
    };
}

// ----------------------------------------------------------------------------

int main()
{
    int array[] = { 1, 2 };
    return cbt::main_test({ cbt::BidirectionalIterator(std::cout, "int*",
                                                       std::begin(array), std::end(array))
                          , cbt::BidirectionalIterator(std::cout, "BiDirIt",
                                                       BiDirIt{std::begin(array)}, BiDirIt{std::end(array)})
                          });
}
