// cbt/constructible.t.cpp                                            -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2017 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "cbt/constructible.hpp"
#include "cbt/test.hpp"
#include <iostream>

// ----------------------------------------------------------------------------

namespace {
    struct indestructible {
        indestructible() {}
        ~indestructible() = delete;
    };

    struct throwdestructible {
        throwdestructible() {}
        ~throwdestructible() noexcept(false) {}
    };

    struct simple {
        int value;
        simple(int value): value(value) {}
    };
    bool test_equal(simple const& s0, simple const& s1) {
        return s0.value == s1.value;
    }

    struct comparable {
        int value;
        explicit comparable(int value): value(value) {}
    };
    bool operator== (comparable const& s0, comparable const& s1) {
        return s0.value == s1.value;
    }

    struct not_comparable {
        int value;
        not_comparable(int value): value(value) {}
    };

    struct odd_comparable {
        odd_comparable() {}
    };
    bool test_equal(odd_comparable const& o0, odd_comparable const& o1) {
        return &o0 == &o1;
    }
}

// ----------------------------------------------------------------------------

int main()
{
    return cbt::test(
                     !cbt::Constructible<indestructible>(std::cout, "indestructible"),
                     !cbt::Constructible<throwdestructible>(std::cout, "throwdestructible"),
                     !cbt::Constructible<simple>(std::cout, "simple"),
                     cbt::Constructible<simple, int>(std::cout, "simple"),
                     cbt::Constructible<simple>(std::cout, "simple", 17),
                     cbt::Constructible<comparable>(std::cout, "comparable", 17),
                     cbt::Constructible<not_comparable>(std::cout, "not_comparable", 17),
                     !cbt::Constructible<odd_comparable>(std::cout, "odd_comparable")
                     );
}
