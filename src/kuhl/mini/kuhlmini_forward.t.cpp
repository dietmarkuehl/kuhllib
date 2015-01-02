// kuhl/mini/kuhlmini_forward.t.cpp                                   -*-C++-*-
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

#include "kuhl/mini/forward.hpp"
#include <stdlib.h>

namespace KM = kuhl::mini;

// ----------------------------------------------------------------------------

namespace {
    template <typename, typename>
    struct is_same {
        static constexpr bool value = false;
    };
    template <typename T>
    struct is_same<T, T> {
        static constexpr bool value = true;
    };
}

// ----------------------------------------------------------------------------

auto test()
    -> bool {
    int value(0);
    int cvalue(0);
    return is_same<int&&, decltype(KM::forward<int>(int()))>::value
        && is_same<int&, decltype(KM::forward<int&>(value))>::value
        && is_same<int const&, decltype(KM::forward<int const&>(cvalue))>::value
        && value == 0
        && cvalue == 0
        ;
}

// ----------------------------------------------------------------------------

auto main()
    -> int
{
    return test()? EXIT_SUCCESS: EXIT_FAILURE;
}
