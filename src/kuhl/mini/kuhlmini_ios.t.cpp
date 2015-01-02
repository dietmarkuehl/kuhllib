// kuhl/mini/kuhlmini_ios.t.cpp                                       -*-C++-*-
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

#include "kuhl/mini/ios.hpp"
#include "kuhl/mini/streambuf.hpp"
#include <stdlib.h>

namespace KM = kuhl::mini;

// ----------------------------------------------------------------------------

namespace {
    class testios
        : public KM::ios {
    public:
        testios(KM::streambuf* sbuf)
            : KM::ios(0) {
            if (this->rdbuf() == 0) {
                this->init(sbuf);
            }
        }
    };
}

// ----------------------------------------------------------------------------

static auto test()
    -> bool {
    KM::streambuf sbuf;
    KM::ios       ios0(0);
    KM::ios       ios(&sbuf);
    testios       tios(&sbuf);
    return ios0.rdbuf() == 0
        && ios.rdbuf()  == &sbuf
        && tios.rdbuf() == &sbuf
        && ios.width()  == 0
        && ios.width(17) == 0
        && ios.width() == 17
        && ios.fill() == ' '
        && ios.fill('@') == ' '
        && ios.fill() == '@'
        && ios.flags() == KM::ios::dec
        && ios.setf(KM::ios::left, KM::ios::adjustfield) == KM::ios::dec
        && ios.flags() == (KM::ios::dec | KM::ios::left)
        && ios.setf(KM::ios::right, KM::ios::adjustfield) == (KM::ios::dec | KM::ios::left)
        && ios.flags() == (KM::ios::dec | KM::ios::right)
        ;
}

// ----------------------------------------------------------------------------

auto main()
    -> int
{
    return test()? EXIT_SUCCESS: EXIT_FAILURE;
}
