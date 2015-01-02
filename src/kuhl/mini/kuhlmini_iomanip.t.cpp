// kuhl/mini/kuhlmini_iomanip.t.cpp                                   -*-C++-*-
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

#include "kuhl/mini/iomanip.hpp"
#include "kuhl/mini/ostream.hpp"
#include "kuhl/mini/streambuf.hpp"
#include <stdlib.h>

namespace KM = kuhl::mini;

// ----------------------------------------------------------------------------

static auto test_setfill()
    -> bool {
    KM::streambuf sbuf;
    KM::ostream out(&sbuf);
    return out.fill() == ' '
        && (out << KM::setfill('@'), true)
        && out.fill() == '@'
        ;
}

// ----------------------------------------------------------------------------

static auto test_setw()
    -> bool {
    KM::streambuf sbuf;
    KM::ostream out(&sbuf);
    return out.width() == 0
        && (out << KM::setw(42), true)
        && out.width() == 42
        ;
}

// ----------------------------------------------------------------------------

static auto test()
    -> bool {
    return test_setfill()
        && test_setw()
        ;
}

// ----------------------------------------------------------------------------

auto main()
    -> int
{
    return test()? EXIT_SUCCESS: EXIT_FAILURE;
}
