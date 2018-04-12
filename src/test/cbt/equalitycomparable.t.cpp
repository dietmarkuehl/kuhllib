// cbt/equalitycomparable.t.cpp                                       -*-C++-*-
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

#include "cbt/equalitycomparable.hpp"
#include <iostream>
#include <utility>

// ----------------------------------------------------------------------------

namespace {
    struct X {
        static constexpr char const* name = "X";
        X(X const&) = delete;
        X(X&&) = delete;
        int v;
    };

    //#define HAS_EQUALITY
#ifdef HAS_EQUALITY
    bool operator== (X&& x0, X&& x1) { return x0.v == x1.v; }
    //bool operator== (X const&& x0, X const&& x1) { return x0.v == x1.v; }
    bool operator== (X& x0, X& x1) { return x0.v == x1.v; }
    bool operator== (X const& x0, X const& x1) { return x0.v == x1.v; }
#endif
#define HAS_INEQUALITY
#ifdef HAS_INEQUALITY
    bool operator!= (X const& x0, X const& x1) { return x0.v != x1.v; }
#endif
}

// ----------------------------------------------------------------------------

template <typename T>
bool transitivity(std::initializer_list<std::initializer_list<T>> ) {
    return true;
}

// ----------------------------------------------------------------------------

int main()
{
#ifdef HAS_EQUALITY
    X       x1{1}, x2{2};
    X const c1{1}, c2{2};
    (void)(X{1} == X{2} && x1 == x2 && c1 == c2);
#endif
#ifdef HAS_INEQUALITY
    (void)(X{1} != X{2});
#endif

    return cbt::testEqualityComparable(
               { { X{0}, X{0} }, { X{1}, X{1} }, { X{2}, X{2} } })
        ? EXIT_SUCCESS: EXIT_FAILURE;
}
