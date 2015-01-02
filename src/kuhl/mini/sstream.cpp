// kuhl/mini/sstream.cpp                                              -*-C++-*-
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

#include "kuhl/mini/sstream.hpp"
#include "kuhl/mini/string.hpp"
#include <stddef.h>
#include <string.h>
#include <stdio.h>

namespace KM = kuhl::mini;

// ----------------------------------------------------------------------------

auto KM::stringbuf::overflow(int c)
    -> int {
    if (c != -1) {
        size_t size(this->pptr() - this->pbase() + 1u);
        size_t newsize(size < 16u? 16u: size * 2u);
        char* buffer = new char[newsize];
        if (this->pbase()) {
            memcpy(buffer, this->pbase(), size);
            delete[] this->pbase();
        }
        this->setp(buffer, buffer + newsize - 1u);
        this->pbump(size - 1u);
        *this->pptr() = c;
        this->pbump(1);
    }
    return c;
}

// ----------------------------------------------------------------------------

auto KM::stringbuf::str() const
    -> KM::string {
    if (this->pptr()) {
        *this->pptr() = 0;
        return KM::string(this->pbase());
    }
    return KM::string();
}

// ----------------------------------------------------------------------------

KM::ostringstream::ostringstream()
    : KM::stringbuf()
    , KM::ios(this)
    , KM::ostream(this) {
}

KM::ostringstream::~ostringstream() {
    delete[] this->pbase();
}

// ----------------------------------------------------------------------------

auto KM::ostringstream::str() const
    -> KM::string {
    return this->KM::stringbuf::str();
}
